package cryptoex.test;

import cryptoex.logic.*;

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
    OrderBook ob = null;
    
    @Before
    public void setUp() {
        trader1 = new Trader("V", "vanvan", "123123");
        trader2 = new Trader("ES", "chidi", "123123");
        trader3 = new Trader("JC", "collier", "123123");
        crypto = new Crypto('a', 10, 1000000000000L);
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
     * Case when limit order is unmatched.
     */
    @Test
    public void testAddLimitBuyOrderUnmatched() {
        ob.addSellOrder(trader2, crypto, 30, 50);
        
        boolean result = ob.addBuyOrder(trader1, crypto, 50, 40);
        BuyOrder expectedOrder = new BuyOrder(trader1, crypto, 50, 40, ob.getOrderNo());
        assertTrue(result);
        assertTrue(ob.getBuyOrders().contains(expectedOrder));
    }
    
    /**
     * Test of addBuyOrder method, of class OrderBook.
     * Case when limit order is fully matched.
     */
    @Test
    public void testAddLimitBuyOrderMatched() {
        //Adding balance/crypto so order will be completed
        trader1.addBalance(20000);
        trader2.addCrypto(crypto, 200);
        trader3.addCrypto(crypto, 100);
        ob.addSellOrder(trader2, crypto, 30, 35);
        ob.addSellOrder(trader3, crypto, 100, 40);
        
        boolean result = ob.addBuyOrder(trader1, crypto, 50, 40);
        BuyOrder expectedOrder = new BuyOrder(trader1, crypto, 50, 40, 
                ob.getOrderNo());
        SellOrder sellCompleted = new SellOrder(trader2, crypto, 30, 35, 
                ob.getOrderNo()-2);
        assertTrue(result);
        
        assertTrue(ob.getFinishedOrders().contains(expectedOrder));
        assertTrue(ob.getFinishedOrders().contains(sellCompleted));
    }
    
    /**
     * Test of addBuyOrder method, of class OrderBook.
     * Case when limit order is unmatched.
     */
    @Test
    public void testAddMarketBuyOrderUnmatched() {
        boolean result = ob.addBuyOrder(trader1, crypto, 50, 0);
        BuyOrder expectedOrder = new BuyOrder(trader1, crypto, 50, 0, ob.getOrderNo());
        assertTrue(result);
        assertTrue(ob.getBuyOrders().contains(expectedOrder));
    }
    
    /**
     * Test of addBuyOrder method, of class OrderBook.
     * Case when limit order is fully matched.
     */
    @Test
    public void testAddMarketBuyOrderMatched() {
        //Adding balance/crypto so order will be completed
        trader1.addBalance(200000);
        trader2.addCrypto(crypto, 200);
        trader3.addCrypto(crypto, 100);
        ob.addSellOrder(trader2, crypto, 30, 35);
        ob.addSellOrder(trader3, crypto, 100, 40);
        
        
        boolean result = ob.addBuyOrder(trader1, crypto, 50, 0);
        BuyOrder expectedOrder = new BuyOrder(trader1, crypto, 50, 0, 
                ob.getOrderNo());
        SellOrder sellCompleted = new SellOrder(trader2, crypto, 30, 35, 
                ob.getOrderNo()-2);
        assertTrue(result);
        
        assertTrue(ob.getFinishedOrders().contains(expectedOrder));
        assertTrue(ob.getFinishedOrders().contains(sellCompleted));
    }
    
    /**
     * Test of addSellOrder method, of class OrderBook.
     * Case when limit order is matched.
     */
    @Test
    public void testAddLimitSellOrderMatched() {
        //Adding balance/crypto so order will be completed
        trader1.addBalance(20000);
        trader2.addCrypto(crypto, 200);
        trader3.addCrypto(crypto, 100);
        ob.addBuyOrder(trader1, crypto, 50, 40);
        ob.addBuyOrder(trader1, crypto, 10, 45);
        
        boolean result = ob.addSellOrder(trader2, crypto, 30, 40);
        SellOrder expectedOrder = new SellOrder(trader2, crypto, 30, 40, 
                ob.getOrderNo());
        BuyOrder buyCompleted = new BuyOrder(trader1, crypto, 10, 45,
                ob.getOrderNo()-1);
        
        assertTrue(result);
        assertTrue(ob.getFinishedOrders().contains(expectedOrder));
        assertTrue(ob.getFinishedOrders().contains(buyCompleted));
    }

    /**
     * Test of addSellOrder method, of class OrderBook.
     * Case when limit order is unmatched.
     */
    @Test
    public void testAddLimitSellOrderUnmatched() {
        ob.addBuyOrder(trader1, crypto, 100, 50);
                
        boolean result = ob.addSellOrder(trader2, crypto, 30, 40);
        SellOrder expectedOrder = new SellOrder(trader2, crypto, 30, 40, 
                ob.getOrderNo());
        
        
        assertTrue(ob.getSellOrders().contains(expectedOrder));
    }

    /**
     * Test of addSellOrder method, of class OrderBook.
     * Case when limit order is matched.
     */
    @Test
    public void testAddMarketSellOrderMatched() {
        //Adding balance/crypto so order will be completed
        trader1.addBalance(20000);
        trader2.addCrypto(crypto, 200);
        trader3.addCrypto(crypto, 100);
        ob.addBuyOrder(trader1, crypto, 50, 40);
        ob.addBuyOrder(trader1, crypto, 10, 45);
        
        boolean result = ob.addSellOrder(trader2, crypto, 30, 0);
        SellOrder expectedOrder = new SellOrder(trader2, crypto, 30, 0, 
                ob.getOrderNo());
        BuyOrder buyCompleted = new BuyOrder(trader1, crypto, 10, 45,
                ob.getOrderNo()-1);
        
        assertTrue(result);
        assertTrue(ob.getFinishedOrders().contains(expectedOrder));
        assertTrue(ob.getFinishedOrders().contains(buyCompleted));
    }

    /**
     * Test of addSellOrder method, of class OrderBook.
     * Case when limit order is unmatched.
     */
    @Test
    public void testAddMarketSellOrderUnmatched() {
        boolean result = ob.addSellOrder(trader2, crypto, 30, 0);
        SellOrder expectedOrder = new SellOrder(trader2, crypto, 30, 0, 
                ob.getOrderNo());
        assertTrue(result);
        assertTrue(ob.getSellOrders().contains(expectedOrder));
    }
    
    /**
     * Test of existsMatch method, of class OrderBook.
     * OrderList matches orders immediately, so existsMatch should always be
     * false - as if a match existed it would be already matched.
     */
    @Test
    public void testExistsMatch() {
        ob.addBuyOrder(trader1, crypto, 100, 50);
        ob.addBuyOrder(trader2, crypto, 100, 50);
        ob.addBuyOrder(trader2, crypto, 100, 0);
        
        ob.addSellOrder(trader3, crypto, 50, 49);
        ob.addSellOrder(trader3, crypto, 50, 51);
        
        assertFalse(ob.existsMatch());
    }

    /**
     * Test of calcPrice method, of class OrderBook.
     * Case when both are market orders
     */
    @Test
    public void testCalcPriceBothMarket() {
        BuyOrder buyOrder = new BuyOrder(trader1, crypto, 100, 0, 1);
        SellOrder sellOrder = new SellOrder(trader1, crypto, 100, 0, 1);
        
        double result = ob.calcPrice(buyOrder, sellOrder);
        assertEquals(ob.getAveragePrice(), result ,0.0);
    }
    
    /**
     * Test of calcPrice method, of class OrderBook.
     * Case when sell order is market.
     */
    @Test
    public void testCalcPriceSellMarket() {
        BuyOrder buyOrder = new BuyOrder(trader1, crypto, 100, 50, 1);
        SellOrder sellOrder = new SellOrder(trader1, crypto, 100, 0, 1);
        
        double result = ob.calcPrice(buyOrder, sellOrder);
        assertEquals(50.0, result ,0.0);
    }
    
    /**
     * Test of calcPrice method, of class OrderBook.
     * Case when buy order is market.
     */
    @Test
    public void testCalcPriceBuyMarket() {
        BuyOrder buyOrder = new BuyOrder(trader1, crypto, 100, 0, 1);
        SellOrder sellOrder = new SellOrder(trader1, crypto, 100, 50, 1);
        
        double result = ob.calcPrice(buyOrder, sellOrder);
        assertEquals(50.0, result ,0.0);
    }
    
    /**
     * Test of calcPrice method, of class OrderBook.
     * Case when both are limit.
     */
    @Test
    public void testCalcPriceBothLimit() {
        BuyOrder buyOrder = new BuyOrder(trader1, crypto, 100, 90, 1);
        SellOrder sellOrder = new SellOrder(trader1, crypto, 100, 50, 1);
        
        double result = ob.calcPrice(buyOrder, sellOrder);
        assertEquals(50.0, result ,0.0);
    }

    /**
     * Test of update method, of class OrderBook.
     */
    @Test
    public void testUpdate() {
        ob.addBuyOrder(trader1, crypto, 100, 0);
        BuyOrder order = ob.getBuyOrders().first();
        
        order.cancelOrder();    //update called in here
        assertTrue(ob.getCancelledOrders().contains(order));
        assertFalse(ob.getBuyOrders().contains(order));
    }
    
}
