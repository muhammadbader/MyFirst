//package bgu.spl.mics;
//
//import org.junit.jupiter.api.BeforeEach;
//import org.junit.jupiter.api.Test;
//
//import static org.junit.Assert.assertTrue;
//import static org.junit.jupiter.api.Assertions.*;
//
//import java.util.concurrent.TimeUnit;
//
//import org.junit.jupiter.api.AfterEach;
//
//public class FutureTest {
//	private Future<String> future;
//
//    @BeforeEach
//    public void setUp(){
//    	future=new Future<String>();
//    }
//
//    @AfterEach
//    public void tearDown(){
//
//    }
//
//    @Test
//    public void testget() {
//        //TODO: change this test and add more tests :)
//        String s="kkk";
//
//        future.get();
//        assertFalse(future.isDone());
//        future.resolve(s);
//        String k = null;
//        k = future.get();
//        assertTrue(future.isDone());
//        assertEquals(k,s);
//    }
//
//    @Test
//    public void testresolve(){
//        //TODO: change this test and add more tests :)
//        String s = "okkk";
//        assertFalse(future.isDone());
//        future.resolve(s);
//        assertTrue(future.isDone());
//        String k = null;
//        k = future.get();
//        assertTrue(future.isDone());
//        assertEquals(k,s);
//    }
//
//    @Test
//    public void testisDone(){
//        //TODO: change this test and add more tests :)
//        assertEquals(false,future.isDone());
//        String s = "okkk";
//        future.resolve(s);
//        assertEquals(true,future.isDone());
//    }
//
//    @Test
//    public void testget2(){
//        //TODO: change this test and add more tests :)
//        //get(long timeout, TimeUnit unit)
//    	TimeUnit unit=TimeUnit.SECONDS;
//    	long timeout =5;
//    	String s=null;
//    	try {
//			s=future.get(timeout,unit);
//		} catch (InterruptedException e) {
//			// TODO Auto-generated catch block
//		}
//    	assertEquals(null,s);
//
//    	long time = System.currentTimeMillis();
//		while (!future.isDone()) {
//			future.resolve("mmamsf");
//		}
//		assertTrue(System.currentTimeMillis() - time <= unit.toMillis(timeout));
//
//    }
//
//}