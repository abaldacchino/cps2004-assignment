
package cryptoex.logic;


public class BuyOrder extends Order {

    public BuyOrder(Trader owner, Crypto type, double quantity) {
        super(owner, type, quantity);
    }

    @Override
    boolean makeOrder() {
    }

    @Override
    boolean cancelOrder() {
    }

    @Override
    boolean executable(double price, double quantity) {
    }

    @Override
    boolean executeOrder(double price, double quantity) {
    }
    
}
