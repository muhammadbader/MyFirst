package bgu.spl.mics.application.passiveObjects;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.*;
import java.util.LinkedList;
import java.util.List;

/**
 *  That's where Q holds his gadget (e.g. an explosive pen was used in GoldenEye, a geiger counter in Dr. No, etc).
 * <p>
 * This class must be implemented safely as a thread-safe singleton.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You can add ONLY private fields and methods to this class as you see fit.
 */
public class Inventory {
	private List<String> gadgets=new LinkedList<>();
	private static class singletonInventory{
		private static Inventory instance=new Inventory();
	}
	/**
     * Retrieves the single instance of this class.
     */
	public static Inventory getInstance() {
		//TODO: Implement this
		return singletonInventory.instance;
	}
	public boolean isEmpty(){
		return gadgets.isEmpty();
	}
	/**
     * Initializes the inventory. This method adds all the items given to the gadget
     * inventory.
     * <p>
     * @param inventory 	Data structure containing all data necessary for initialization
     * 						of the inventory.
     */

	public void load (String[] inventory) {
		//TODO: Implement this
		for(int i=0;i<inventory.length;i++){
			gadgets.add(inventory[i]);
		}

	}
	
	/**
     * acquires a gadget and returns 'true' if it exists.
     * <p>
     * @param gadget 		Name of the gadget to check if available
     * @return 	‘false’ if the gadget is missing, and ‘true’ otherwise
     */
	public synchronized boolean getItem(String gadget){
		//TODO: Implement this
		for(String e:gadgets){
			if(e.equals(gadget)) {
				gadgets.remove(e);
				return true;
			}
		}
		return false;
	}

	/**
	 *
	 * <p>
	 * Prints to a file name @filename a serialized object List<Gadget> which is a
	 * List of all the Gadgets in the diary.
	 * This method is called by the main method in order to generate the output.
	 */
	public void printToFile(String filename){

		JSONObject output = new JSONObject();
		JSONArray GadgetArray = new JSONArray();

		for(int i=0;i<gadgets.size();i++) {
			GadgetArray.add(gadgets.get(i));

		}
		output.put("Gadgets",GadgetArray);
		try {
			FileWriter file = new FileWriter(filename);
			file.write(output.toJSONString());
			file.flush();
			file.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		}
	}

