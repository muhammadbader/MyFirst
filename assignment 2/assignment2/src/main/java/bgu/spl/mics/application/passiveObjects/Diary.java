package bgu.spl.mics.application.passiveObjects;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.*;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

/**
 * Passive object representing the diary where all reports are stored.
 * <p>
 * This class must be implemented safely as a thread-safe singleton.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You can add ONLY private fields and methods to this class as you see fit.
 */
public class Diary {
	private List<Report> reports=new LinkedList<>();//atomic Reference
	private AtomicInteger total=new AtomicInteger();
	private static class SingletonDiary {
		private static Diary instance = new Diary();
	}
	Object lock=new Object();
	
	
	/**
	 * Retrieves the single instance of this class.
	 */
	public static Diary getInstance() {
		//TODO: Implement this
		return SingletonDiary.instance;
	}

	public List<Report> getReports() {
		return reports;
	}

	/**
	 * adds a report to the diary
	 * @param reportToAdd - the report to add
	 */
	public void addReport(Report reportToAdd){
		//TODO: Implement this
		synchronized (lock) {
			reports.add(reportToAdd);
		}
	}
	public void incremrnt() {
		total.incrementAndGet();
	}

	/**
	 *
	 * <p>
	 * Prints to a file name @filename a serialized object List<Report> which is a
	 * List of all the reports in the diary.
	 * This method is called by the main method in order to generate the output.
	 */
	public void printToFile(String filename){

		JSONObject jsonObject = new JSONObject();
		JSONObject output = new JSONObject();
		JSONArray serialNumberArray = new JSONArray();
		JSONArray agentsName = new JSONArray();
		JSONArray reportArray = new JSONArray();
		int a = reports.size();
		for(int i=0;i<a;i++) {
			serialNumberArray.add(reports.get(i).getAgentsSerialNumbersNumber().toString());
			agentsName.add(reports.get(i).getAgentsNames().toString());
			jsonObject.put("missionName",reports.get(i).getMissionName());
			jsonObject.put("m",reports.get(i).getM());
			jsonObject.put("moneypenny",reports.get(i).getMoneypenny());
			jsonObject.put("agentsSerialNumbers",serialNumberArray);
			jsonObject.put("agentsNames",agentsName);
			jsonObject.put("gadgetName",reports.get(i).getGadgetName());
			jsonObject.put("timeCreated",reports.get(i).getTimeCreated());
			jsonObject.put("timeIssued",reports.get(i).getTimeIssued());
			jsonObject.put("qTime",reports.get(i).getQTime());
			reportArray.add(jsonObject);
			jsonObject = new JSONObject();
			serialNumberArray = new JSONArray();
			agentsName = new JSONArray();
		}
		output.put("total",total.get());
		output.put("reports",reportArray);

		try {
			FileWriter file = new FileWriter(filename);
			file.write(output.toJSONString());
			file.flush();
			file.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Gets the total number of received missions (executed / aborted) be all the M-instances.
	 * @return the total number of received missions (executed / aborted) be all the M-instances.
	 */
	public int getTotal(){
		//TODO: Implement this
		return total.get();
	}
}
