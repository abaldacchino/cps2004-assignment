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
public class OrderBookTest {
    Trader trader1 = null;
    Trader trader2 = null;
    Trader trader3 = null;
    Crypto crypto = null;
    BuyOrder buy1 = null;
    BuyOrder buy2 = null;
    BuyOrder buy3 = null;
    SellOrder sell1 = null;
    SellOrder sell2 = null;
    SellOrder sell3 = null;
    SellOrder sell4 = null;
    OrderBook ob = null;
    
    @Before
    public void setUp() {
        trader1 = new Trader("V", "vanvan", "123123");
        trader2 = new Trader("ES", "chidi", "123123");
        trader3 = new Trader("JC", "collier", "123123");
        crypto = new Crypto('a', 10, 1000000000000L);
        
        BuyOrder buy1 = new BuyOrder(trader1, crypto, 100, 70, 101);
        buy2 = new BuyOrder(trader2, crypto, 90, 88, 104);
        buy3 = new BuyOrder(trader2, crypto, 50, 80, 110);
        sell1 = new SellOrder(trader3, crypto, 10, 99, 99);
        sell2 = new SellOrder(trader2, crypto, 110, 100, 100);
        sell3 = new SellOrder(trader1, crypto, 80, 95, 103);
        sell4 = new SellOrder(trader1, crypto, 1, 99, 105);
        ob = new OrderBook(crypto);
    }
    

    /**
     * Test of getCryptoType method, of class OrderBook.
     */
    @Test
    public void testGetCryptoType() {
        Crypto result = ob.getCryptoType();
        assertEquals(crypto, result);
    }

    /**
     * Test of addBuyOrder method, of class OrderBook.
     */
    @Test
    public void testAddBuyOrder() {
        System.out.println("addBuyOrder");
        Trader trader = null;
        Crypto crypto = null;
        double quantity = 0.0;
        double bidPrice = 0.0;
        OrderBook instance = null;
        boolean expResult = false;
        boolean result = instance.addBuyOrder(trader, crypto, quantity, bidPrice);
        assertEquals(expResult, result);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of addSellOrder method, of class OrderBook.
     */
    @Test
    public void testAddSellOrder() {
        System.out.println("addSellOrder");
        Trader trader = null;
        Crypto crypto = null;
        double quantity = 0.0;
        double askPrice = 0.0;
        OrderBook instance = null;
        boolean expResult = false;
        boolean result = instance.addSellOrder(trader, crypto, quantity, askPrice);
        assertEquals(expResult, result);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of existsMatch method, of class OrderBook.
     */
    @Test
    public void testExistsMatch() {
        System.out.println("existsMatch");
        OrderBook instance = null;
        boolean expResult = false;
        boolean result = instance.existsMatch();
        assertEquals(expResult, result);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of fillOrders method, of class OrderBook.
     */
    @Test
    public void testFillOrders() {
        System.out.println("fillOrders");
        OrderBook instance = null;
        instance.fillOrders();
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of calcPrice method, of class OrderBook.
     */
    @Test
    public void testCalcPrice() {
        System.out.println("calcPrice");
        BuyOrder buyOrder = null;
        SellOrder sellOrder = null;
        OrderBook instance = null;
        double expResult = 0.0;
        double result = instance.calcPrice(buyOrder, sellOrder);
        assertEquals(expResult, result, 0.0);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of update method, of class OrderBook.
     */
    @Test
    public void testUpdate() {
        System.out.println("update");
        Order order = null;
        OrderBook instance = null;
        instance.update(order);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
}
