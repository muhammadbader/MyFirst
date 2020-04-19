package bgu.spl.mics.application.messages;

import bgu.spl.mics.Broadcast;
import bgu.spl.mics.Event;
import bgu.spl.mics.Future;
import bgu.spl.mics.Message;
import bgu.spl.mics.application.passiveObjects.MissionInfo;
import javafx.util.Pair;

public class GadgetAvailableEvent implements Event<Pair<Boolean,Pair<Integer,String>>> {
	private String gadget;
	
	public GadgetAvailableEvent(String gadget) {
        this.gadget = gadget;
    }
    

    public String toString() {
        return "GadgetAvailableEvent";
        }
	
    public String getGadget() {
        return gadget;
    }
}
