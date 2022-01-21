package cryptoex.test;

import cryptoex.logic.*;

import java.util.ArrayList;
import java.util.Map;
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
public class TraderTest {
    Trader trader = null;
    Crypto crypto = new Crypto('c',4,10000);
    
    @Before
    public void setUp() {
        trader = new Trader("name", "user", "pass");
    }
    
    /**
     * Test of canAfford method, of class Trader.
     * Case where cost = balance
     */
    @Test
    public void testCanAfford_doubleEqual() {
        double cost = 20;
        trader.addBalance(cost);
        
        assertTrue(trader.canAfford(cost));
    }
    
    /**
     * Test of canAfford method, of class Trader.
     * Case where cost less than balance
     */
    @Test
    public void testCanAfford_doubleLess() {
        double cost = 20;
        trader.addBalance(40);
        
        assertTrue(trader.canAfford(cost));
    }
    
    /**
     * Test of canAfford method, of class Trader.
     * Case where cost more than balance
     */
    @Test
    public void testCanAfford_doubleMore() {
        double cost = 20;
        
        assertFalse(trader.canAfford(cost));
    }

    /**
     * Test of canAfford method, of class Trader.
     * Case when quantity equal to owned quantity
     */
    @Test
    public void testCanAfford_Crypto_doubleEqual() {
        double quantity = 20.0;
        trader.addCrypto(crypto, 20);
        assertTrue(trader.canAfford(crypto, quantity));
    }
    
    /**
     * Test of canAfford method, of class Trader.
     * Case when quantity less than owned quantity
     */
    @Test
    public void testCanAfford_Crypto_doubleLess() {
        double quantity = 10.0;
        trader.addCrypto(crypto, 20);
        assertTrue(trader.canAfford(crypto, quantity));
    }

    /**
     * Test of canAfford method, of class Trader.
     * Case when quantity more than owned quantity
     */
    @Test
    public void testCanAfford_Crypto_doubleMore() {
        double quantity = 50.0;
        trader.addCrypto(crypto, 20);
        assertFalse(trader.canAfford(crypto, quantity));
    }
    
    /**
     * Test of canAfford method, of class Trader.
     * Case when user owns none of the crypto
     */
    @Test
    public void testCanAfford_Crypto_doubleNone() {
        double quantity = 50.0;
        assertFalse(trader.canAfford(crypto, quantity));
    }
    
    /**
     * Test of deductBalance method, of class Trader.
     */
    @Test
    public void testDeductBalance() {
        double cost = 20.0;
        trader.addBalance(40);
          
        trader.deductBalance(cost);
        assertEquals(20.0, trader.getBalance(),0.0);
    }

    /**
     * Test of deductCrypto method, of class Trader.
     */
    @Test
    public void testDeductCrypto() {
        double quantity = 20.0;
        trader.addCrypto(crypto, 40);
          
        trader.deductCrypto(crypto, quantity);
        assertEquals(20.0, trader.getOwnedCrypto().get(crypto),0.0);
    }

    /**
     * Test of addBalance method, of class Trader.
     */
    @Test
    public void testAddBalance() {
        trader.addBalance(40);
          
        assertEquals(40.0, trader.getBalance(),0.0);
    }

    /**
     * Test of addCrypto method, of class Trader.
     */
    @Test
    public void testAddCrypto() {
        trader.addCrypto(crypto, 40);
          
        assertEquals(40.0, trader.getOwnedCrypto().get(crypto),0.0);
    }

    /**
     * Test of update method, of class Trader.
     */
    @Test
    public void testUpdate() {
        Trader placeholder = new Trader("", "", "");
        BuyOrder order = new BuyOrder(placeholder, crypto, 20, 20, 1);
        
        //Update places order in correct ArrayList in trader
        trader.update(order);
        assertTrue(trader.getPendingOrders().contains(order));
    }

    /**
     * Test of cancelOrder method, of class Trader.
     * Case when order is not owned by trader calling cancel.
     */
    @Test
    public void testCancelOrderNotOwned() {
        Trader placeholder = new Trader("", "", "");
        BuyOrder order = new BuyOrder(placeholder, crypto, 20, 20, 1);
        
        //Order does not belong to trader, should not be deleted
        assertFalse(trader.cancelOrder(order));
        assertEquals("Pending", order.getStatus());
    }
    
    /**
     * Test of cancelOrder method, of class Trader.
     * Case when order is owned by trader calling cancel.
     */
    @Test
    public void testCancelOrderOwned() {
        BuyOrder order = new BuyOrder(trader, crypto, 20, 20, 1);
        
        //Order does not belong to trader, should not be deleted
        assertTrue(trader.cancelOrder(order));
        assertTrue(trader.getCancelledOrders().contains(order));
        assertEquals("Cancelled", order.getStatus());
    }
}
