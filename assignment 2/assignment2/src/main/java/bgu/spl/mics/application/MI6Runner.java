package bgu.spl.mics.application;
import bgu.spl.mics.application.passiveObjects.*;
import bgu.spl.mics.application.publishers.TimeService;
import bgu.spl.mics.application.subscribers.Intelligence;
import bgu.spl.mics.application.subscribers.M;
import bgu.spl.mics.application.subscribers.Moneypenny;
import bgu.spl.mics.application.subscribers.Q;
import com.google.gson.*;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/** This is the Main class of the application. You should parse the input file,
 * create the different instances of the objects, and run the system.
 * In the end, you should output serialized objects.
 */
public class MI6Runner {
    public static void main(String[] args) throws FileNotFoundException {
        // TODO Implement this
    	bgu.spl.mics.application.passiveObjects.Inventory inventory=Inventory.getInstance();
    	Squad squad=Squad.getInstance();
    	Diary diary= Diary.getInstance();


		Thread timer;
		try {

    		JsonParser parser = new JsonParser();
			Object obj = parser.parse(new FileReader(args[0]));
			JsonObject jsonObj = (JsonObject) obj;
			JsonArray inputinventory = (JsonArray)jsonObj.getAsJsonArray("inventory");
			JsonArray inputsquad = (JsonArray)jsonObj.getAsJsonArray("squad");
			JsonObject inputservices = (JsonObject)jsonObj.get("services").getAsJsonObject();


			JsonArray Intelligence=(JsonArray)inputservices.getAsJsonArray("intelligence");
			int intelli=Intelligence.size();
			int Moneypenny=inputservices.get("Moneypenny").getAsInt();
			int m=inputservices.get("M").getAsInt();
			int ServicesNum=Moneypenny+m+1+intelli;
			ExecutorService threadPool = Executors.newFixedThreadPool(ServicesNum);
			CountDownLatch countDownlatch = new CountDownLatch(ServicesNum);

			String[] gadget=new String[inputinventory.size()];
			for (int i=0;i<inputinventory.size();i++){
				gadget[i]=inputinventory.get(i).getAsString();
			}
			inventory.load(gadget);

			for (int i=0;i<Intelligence.size();i++){
				Intelligence intel=new Intelligence(countDownlatch);
				JsonArray missions = (JsonArray)Intelligence.get(i).getAsJsonObject().get("missions");
				for(int k=0;k<missions.size();k++){
					MissionInfo mssn=new MissionInfo();
					JsonObject mission= (JsonObject) missions.get(k).getAsJsonObject();
					List<String> serialAgentsNumbers=new LinkedList<>();
					for (int q=0;q<mission.getAsJsonArray("serialAgentsNumbers").size();q++){
						serialAgentsNumbers.add(mission.getAsJsonArray("serialAgentsNumbers").get(q).getAsString());
					}
					mssn.setSerialAgentsNumbers(serialAgentsNumbers);
					int duration=mission.get("duration").getAsInt();
					String gadg=mission.get("gadget").getAsString();
					String missionName=mission.get("name").getAsString();
					int timeExpired=mission.get("timeExpired").getAsInt();
					int timeIssued=mission.get("timeIssued").getAsInt();

					mssn.setDuration(duration);
					mssn.setGadget(gadg);
					mssn.setMissionName(missionName);
					mssn.setTimeExpired(timeExpired);
					mssn.setTimeIssued(timeIssued);

					intel.addmission(mssn);
				}
				threadPool.execute(intel);
			}

			Agent[] agent=new Agent[inputsquad.size()];
			for (int i=0;i<inputsquad.size();i++){
				String name=inputsquad.get(i).getAsJsonObject().get("name").getAsString();
				String serial=inputsquad.get(i).getAsJsonObject().get("serialNumber").getAsString();
				Agent ag=new Agent();
				ag.setName(name);
				ag.setSerialNumber(serial);
				agent[i]=ag;
			}
			squad.load(agent);

			Q oneq=new Q(countDownlatch);

			for(int k=0;k<m;k++){
				M imm=new M(k,countDownlatch);
				threadPool.execute(imm);
			}
			threadPool.execute(oneq);
			//threads for Monneypennt
			for(int i=0;i<Moneypenny;i++){
				Moneypenny money=new Moneypenny(i,countDownlatch);
				threadPool.execute(money);
			}
			//threads for M

			int time=inputservices.get("time").getAsInt();
			TimeService timeservice=new TimeService(time,100,countDownlatch);
			timer=new Thread(timeservice);
			timer.start();

			countDownlatch.await();
			threadPool.shutdown();
		}catch(Exception e) {
    		e.printStackTrace();
    	}
		diary.printToFile(args[2]);
		inventory.printToFile(args[1]);

    }
}
