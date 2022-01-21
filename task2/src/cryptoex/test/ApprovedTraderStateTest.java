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
public class ApprovedTraderStateTest {
    
    Crypto crypto = new Crypto('a',10,100000000);
    Trader trader = new Trader("Name", "user", "pass");
    ApprovedTraderState approvedState = new ApprovedTraderState(trader);
    
    

    /**
     * Test of makeLimitBuyOrder method, of class ApprovedTraderState
     * for invalid bidPrice (zero).
     */
    @Test
    public void testMakeLimitBuyOrderBidPriceZero() {
        double bidPrice = 0.0;
        double quantity = 10;
        boolean expResult = false;
        boolean result = approvedState.makeLimitBuyOrder(crypto, bidPrice, quantity);
        assertEquals(expResult, result);
    }

    /**
     * Test of makeLimitBuyOrder method, of class ApprovedTraderState
     * for invalid bidPrice (negative).
     */
    @Test
    public void testMakeLimitBuyOrderBidPriceNegative() {
        double bidPrice = -20;
        double quantity = 10;
        boolean expResult = false;
        boolean result = approvedState.makeLimitBuyOrder(crypto, bidPrice, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeLimitBuyOrder method, of class ApprovedTraderState
     * for invalid quantity (zero).
     */
    @Test
    public void testMakeLimitBuyOrderQuantityZero() {
        double bidPrice = 3;
        double quantity = 0;
        boolean expResult = false;
        boolean result = approvedState.makeLimitBuyOrder(crypto, bidPrice, quantity);
        assertEquals(expResult, result);
    }

    /**
     * Test of makeLimitBuyOrder method, of class ApprovedTraderState
     * for invalid quantity (negative).
     */
    @Test
    public void testMakeLimitBuyOrderQuantityNegative() {
        double bidPrice = 3;
        double quantity = -10;
        boolean expResult = false;
        boolean result = approvedState.makeLimitBuyOrder(crypto, bidPrice, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeLimitBuyOrder method, of class ApprovedTraderState for
     * valid parameters.
     */
    @Test
    public void testMakeLimitBuyOrderValid() {
        double bidPrice = 3;
        double quantity = 10;
        boolean expResult = true;
        boolean result = approvedState.makeLimitBuyOrder(crypto, bidPrice, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeMarketBuyOrder method, of class ApprovedTraderState for 
     * invalid quantity (zero).
     */
    @Test
    public void testMakeMarketBuyOrderQuantityZero() {
        double quantity = 0.0;
        boolean expResult = false;
        boolean result = approvedState.makeMarketBuyOrder(crypto, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeMarketBuyOrder method, of class ApprovedTraderState for 
     * invalid quantity (negative).
     */
    @Test
    public void testMakeMarketBuyOrderQuantityNegative() {
        double quantity = -10;
        boolean expResult = false;
        boolean result = approvedState.makeMarketBuyOrder(crypto, quantity);
        assertEquals(expResult, result);
    }

    /**
     * Test of makeMarketBuyOrder method, of class ApprovedTraderState for 
     * valid parameters.
     */
    @Test
    public void testMakeMarketBuyOrderValid() {
        double quantity = 20.12;
        boolean expResult = true;
        boolean result = approvedState.makeMarketBuyOrder(crypto, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeLimitSellOrder method, of class ApprovedTraderState for
     * invalid askPrice (zero)
     */
    @Test
    public void testMakeLimitSellOrderAskPriceZero() {
        double askPrice = 0.0;
        double quantity = 123;
        boolean expResult = false;
        boolean result = approvedState.makeLimitSellOrder(crypto, askPrice, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeLimitSellOrder method, of class ApprovedTraderState for
     * invalid askPrice (negative)
     */
    @Test
    public void testMakeLimitSellOrderAskPriceNegative() {
        double askPrice = -10;
        double quantity = 123;
        boolean expResult = false;
        boolean result = approvedState.makeLimitSellOrder(crypto, askPrice, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeLimitSellOrder method, of class ApprovedTraderState for
     * invalid quantity (zero)
     */
    @Test
    public void testMakeLimitSellOrderQuantityZero() {
        double askPrice = 123;
        double quantity = 0;
        boolean expResult = false;
        boolean result = approvedState.makeLimitSellOrder(crypto, askPrice, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeLimitSellOrder method, of class ApprovedTraderState for
     * invalid quantity (negative)
     */
    @Test
    public void testMakeLimitSellOrderQuantityNegative() {
        double askPrice = 123;
        double quantity = -10;
        boolean expResult = false;
        boolean result = approvedState.makeLimitSellOrder(crypto, askPrice, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeLimitSellOrder method, of class ApprovedTraderState for
     * valid parameters.
     */
    @Test
    public void testMakeLimitSellOrderQuantityValid() {
        double askPrice = 123;
        double quantity = 123;
        boolean expResult = true;
        boolean result = approvedState.makeLimitSellOrder(crypto, askPrice, quantity);
        assertEquals(expResult, result);
    }



    /**
     * Test of makeMarketSellOrder method, of class ApprovedTraderState for
     * invalid quantity (zero).
     */
    @Test
    public void testMakeMarketSellOrderQuantityZero() {
        double quantity = 0.0;
        boolean expResult = false;
        boolean result = approvedState.makeMarketSellOrder(crypto, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeMarketSellOrder method, of class ApprovedTraderState for
     * invalid quantity (negative).
     */
    @Test
    public void testMakeMarketSellOrderQuantityNegative() {
        double quantity = -10;
        boolean expResult = false;
        boolean result = approvedState.makeMarketSellOrder(crypto, quantity);
        assertEquals(expResult, result);
    }
    
    /**
     * Test of makeMarketSellOrder method, of class ApprovedTraderState for
     * valid parameters.
     */
    @Test
    public void testMakeMarketSellOrderValid() {
        double quantity = 10;
        boolean expResult = true;
        boolean result = approvedState.makeMarketSellOrder(crypto, quantity);
        assertEquals(expResult, result);
    }

}
