package bgu.spl.mics.application.messages;

import bgu.spl.mics.Event;
import bgu.spl.mics.Future;
import bgu.spl.mics.Message;
import bgu.spl.mics.application.passiveObjects.MissionInfo;
import java.util.LinkedList;
import java.util.List;
import javafx.util.Pair;

public class AgentsAvailableEvent implements Event<Pair<Pair<List<String>,List<String>>,Pair<Integer,Future<Boolean>>>> {
    private int duration;
    private List<String> serial;
	
	public AgentsAvailableEvent(List<String> serial,int duration) {
        this.duration = duration;
        this.serial=serial;
    }

    public String toString() {
    return "AgentsAvailableEvent";
    }

    public int getDuration() {
        return duration;
    }
    public List<String> getSerial() {
        return serial;
    }
}
