//package bgu.spl.mics;
//
//import bgu.spl.mics.application.passiveObjects.Agent;
//import bgu.spl.mics.application.passiveObjects.Squad;
//import org.junit.jupiter.api.AfterEach;
//import org.junit.jupiter.api.BeforeEach;
//import org.junit.jupiter.api.Test;
//
//import java.util.LinkedList;
//import java.util.List;
//import java.util.concurrent.atomic.AtomicBoolean;
//
//import static org.junit.Assert.*;
//
//public class SquadTest {
//	private Squad squad;
//
//	@BeforeEach
//	public void setUp() {
//		squad = new Squad();
//	}
//
//	@AfterEach
//	public void tearDown() {
//
//	}
//
//	@Test
//	public void testgetInstance() {
//		// TODO: change this test and add more tests :)
//		Squad first = squad.getInstance();
//		Squad second = squad.getInstance();
//		assertEquals(first, second);
//	}
//
//	@Test
//	public void testload() {
//		// TODO: change this test and add more tests :)
//		assertTrue("the squad  is Empty", squad.isEmpty());
//		Agent a1 = new Agent();
//		Agent a2 = new Agent();
//		Agent a3 = new Agent();
//		a1.setSerialNumber("bilal");
//		a2.setSerialNumber("billo");
//		a3.setSerialNumber("bolbol");
//		Agent[] aboalbalabil = { a1, a2, a3 };
//		squad.load(aboalbalabil);
//		assertFalse(squad.isEmpty());
//	}
//
//	@Test
//	public void testreleaseAgents() {
//		// TODO: change this test and add more tests :)
//		Agent[] s1 = { new Agent() };
//		s1[0].setSerialNumber("bolbolati");
//		squad.load(s1);
//		assertTrue(s1[0].isAvailable());
//		s1[0].acquire();
//		List<String> s = new LinkedList<String>();
//		s.add("bolbolati");
//		squad.releaseAgents(s);
//		assertFalse(!s1[0].isAvailable());
//	}
//
//	@Test
//	public void testsendAgents() {
//		// TODO: change this test and add more tests :
//		Agent a1 = new Agent();
//		Agent a2 = new Agent();
//		a1.setSerialNumber("bilal");
//		a2.setSerialNumber("billo");
//		Agent[] aboalbalabil = { a1, a2 };
//		squad.load(aboalbalabil);
//		List<String> serials = new LinkedList<>();
//		serials.add("bilal");
//		int timetest = 2000;
//		squad.sendAgents(serials, timetest);
//		long time = System.currentTimeMillis();
//		while (!a1.isAvailable()) {
//		}
//		assertTrue(System.currentTimeMillis() - time >= timetest);
//
//	}
//
//	@Test
//	public void testgetAgents() {
//		// TODO: change this test and add more tests :)
//		Agent a1 = new Agent();
//		Agent a2 = new Agent();
//		a1.setSerialNumber("bilal");
//		a2.setSerialNumber("billo");
//		Agent[] aboalbalabil = { a1, a2 };
//		squad.load(aboalbalabil);
//
//		List<String> serials = new LinkedList<>();
//		serials.add("bilal");
//
//
//		boolean test = squad.getAgents(serials);
//		assertTrue(test);
//		serials.add("khokha");
//		test = squad.getAgents(serials);
//		assertFalse(test);
//
//	}
//
//	@Test
//	public void testgetAgentsNames() {
//		// TODO: change this test and add more tests :)
//		Agent a1 = new Agent();
//		Agent a2 = new Agent();
//		a1.setSerialNumber("bilal");
//		a2.setSerialNumber("billo");
//		a1.setName("babo");
//		a2.setName("bolboly");
//		Agent[] aboalbalabil = { a1, a2 };
//		squad.load(aboalbalabil);
//		List<String> serials = new LinkedList<>();
//		serials.add(a1.getSerialNumber());
//		List<String> agentNames = squad.getAgentsNames(serials);
//		assertEquals(agentNames.size(), 1);
//		assertEquals(a1.getName(),agentNames.get(0));
//
//	}
//
//}
