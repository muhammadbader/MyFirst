package bgu.spl.mics.application.subscribers;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.concurrent.CountDownLatch;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.MissionReceivedEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.messages.lastTick;
import bgu.spl.mics.application.passiveObjects.*;;

/**
 * A Publisher\Subscriber.
 * Holds a list of Info objects and sends them
 *
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Intelligence extends Subscriber {
	private HashMap<Integer, LinkedList<MissionInfo>> infoMap = new HashMap<>();
	private int currentTick;
	private CountDownLatch count;

	public Intelligence( CountDownLatch count) {
		super("Intelligence");
		// TODO Implement this
		currentTick = 0;
		this.count=count;
	}


	public void addmission(MissionInfo mission) {//updating the missions
		if (infoMap.get(mission.getTimeIssued()) == null) {
			infoMap.put(mission.getTimeIssued(), new LinkedList<>());
			infoMap.get(mission.getTimeIssued()).add(mission);
		} else
			infoMap.get(mission.getTimeIssued()).add(mission);
	}


	@Override
	protected void initialize() {
		// TODO Implement this
		subscribeBroadcast(TickBroadcast.class, (tick) -> {
			currentTick = tick.gettick();
			if (infoMap.get(currentTick) != null) {
				LinkedList<MissionInfo> tmp = infoMap.get(currentTick);
				infoMap.remove(currentTick);//a clean up
				for (MissionInfo mssn : tmp) {
					MissionReceivedEvent mission = new MissionReceivedEvent(mssn);
					mission.settick(currentTick);
					getSimplePublisher().sendEvent(mission);
				}
			}
		});
		subscribeBroadcast(lastTick.class, ter -> {
			terminate();
			count.countDown();
		});
	}
}
