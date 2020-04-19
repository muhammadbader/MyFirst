package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Future;
import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.AgentsAvailableEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.messages.lastTick;
import bgu.spl.mics.application.passiveObjects.Squad;
import javafx.util.Pair;

import java.util.List;
import java.util.concurrent.CountDownLatch;

/**
 * Only this type of Subscriber can access the squad. Three are several
 * Moneypenny-instances - each of them holds a unique serial number that will
 * later be printed on the report.
 *
 * You can add private fields and public methods to this class. You MAY change
 * constructor signatures and even add new public constructors.
 */
public class Moneypenny extends Subscriber {

	private int currentTick;
	private Squad squad;
	private int id;
	private CountDownLatch count;

	public Moneypenny(int id, CountDownLatch count) {
		super("Moneypenny");
		// TODO Implement this
		this.id = id;
		squad = Squad.getInstance();
		this.count = count;
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
		subscribeEvent(AgentsAvailableEvent.class, agents -> {
			List<String> serial = agents.getSerial();
			List<String> names = null;
			boolean isit = squad.getAgents(serial);
			if (isit) {
				names = squad.getAgentsNames(serial);
				Future<Boolean> exec = new Future<>();
				Pair<Integer, Future<Boolean>> tmep1 = new Pair<Integer, Future<Boolean>>(id, exec);
				Pair<List<String>, List<String>> tmep2 = new Pair<List<String>, List<String>>(serial, names);
				Pair<Pair<List<String>, List<String>>, Pair<Integer, Future<Boolean>>> result = new Pair<Pair<List<String>, List<String>>, Pair<Integer, Future<Boolean>>>(
						tmep2, tmep1);
				complete(agents, result);

				if (exec != null && exec.get()) {
					squad.sendAgents(serial, agents.getDuration());
				} else {
					squad.releaseAgents(serial);
				}

			} else {
				complete(agents, null);
			}

		});

	}

}
