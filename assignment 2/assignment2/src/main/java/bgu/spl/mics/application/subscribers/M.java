package bgu.spl.mics.application.subscribers;

import java.util.List;
import java.util.concurrent.CountDownLatch;

import bgu.spl.mics.Future;
import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.AgentsAvailableEvent;
import bgu.spl.mics.application.messages.GadgetAvailableEvent;
import bgu.spl.mics.application.messages.MissionReceivedEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.messages.lastTick;
import bgu.spl.mics.application.passiveObjects.Diary;
import bgu.spl.mics.application.passiveObjects.MissionInfo;
import bgu.spl.mics.application.passiveObjects.Report;
import javafx.util.Pair;

/**
 * M handles ReadyEvent - fills a report and sends agents to mission.
 *
 * You can add private fields and public methods to this class. You MAY change
 * constructor signatures and even add new public constructors.
 */

public class M extends Subscriber {
	
	private int m;
	private Diary diary;
	private int currentTick;
	private CountDownLatch count;

	public M(int M, CountDownLatch count) {
		super("M");
		// TODO Implement this
		diary = Diary.getInstance();
		m = M;
		this.count=count;
	}

	@Override
	protected void initialize() {
		// TODO Implement this
		subscribeBroadcast(TickBroadcast.class, (tick) -> {
			currentTick = tick.gettick();
		});
		subscribeBroadcast(lastTick.class, ter -> {
			terminate();
			count.countDown();
		});
		subscribeEvent(MissionReceivedEvent.class, mssn -> {
			diary.incremrnt();
			Report report = new Report();
			report.setTimeCreated(currentTick);
			MissionInfo mission = mssn.getMession();
			report.setMissionName(mission.getMissionName());
			report.setM(m);
			report.setTimeIssued(mssn.gettick());
			Future<Pair<Pair<List<String>, List<String>>, Pair<Integer, Future<Boolean>>>> future = getSimplePublisher().sendEvent(new AgentsAvailableEvent(mission.getSerialAgentsNumbers(), mission.getDuration()));
			//the first is the serial number
			if(future!=null){
			Pair<Pair<List<String>, List<String>>, Pair<Integer, Future<Boolean>>> tmp = future.get();
			//check if not null
			if (tmp != null) {//if future != null
				// update the report with the agents serial numbers
				report.setMoneypenny(tmp.getValue().getKey());
				Future<Boolean> tmpfuture = tmp.getValue().getValue();
				report.setAgentsSerialNumbersNumber(tmp.getKey().getKey());
				report.setAgentsNames(tmp.getKey().getValue());

				Future<Pair<Boolean, Pair<Integer, String>>> future2 = getSimplePublisher().sendEvent(new GadgetAvailableEvent(mission.getGadget()));
				if (future2 != null) {
					Pair<Boolean, Pair<Integer, String>> gad = future2.get();
					// update the report


					if (gad != null && gad.getKey()) {
						report.setGadgetName(gad.getValue().getValue());
						report.setQTime(gad.getValue().getKey());
						if (mission.getTimeExpired() >= currentTick) {

							tmpfuture.resolve(true);//execute the mission
							diary.addReport(report);
						} else {
							tmpfuture.resolve(false);//abort the mission
						}

					} else {
						tmpfuture.resolve(false);//abort the mission
					}

				} else {
					tmpfuture.resolve(false);//abort the mission
				}

			}
		}

		});

	}

}