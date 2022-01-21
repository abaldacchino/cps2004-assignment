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
public class AdminTest {
    Admin admin = null;
    Trader trader = null;
    
    @Before
    public void setUp() {
        admin = new Admin("Rosemary Sage", "usern", "pass");
        trader = new Trader("Name", "user", "pass");
    }
    
    /**
     * Test of approveTrader method, of class Admin.
     */
    @Test
    public void testApproveTrader() {
        admin.approveTrader(trader);
        assertTrue(trader.getState() instanceof ApprovedTraderState);
    }

    /**
     * Test of freezeTrader method, of class Admin for trader previously
     * approved.
     */
    @Test
    public void testFreezeApprovedTrader() {
        admin.approveTrader(trader);
        admin.freezeTrader(trader);
        assertTrue(trader.getState() instanceof FrozenTraderState);
    }
    
    /**
     * Test of freezeTrader method, of class Admin for trader not previously
     * approved.
     */
    @Test
    public void testFreezePendingTrader() {
        admin.freezeTrader(trader);
        assertTrue(trader.getState() instanceof FrozenTraderState);
    }
    
}
