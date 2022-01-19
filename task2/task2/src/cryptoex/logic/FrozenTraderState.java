package cryptoex.logic;


public class FrozenTraderState extends TraderState{

    public FrozenTraderState(Trader trader){
        super(trader);
    }
    
    @Override
    public boolean makeLimitBuyOrder(Crypto crypto, double bidPrice, double quantity) {
        return false;
    }

    @Override
    public boolean makeMarketBuyOrder(Crypto crypto, double quantity) {
        return false;
    }

    @Override
    public boolean makeLimitSellOrder(Crypto crypto, double askPrice, double quantity) {
        return false;
    }

    @Override
    public boolean makeMarketSellOrder(Crypto crypto, double quantity) {
        return false;
    }
    
}
