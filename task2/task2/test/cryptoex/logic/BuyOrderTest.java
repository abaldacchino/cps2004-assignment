
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
public class BuyOrderTest {
    Trader owner = new Trader("","","");
    Crypto type = new Crypto('c', 5, 10001);
    double quantity =50;
    BuyOrder a;
    BuyOrder b;
    
    /**
     * Test of compareTo method, of class BuyOrder.
     * Case when a bidPrice less than b bidPrice
     */
    @Test
    public void testCompareToBidPrice() {
        a = new BuyOrder(owner, type, quantity, 50, 4);
        b = new BuyOrder(owner, type, quantity, 100, 5);
        
        assertTrue(a.compareTo(b) <0);
        assertTrue(b.compareTo(a) >0);
    }
    
    /**
     * Test of compareTo method, of class BuyOrder.
     * Case when bidPrice equal, goes on orderNo
     */
    @Test
    public void testCompareToOrderNo() {
        a = new BuyOrder(owner, type, quantity, 50, 4);
        b = new BuyOrder(owner, type, quantity, 50, 5);
        
        assertTrue(a.compareTo(b) >0);
        assertTrue(b.compareTo(a) <0);
    }
    
    /**
     * Test of compareTo method, of class BuyOrder.
     * Case when both equal
     */
    @Test
    public void testCompareToEqual() {
        a = new BuyOrder(owner, type, quantity, 50, 5);
        b = new BuyOrder(owner, type, quantity, 50, 5);
        
        assertTrue(a.compareTo(b) ==0);
        assertTrue(b.compareTo(a) ==0);
    }
}
