//package bgu.spl.mics;
//
//import org.junit.jupiter.api.BeforeEach;
//import org.junit.jupiter.api.Test;
//
//import bgu.spl.mics.application.messages.AgentsAvailableEvent;
//import bgu.spl.mics.application.messages.GadgetAvailableEvent;
//import bgu.spl.mics.application.subscribers.Moneypenny;
//
//import static org.junit.jupiter.api.Assertions.*;
//
//public class MessageBrokerTest {
//	private MessageBroker messageB;
//
//	@BeforeEach
//	public void setUp() {
//		messageB = new MessageBrokerImpl();
//	}
//
//	// <T> void subscribeEvent(Class<? extends Event<T>> type, Subscriber s);
//
//	@SuppressWarnings("unchecked")
//	@Test
//	public void testsubscribeEvent() {
//		// TODO: change this test and add more tests :)
//		Subscriber s = new Moneypenny("cocojumpo",0);
//		Event<String> e = new GadgetAvailableEvent();
//		messageB.subscribeEvent((Class<? extends Event<String>>) e.getClass(), s);
//		Future f = messageB.sendEvent(e);
//		assertNotNull(f);
//	}
//
//	@Test
//	public void testregister() throws InterruptedException {
//		// TODO: change this test and add more tests :)
//		Subscriber s = new Moneypenny("samir",0);
//		messageB.register(s);
//		Event<String> e = new GadgetAvailableEvent();
//		messageB.subscribeEvent((Class<? extends Event<String>>) e.getClass(), s);
//		messageB.sendEvent(e);
//		Message m = messageB.awaitMessage(s);
//		assertNotNull(m);
//	}
//
//	@Test
//	public void testunregister() throws InterruptedException {
//		// TODO: change this test and add more tests :)
//		Subscriber s = new Moneypenny("samir",0);
//		messageB.register(s);
//		Event<String> e = new GadgetAvailableEvent();
//		messageB.subscribeEvent((Class<? extends Event<String>>) e.getClass(), s);
//		messageB.sendEvent(e);
//		messageB.unregister(s);
//		Message m = null;
//		try {
//			m = messageB.awaitMessage(s);
//		} catch (InterruptedException e1) {
//			assertFalse(false);
//		}
//	}
//
//	@Test
//	public void testcomplete() {
//		// TODO: change this test and add more tests :)
//		Event<String> event = new AgentsAvailableEvent();
//		String result = "completed";
//		messageB.complete(event, result);
//		assertTrue(event.isCompleted());
//	}
//
//	@Test
//	public void testsendEvent() {
//		// TODO: change this test and add more tests :)
//		Subscriber s = new Moneypenny("cocojumpo");
//		Event<String> e = new GadgetAvailableEvent();
//		Future f = messageB.sendEvent(e);
//		assertNull(f);
//		messageB.subscribeEvent((Class<? extends Event<String>>) e.getClass(), s);
//		f = messageB.sendEvent(e);
//		assertNotNull(f);
//	}
//
//	@Test
//	public void testawaitMessage() {
//		// TODO: change this test and add more tests :)
//		Subscriber s = new Moneypenny("cocojumpo");
//		Event<String> e = new GadgetAvailableEvent();
//		messageB.subscribeEvent((Class<? extends Event<String>>) e.getClass(), s);
//		Message m = null;
//		try {
//			m = messageB.awaitMessage(s);
//		} catch (InterruptedException e1) {
//			assertFalse(true);
//		}
//		assertNotNull(m);
//	}
//
//	@Test
//	public void testsendBroadcast() {
//		// TODO: change this test and add more tests :)
//		Subscriber s = new Moneypenny("elisa");
//		Subscriber s2 = new Moneypenny("nancy");
//		Broadcast e = new GadgetAvailableEvent();
//		messageB.subscribeBroadcast((Class<? extends Broadcast>) e.getClass(), s);
//		messageB.subscribeBroadcast((Class<? extends Broadcast>) e.getClass(), s2);
//		messageB.sendBroadcast(e);
//		Message m = null;
//		try {
//			m = messageB.awaitMessage(s);
//		} catch (InterruptedException e1) {
//			assertFalse(true);
//		}
//		assertNotNull(m);
//		try {
//			m = messageB.awaitMessage(s2);
//		} catch (InterruptedException e1) {
//			assertFalse(true);
//		}
//		assertNotNull(m);
//	}
//
//	@Test
//	public void testsubscribeBroadcast() {
//		// TODO: change this test and add more tests :)
//		Subscriber s = new Moneypenny("nana");
//		Broadcast e = new GadgetAvailableEvent();
//		messageB.subscribeBroadcast((Class<? extends Broadcast>) e.getClass(), s);
//		messageB.sendBroadcast(e);
//		Message m = null;
//		try {
//			m = messageB.awaitMessage(s);
//		} catch (InterruptedException e1) {
//			assertFalse(true);
//		}
//		assertNotNull(m);
//
//	}
//}
