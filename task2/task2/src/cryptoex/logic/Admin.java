
package cryptoex.logic;


public class Admin extends User {
    
    public Admin(String name, String username, String password){
        super(name, username, password);
    }
    
    /**
     * Method allows Admin to set state of Trader to ApprovedTraderState,
     * essentially allowing them to use the crypto exchange
     * 
     * @param trader Trader to approve
     */
    public void approveTrader(Trader trader){
        TraderState newState = new ApprovedTraderState(trader);
        trader.changeState(newState);
    }
    
    /**
     * Method allows Admin to set state of Trader to FrozenTraderState,
     * barring them from accessing the crypto exchange
     * 
     * @param trader Trader to approve
     */
    public void freezeTrader(Trader trader){
        TraderState newState = new FrozenTraderState(trader);
        trader.changeState(newState);
    }
}
