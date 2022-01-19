
package cryptoex.logic;

/**
 * Implements Observer design pattern used in order to let Trader and OrderBook
 * know of any updates to an Order.
 */
public interface OrderObserver {
    void update(Order order);
}
