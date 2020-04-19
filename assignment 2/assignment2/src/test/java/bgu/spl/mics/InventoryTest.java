//package bgu.spl.mics;
//
//import bgu.spl.mics.application.passiveObjects.Inventory;
//import org.junit.After;
//import org.junit.Before;
//import org.junit.jupiter.api.AfterEach;
//import org.junit.jupiter.api.BeforeEach;
//import org.junit.jupiter.api.Test;
//import static org.junit.Assert.*;
//
//public class InventoryTest {
//    private Inventory inventory;
//    @BeforeEach
//    public void setUp(){
//        inventory=new Inventory();
//    }
//    @AfterEach
//    public void teardown() throws Exception{}
//
//
//    @Test
//    public void testgetInstance(){
//        //TODO: change this test and add more tests :)
//        Inventory type1=inventory.getInstance();
//        Inventory type2=inventory.getInstance();
//        assertEquals(type1,type2);
//    }
//
//    @Test
//    public void testload(){
//        //TODO: change this test and add more tests :)
//        assertTrue("the Inventory is Empty",inventory.isEmpty());
//        String[] things={"knife","pistol","shotgun"};
//        inventory.load(things);
//        assertFalse(inventory.isEmpty());
//        for(int i=0;i<things.length;i++){
//            assertTrue(things[i]+" is there",inventory.getItem(things[i]));
//        }
//    }
//
//    @Test
//    public void testisEmpty(){
//        //TODO: change this test and add more tests :)
//        String[] s1 = {"knife"};
//        assertEquals(true, inventory.isEmpty());
//        inventory.load(s1);
//        assertEquals(false, inventory.isEmpty());
//
//    }
//    public void testigetItem(){
//        //TODO: change this test and add more tests :)
//        String[] s1 = {"knife","pistol","shotgun"};
//        assertEquals(true, inventory.isEmpty());
//        inventory.load(s1);
//        for(int i=0;i<s1.length;i++){
//            assertTrue(s1[i]+" is there",inventory.getItem(s1[i]));
//        }
//    }
//
//
//}
