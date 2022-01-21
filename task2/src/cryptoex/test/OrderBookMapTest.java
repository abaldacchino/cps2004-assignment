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
public class OrderBookMapTest {

    /**
     * Test of getInstance method, of class OrderBookMap.
     */
    @Test
    public void testGetInstance() {
        OrderBookMap orderBookMap = OrderBookMap.getInstance();
        assertTrue(orderBookMap instanceof OrderBookMap);
    }

    /**
     * Test of getOrderBook method, of class OrderBookMap.
     */
    @Test
    public void testGetOrderBook() {
        Crypto crypto = new Crypto('a', 4, 100001);
        OrderBookMap orderBookMap = OrderBookMap.getInstance();
        OrderBook orderBook = orderBookMap.getOrderBook(crypto);
        assertEquals(crypto, orderBook.getCryptoType());
        
        OrderBook sameOrderBook = orderBookMap.getOrderBook(crypto);
        assertEquals(orderBook, sameOrderBook);
    }
    
}
