package bgu.spl.net.impl;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;


public class ConnectionsImpl implements Connections<STOMPMessage> {
        private static class SingletonConnections {
            private static Connections<STOMPMessage> instance = new ConnectionsImpl();
        }
    private ConcurrentHashMap<Integer, ConnectionHandler> handlerMap = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, Readers> readerMap = new ConcurrentHashMap<>();
    private ConcurrentHashMap<Integer, Readers> HandlerReader = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, LinkedList<ConnectionHandler>> TopicMap = new ConcurrentHashMap<>();
    private Object lock = new Object();
    private AtomicInteger id=new AtomicInteger(0);


    public static ConnectionsImpl getInstance(){
        return (ConnectionsImpl)SingletonConnections.instance;
    }
    public int getID(){
        return id.incrementAndGet();
    }

    @Override
    public boolean send(int connectionId, STOMPMessage msg) {
        synchronized (lock) {
            if (handlerMap.get(connectionId) == null)
                return false;
            handlerMap.get(connectionId).send(msg);
            return true;
        }
    }

    @Override
    public void send(String channel, STOMPMessage msg) {
        synchronized (lock) {
            LinkedList<ConnectionHandler> HANDLERS = TopicMap.get(channel);
            for (ConnectionHandler handler : HANDLERS) {
                handler.send(msg);

            }
        }
    }

    @Override
    public void disconnect(int connectionId) {
        synchronized (lock) {
            Readers logout = HandlerReader.remove(connectionId);
            if(logout!=null){
                ConnectionHandler willremove=handlerMap.remove(connectionId);
                for(String topic:logout.gettopics()){//remove the current client from all the topics
                    TopicMap.get(topic).remove(willremove);
                }
                logout.disconnect();//todo: send RECEIPT frame, in order for the client (and the connection handler) to close the channel
            }

        }

    }

    public void joinTopic(String topic, int id,String subsription) {
       synchronized (lock){
           Readers reader=HandlerReader.get(id);
           LinkedList<ConnectionHandler> target = TopicMap.get(topic);
           if (target != null) {
               target.add(handlerMap.get(id));
           } else {
               target = new LinkedList<>();
               target.add(handlerMap.get(id));
               TopicMap.put(topic, target);
           }
           reader.joinGenre(topic,subsription);
       }
    }
    public void exitTopic(String subscriptionID,int id){
        synchronized (lock){
            Readers reader=HandlerReader.get(id);
            String topic=reader.getTopic(subscriptionID);
            LinkedList<ConnectionHandler> target = TopicMap.get(topic);
            ConnectionHandler handler=handlerMap.get(id);
            if (target != null&&handler!=null) {
                target.remove(handler);
                reader.removeShelf(topic);
            }
        }
    }
    public String getSubscriptionID(int id,String topic){
        Readers reader=HandlerReader.get(id);
        if(reader!=null){
            return reader.getSubscriptionID(topic);
        }
        return null;

    }

    public void addHandler(int id, ConnectionHandler handler) {
        handlerMap.put(id, handler);
    }

    public boolean addreader(Readers reader) {
        synchronized (reader) {
            if(readerMap.get(reader.getName())==null) {
                readerMap.put(reader.getName(), reader);
                return true;
            }
            return false;
        }
    }

    public Readers getReader(String name) {
        synchronized (readerMap){
            return readerMap.get(name);
        }
    }
    public String getReadername(int id){
        return HandlerReader.get(id).getName();
    }

    public boolean connectReader(int id, Readers reader) {
        synchronized (reader){
            if(!reader.isConnect()){
                reader.connect();
                HandlerReader.put(id,reader);
                return true;
            }
            return false;
        }
    }

    public String login(String name, String pass) {
        synchronized (lock) {
            Readers curr = readerMap.get(name);
            if (curr == null) {
                return "new user";
            } else if (!curr.getPass().equals(pass)) {
                return "wrong password";
            } else if (curr.isConnect()) {
                return "logged in";
            }
            return "tamam";
        }
    }
}
