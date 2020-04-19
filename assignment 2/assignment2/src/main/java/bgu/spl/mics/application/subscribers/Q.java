package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.GadgetAvailableEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.messages.lastTick;
import bgu.spl.mics.application.passiveObjects.Inventory;
import javafx.util.Pair;

import java.util.concurrent.CountDownLatch;

/**
 * Q is the only Subscriber\Publisher that has access to the {@link bgu.spl.mics.application.passiveObjects.Inventory}.
 *
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Q extends Subscriber {

	private int currentTick;
	private Inventory inventory;
	private int timeQ;

	private CountDownLatch count;
	
	public Q( CountDownLatch count) {
		super("Q");
		// TODO Implement this
		inventory=Inventory.getInstance();
		this.count=count;
	}

	@Override
	protected void initialize() {
		// TODO Implement this
		subscribeBroadcast(TickBroadcast.class, (tick) -> {
			currentTick = tick.gettick();
		});
		subscribeBroadcast(lastTick.class,ter->{
			terminate();
			count.countDown();
		});
		
		subscribeEvent(GadgetAvailableEvent.class,gadgetcall->{
			timeQ=currentTick;
			String gadget=gadgetcall.getGadget();
			if(inventory.getItem(gadget)) {
				complete(gadgetcall,new Pair<>(true,new Pair<Integer,String>(timeQ,gadget)));
			}else {
				complete(gadgetcall,new Pair<>(false,null));
			}

		});
	}
}