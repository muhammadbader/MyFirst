package bgu.spl.mics.application.messages;
import bgu.spl.mics.Event;
import bgu.spl.mics.application.passiveObjects.MissionInfo;

public class MissionReceivedEvent implements Event<MissionInfo>{

	private MissionInfo mission;
	private int tick;
	
	public MissionReceivedEvent(MissionInfo mission) {
        this.mission = mission;
    }
    public String toString() {
        return "MissionReceivedEvent";
        }
    public MissionInfo getMession() {
        return mission;
    }
    public void settick(int x) {
    	tick=x;
    }
    public int gettick() {
    	return tick;
    }
}
