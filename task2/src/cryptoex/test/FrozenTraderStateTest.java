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
public class FrozenTraderStateTest {
    
    Crypto crypto = new Crypto('c',10, 1010001111);
    double price = 10;
    double quantity = 10;
    Trader trader = new Trader("", "", "");
    FrozenTraderState instance = new FrozenTraderState(trader);
    
    /**
     * Test of makeLimitBuyOrder method, of class FrozenTraderState.
     */
    @Test
    public void testMakeLimitBuyOrder() {
        boolean expResult = false;
        boolean result = instance.makeLimitBuyOrder(crypto, price, quantity);
        assertEquals(expResult, result);
    }

    /**
     * Test of makeMarketBuyOrder method, of class FrozenTraderState.
     */
    @Test
    public void testMakeMarketBuyOrder() {
        boolean expResult = false;
        boolean result = instance.makeMarketBuyOrder(crypto, quantity);
        assertEquals(expResult, result);
    }

    /**
     * Test of makeLimitSellOrder method, of class FrozenTraderState.
     */
    @Test
    public void testMakeLimitSellOrder() {
        boolean expResult = false;
        boolean result = instance.makeLimitSellOrder(crypto, price, quantity);
        assertEquals(expResult, result);
    }

    /**
     * Test of makeMarketSellOrder method, of class FrozenTraderState.
     */
    @Test
    public void testMakeMarketSellOrder() {
        boolean expResult = false;
        boolean result = instance.makeMarketSellOrder(crypto, quantity);
        assertEquals(expResult, result);
    }
    
}
