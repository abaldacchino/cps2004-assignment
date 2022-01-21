

package cryptoex.logic;

public class ApprovedTraderState extends TraderState{

    public ApprovedTraderState(Trader trader){
        super(trader);
    }
    
    @Override
    public boolean makeLimitBuyOrder(Crypto crypto, double bidPrice, double quantity) {
        if(bidPrice <=0 || quantity<=0)return false;
        return getOrderBook(crypto).addBuyOrder(trader, crypto, quantity, bidPrice);
    }

    @Override
    public boolean makeMarketBuyOrder(Crypto crypto, double quantity) {
        if(quantity<=0)return false;
        return getOrderBook(crypto).addBuyOrder(trader, crypto, quantity, 0);
    }

    @Override
    public boolean makeLimitSellOrder(Crypto crypto, double askPrice, double quantity) {
        if(askPrice <=0 || quantity<=0)return false;
        return getOrderBook(crypto).addSellOrder(trader, crypto, quantity, askPrice);
    }

    @Override
    public boolean makeMarketSellOrder(Crypto crypto, double quantity) {
        if(quantity<=0)return false;
        
        return getOrderBook(crypto).addSellOrder(trader, crypto, quantity, 0);
    }
    
    /**
     * Method returns instance of OrderBook with matching value of cryptoType
     *
     * @param crypto Crypto corresponding to the type of desired OrderBook
     * @return       OrderBook with cryptoType crypto
     */
    private OrderBook getOrderBook(Crypto crypto){
        OrderBookMap orderBookMap = OrderBookMap.getInstance();
        return orderBookMap.getOrderBook(crypto);
    }
}
