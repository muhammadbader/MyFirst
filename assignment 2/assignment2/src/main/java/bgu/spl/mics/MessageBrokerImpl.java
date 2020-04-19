package bgu.spl.mics;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.LinkedBlockingQueue;


/**
 * The {@link MessageBrokerImpl class is the implementation of the MessageBroker
 * interface. Write your implementation here! Only private fields and methods
 * can be added to this class.
 */
public class MessageBrokerImpl implements MessageBroker {
	private static class SingletonMessageBroker {
		private static MessageBroker instance = new MessageBrokerImpl();
	}

	private Map<Subscriber, BlockingQueue<Message>> subqueue = new HashMap<>();
	private ConcurrentMap<Class<? extends Event<?>>, LinkedList<Subscriber>> eventMap = new ConcurrentHashMap<>();
	private ConcurrentMap<Class<? extends Broadcast>, LinkedList<Subscriber>> broadcastMap = new ConcurrentHashMap<>();
	private ConcurrentMap<Event<?>,Future<?>> futuremap=new ConcurrentHashMap<>();
	Object lock1= new Object();
	Object lock2= new Object();
	/**
	 * Retrieves the single instance of this class.
	 */
	public static MessageBroker getInstance() {
		// TODO: Implement this
		return SingletonMessageBroker.instance;
	}

	@Override
	public <T> void subscribeEvent(Class<? extends Event<T>> type, Subscriber m) {
		// TODO Auto-generated method stub
		synchronized (eventMap) {
			if (eventMap.get(type) == null) {
				eventMap.put(type, new LinkedList<Subscriber>());
				eventMap.get(type).add(m);
			} else {
				eventMap.get(type).add(m);
			}
		}
	}

	@Override
	public void subscribeBroadcast(Class<? extends Broadcast> type, Subscriber m) {
		// TODO Auto-generated method stub
		synchronized(lock1) {
			if (broadcastMap.get(type) == null) {
				broadcastMap.put(type, new LinkedList<Subscriber>());
				broadcastMap.get(type).add(m);
			} else {
				broadcastMap.get(type).add(m);
			}
			lock1.notifyAll();
		}
	}
	@SuppressWarnings("unchecked")
	@Override
	public <T> void complete(Event<T> e, T result) {
		while(futuremap.get(e)==null){
			if(futuremap.get(e)!=null)
				break;
		}
		// TODO Auto-generated method stub
		((Future<T>)futuremap.get(e)).resolve(result);
	}
	@Override
	public void sendBroadcast(Broadcast b) {
		// TODO Auto-generated method stub
		try {
			synchronized (lock1) {
				while (broadcastMap.get(b.getClass()) == null) {
					lock1.wait();
				}
				LinkedList<Subscriber> tmp = broadcastMap.get(b.getClass());
				for (Subscriber s : tmp) {
					subqueue.get(s).put(b);
				}
			}
		}catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	@SuppressWarnings("unchecked")
	@Override
	public synchronized <T> Future<T> sendEvent(Event<T> e) {
		// TODO Auto-generated method stub
		if (eventMap.get(e.getClass()) != null&&!eventMap.get(e.getClass()).isEmpty()) {//if anybody registered for this Event
			Subscriber _subscribed = eventMap.get(e.getClass()).pollFirst();
			if(_subscribed!=null) {
				eventMap.get(e.getClass()).addLast(_subscribed);
				subqueue.get(_subscribed).add(e);
				Future<T> temp;
				temp = new Future<T>();
				futuremap.put(e, temp);
				return temp;
			}
		}
		return null;
	}
	
	@Override
	public synchronized void register(Subscriber m) {
		// TODO Auto-generated method stub

		subqueue.put(m, new LinkedBlockingQueue<Message>());
	}

	@Override
	public synchronized void unregister(Subscriber m) {
		// TODO Auto-generated method stub

		System.out.println("unregeseter");
		while(!subqueue.get(m).isEmpty()){
			try {
				complete((Event<?>)subqueue.get(m).take(),null);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
			subqueue.remove(m);
			for (Class<?> key : eventMap.keySet()) {
				if (eventMap.get(key).contains(m)) {
					eventMap.get(key).remove(m);
					if(eventMap.get(key).isEmpty())
						eventMap.remove(key);
				}
			}
			for (Class<?> key : broadcastMap.keySet()) {
				if (broadcastMap.get(key).contains(m)) {
					broadcastMap.get(key).remove(m);
					if(broadcastMap.get(key).isEmpty())
						broadcastMap.remove(key);
				}
			}


	}

	@Override
	public Message awaitMessage(Subscriber m) throws InterruptedException {
		// TODO Auto-generated method stub
		return  subqueue.get(m).take();
	}
}