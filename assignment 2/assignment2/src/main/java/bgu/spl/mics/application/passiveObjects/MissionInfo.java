package bgu.spl.mics.application.passiveObjects;

import java.util.LinkedList;
import java.util.List;

/**
 * Passive data-object representing information about a mission.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You may add ONLY private fields and methods to this class.
 */
public class MissionInfo {
	private String missionName;
	private List<String> serialAgentsNumbers=new LinkedList<>();
	private String gadget;
	private int timeIssued;
	private int timeExpired;
	private int duration;
	

    /**
     * Sets the name of the mission.
     */
    public void setMissionName(String missionName) {
        // TODO Implement this
    	this.missionName=missionName;
    }

	/**
     * Retrieves the name of the mission.
     */
	public String getMissionName() {//report
		// TODO Implement this
		return missionName;
	}

	static void bubbleSort(List<String> arr) {
		int n = arr.size();
		String temp =null;
		for (int i = 0; i < n; i++) {
			for (int j = 1; j < (n - i); j++) {
				if (arr.get(j-1).compareTo(arr.get(j)) >0) {
					//swap elements
					temp = arr.get(j-1);
					arr.set(j-1,arr.get(j)) ;
					arr.set(j,temp);
				}

			}
		}
	}
		/**
         * Sets the serial agent number.
         */
    public void setSerialAgentsNumbers(List<String> serialAgentsNumbers) {
        // TODO Implement this
		bubbleSort(serialAgentsNumbers);
    	for(String s:serialAgentsNumbers)
    		this.serialAgentsNumbers.add(s);
    }

	/**
     * Retrieves the serial agent number.
     */
	public List<String> getSerialAgentsNumbers() {//Moneypenny
		// TODO Implement this
		return serialAgentsNumbers;
	}

    /**
     * Sets the gadget name.
     */
    public void setGadget(String gadget) {
        // TODO Implement this
    	this.gadget=gadget;
    }

	/**
     * Retrieves the gadget name.
     */
	public String getGadget() {//Q
		// TODO Implement this
		return gadget;
	}

    /**
     * Sets the time the mission was issued in milliseconds.
     */
    public void setTimeIssued(int timeIssued) {
        // TODO Implement this
    	this.timeIssued=timeIssued;
    }

	/**
     * Retrieves the time the mission was issued in milliseconds.
     */
	public int getTimeIssued() {//Intelligence
		// TODO Implement this
		return timeIssued;
	}

    /**
     * Sets the time that if it that time passed the mission should be aborted.
     */
    public void setTimeExpired(int timeExpired) {
        // TODO Implement this
    	this.timeExpired=timeExpired;
    }

	/**
     * Retrieves the time that if it that time passed the mission should be aborted.
     */
	public int getTimeExpired() {//Intelligence
		// TODO Implement this
		return timeExpired;
	}

    /**
     * Sets the duration of the mission in time-ticks.
     */
    public void setDuration(int duration) {
        // TODO Implement this
    	this.duration=duration;
    }

	/**
	 * Retrieves the duration of the mission in time-ticks.
	 */
	public int getDuration() {//agent
		// TODO Implement this
		return duration;
	}
}
