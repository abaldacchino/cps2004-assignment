package cryptoex.logic;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author balda
 */
public class SellOrderTest {
    Trader owner = new Trader("","","");
    Crypto type = new Crypto('c', 5, 10001);
    double quantity =50;
    SellOrder a;
    SellOrder b;
    
    /**
     * Test of compareTo method, of class SellOrder.
     * Case when a askPrice less than b askPrice
     */
    @Test
    public void testCompareToAskPrice() {
        a = new SellOrder(owner, type, quantity, 50, 4);
        b = new SellOrder(owner, type, quantity, 100, 5);
        
        assertTrue(a.compareTo(b) <0);
        assertTrue(b.compareTo(a) >0);
    }
    
    /**
     * Test of compareTo method, of class SellOrder.
     * Case when askPrice equal, goes on orderNo
     */
    @Test
    public void testCompareToOrderNo() {
        a = new SellOrder(owner, type, quantity, 50, 4);
        b = new SellOrder(owner, type, quantity, 50, 10);
        
        assertTrue(a.compareTo(b) <0);
        assertTrue(b.compareTo(a) >0);
    }
    
    /**
     * Test of compareTo method, of class SellOrder.
     * Case when both equal
     */
    @Test
    public void testCompareToEqual() {
        a = new SellOrder(owner, type, quantity, 50, 4);
        b = new SellOrder(owner, type, quantity, 50, 4);
        
        assertTrue(a.compareTo(b) ==0);
        assertTrue(b.compareTo(a) ==0);
    }
}
