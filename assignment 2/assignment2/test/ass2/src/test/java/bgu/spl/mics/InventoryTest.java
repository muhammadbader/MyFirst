package bgu.spl.mics;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.fail;

public class InventoryTest {
    private Inventory inventory;
    @BeforeEach
    public void setUp(){
        inventory=new Inventory;
    }
    @AfterEach
    public void teardown() throws Exception{}


    @Test
    public void testgetInstance(){
        //TODO: change this test and add more tests :)
        Inventory type1=inventory.getInstance();
        Inventory type2=inventory.getInstance();
        assertEquals(type1,type2);
    }

    @Test
    public void test(){
        //TODO: change this test and add more tests :)
        fail("Not a good test");
    }
}
