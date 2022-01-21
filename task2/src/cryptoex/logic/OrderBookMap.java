
package cryptoex.logic;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * OrderBookMap keeps track of all OrderBooks using a ConcurrentHashMap
 * Singleton pattern is used as OrderBookMap should only have one instance.
 * It is assumed that there is only one OrderBook for each Crypto.
 * 
 * @author balda
 */
public final class OrderBookMap {
    private static final OrderBookMap instance = new OrderBookMap();
    
    /**
     * Method returns static instance of OrderBookMap
     * 
     * @return      Instance of OrderBookMap
     */
    public static OrderBookMap getInstance(){
        return instance;
    }
    
    private final Map<Crypto, OrderBook> orderBookMap;
    
    private OrderBookMap(){
        orderBookMap = new ConcurrentHashMap<>();
    }
    
    /**
     * Returns the instance of OrderBook for the crypto specified in parameters.
     * If OrderBook already exists in orderBookMap, the corresponding OrderBook
     * is returned. 
     * If it does not exist, it is created and stored in orderBookMap.
     * 
     * @param crypto Crypto object of OrderBook instance to be retrieved
     * @return       OrderBook with corresponding cryptoType value
     */

    public OrderBook getOrderBook(Crypto crypto){
        OrderBook retrieved = orderBookMap.get(crypto);
        
        if(retrieved==null){
            OrderBook created = new OrderBook(crypto);
            orderBookMap.put(crypto, created);
            return created;
        }else return retrieved;
    }
    
}
