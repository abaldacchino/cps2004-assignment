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
public class OrderTest {
    Trader trader = null;
    Crypto crypto = null; 
    double quantity = 50;
    double price = 20;
    long orderNo =5; 
    String orderType = "BUY";
    
    BuyOrder order = null;
    
    public OrderTest() {
    }
    
    @Before
    public void setUp() {
        trader = new Trader("name", "user", "pass");
        crypto = new Crypto('c', 5, 10000);
        order = new BuyOrder(trader, crypto, quantity, price, orderNo);
    }
    
    @After
    public void tearDown() {
    }

    /**
     * Test of getOwner method, of class Order.
     */
    @Test
    public void testGetOwner() {
        Trader result = order.getOwner();
        
        assertEquals(trader, result);
    }

    /**
     * Test of getType method, of class Order.
     */
    @Test
    public void testGetType() {
        Crypto result = order.getType();
        
        assertEquals(crypto, result);
    }

    /**
     * Test of getOrderNo method, of class Order.
     */
    @Test
    public void testGetOrderNo() {
        long result = order.getOrderNo();
        assertEquals(orderNo, result);
    }

    /**
     * Test of getPrice method, of class Order.
     */
    @Test
    public void testGetPrice() {
        double result = order.getPrice();
        assertEquals(price, result, 0.0);
    }

    /**
     * Test of getQuantity method, of class Order.
     */
    @Test
    public void testGetQuantity() {
        double result = order.getQuantity();
        assertEquals(quantity, result, 0.0);
    }

    /**
     * Test of getVolExecuted method, of class Order.
     */
    @Test
    public void testGetVolExecuted() {
        double result = order.getVolExecuted();
        assertEquals(0, result, 0.0);
    }

    /**
     * Test of getStatus method, of class Order.
     */
    @Test
    public void testGetStatus() {
        String result = order.getStatus();
        assertEquals("Pending", result);
    }

    /**
     * Test of getOrderType method, of class Order.
     * For subclass BuyOrder
     */
    @Test
    public void testGetOrderTypeBuy() {
        String result = order.getOrderType();
        assertEquals("BUY", result);
    }
    
    /**
     * Test of getOrderType method, of class Order.
     * For subclass SellOrder
     */
    @Test
    public void testGetOrderTypeSell(){
        SellOrder sorder = new SellOrder(trader, crypto, quantity, price, orderNo);
        String result = sorder.getOrderType();
        assertEquals("SELL", result);
    }
    /**
     * Test of addObserver method, of class Order.
     */
    @Test
    public void testAddObserver() {
        Trader other = new Trader("Bowie", "bw", "dv");
        
        order.addObserver(other);
        order.cancelOrder();
        //Checking if cancelOrder calls update in Order
        assertTrue(other.getCancelledOrders().contains(order));
    }

    /**
     * Test of getVolLeft method, of class Order.
     */
    @Test
    public void testGetVolLeft() {
        double result = order.getVolLeft();
        assertEquals(quantity, result, 0.0);
    }

    /**
     * Test of fillOrder method, of class Order.
     * Case when order is not completely filled.
     */
    @Test
    public void testFillOrderPartial() {
        double vol = 20;
        order.fillOrder(vol);
        assertEquals("Pending", order.getStatus());
        assertEquals(vol, order.getVolExecuted(), 0.0);
        assertEquals(quantity-vol, order.getVolLeft(), 0.0);
        assertTrue(trader.getPendingOrders().contains(order));
    }
    
    /**
     * Test of fillOrder method, of class Order.
     * Case when order is completely filled.
     */
    @Test
    public void testFillOrderFull() {
        double vol = quantity;
        order.fillOrder(vol);
        assertEquals("Finished", order.getStatus());
        assertEquals(vol, order.getVolExecuted(), 0.0);
        assertEquals(quantity-vol, order.getVolLeft(), 0.0);
        assertTrue(trader.getSuccessfulOrders().contains(order));
    }

    /**
     * Test of cancelOrder method, of class Order.
     * Case when order in completed state (cancel fails)
     */
    @Test
    public void testCancelCompletedOrder() {
        double vol = quantity;
        order.fillOrder(vol);
        assertEquals("Finished", order.getStatus());
        
        assertEquals(false, order.cancelOrder());
        assertFalse(trader.getCancelledOrders().contains(order));
    }
    /**
     * Test of cancelOrder method, of class Order.
     * Case when order in already cancelled state (cancel fails)
     */
    @Test
    public void testCancelCancelledOrder() {
        order.cancelOrder();
        
        assertEquals(false, order.cancelOrder());
    }
    /**
     * Test of cancelOrder method, of class Order.
     * Case when order in pending state (succeeds)
     */
    @Test
    public void testCancelPendingOrder() {
        assertEquals(true, order.cancelOrder());
        assertTrue(trader.getCancelledOrders().contains(order));
        assertEquals("Cancelled", order.getStatus());
    }
}
