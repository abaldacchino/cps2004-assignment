
package cryptoex.logic;


public abstract class TraderState {
    protected final Trader trader;
    
    public TraderState(Trader trader){
        this.trader = trader;
    }
    
    public abstract boolean makeLimitBuyOrder(Crypto crypto, double bidPrice, double quantity);
    
    public abstract boolean makeMarketBuyOrder(Crypto crypto, double quantity);
    
    public abstract boolean makeLimitSellOrder(Crypto crypto, double askPrice, double quantity);
    
    public abstract boolean makeMarketSellOrder(Crypto crypto, double quantity);
    
}
