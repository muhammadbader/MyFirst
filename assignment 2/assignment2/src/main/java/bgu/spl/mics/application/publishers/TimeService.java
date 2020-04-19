package bgu.spl.mics.application.publishers;

import bgu.spl.mics.MessageBroker;
import bgu.spl.mics.MessageBrokerImpl;
import bgu.spl.mics.Publisher;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.messages.lastTick;

import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.CountDownLatch;

/**
 * TimeService is the global system timer There is only one instance of this Publisher.
 * It keeps track of the amount of ticks passed since initialization and notifies
 * all other subscribers about the current time tick using {@link TickBroadcast}.
 * This class may not hold references for objects which it is not responsible for.
 * 
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class TimeService extends Publisher {

	final private int time;//always 100 milli
	private int duration;
	private Timer timer ;
	private int current;
	private TimerTask doing;
	private CountDownLatch count;

	public TimeService(int duration, int time, CountDownLatch count) {
		super("TimeService");
		// TODO Implement this
		this.duration=duration;
		current=0;
		this.time=time;
		this.count=count;
	}

	@Override
	protected void initialize() {
		// TODO Implement this
		timer = new Timer();
		doing=new TimerTask() {
			public void run() {
				// TODO Auto-generated method stub
				if(current<duration)
				{
					getSimplePublisher().sendBroadcast(new TickBroadcast(current));
				}else {
					getSimplePublisher().sendBroadcast(new lastTick());
					timer.cancel();
				}
				current++;
			}
		};
	}
	@Override
	public void run() {
		// TODO Implement this
		initialize();
		timer.schedule(doing, time,time);
	}

}
