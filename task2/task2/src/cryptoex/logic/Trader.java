
package cryptoex.logic;

import java.util.ArrayList;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Trader extends User implements OrderObserver{
    // Contains all pending orders
    private ArrayList<Order> pendingOrders = new ArrayList<>();
    // Contains all cancelled orders
    private ArrayList<Order> cancelledOrders = new ArrayList<>();
    // Contains all successful orders
    private ArrayList<Order> successfulOrders = new ArrayList<>();

    // Maps each Crypto to the amount of that the user owns
    protected final Map<Crypto, Double> ownedCrypto;
    
    //Stores state of Trader 
    private TraderState state;

    //Stores available balance of Trader
    private double balance =0;

    public Trader(String name, String username, String password){
        super(name, username, password);
        ownedCrypto = new ConcurrentHashMap<>();
        state = new PendingTraderState(this);   //user starts off in pending state
    }
    
    public boolean makeLimitBuyOrder(Crypto crypto, double bidPrice, double quantity){
        return state.makeLimitBuyOrder(crypto, bidPrice, quantity);
    }
    
    public boolean makeMarketBuyOrder(Crypto crypto, double quantity){
        return state.makeMarketBuyOrder(crypto, quantity);
    }
    
    public boolean makeLimitSellOrder(Crypto crypto, double askPrice, double quantity){
        return state.makeLimitSellOrder(crypto, askPrice, quantity);
    }
    
    public boolean makeMarketSellOrder(Crypto crypto, double quantity){
        return state.makeMarketSellOrder(crypto, quantity);
    }
    
    
    public void changeState(TraderState state){
        this.state= state;
    }
    
    public boolean canAfford(double cost){
        return balance >=cost;
    }
    
    public boolean canAfford(Crypto crypto, double quantity){
        return ownedCrypto.getOrDefault(crypto,0.0) >= quantity;
    }
    
    public void deductBalance(double cost){
        balance -=cost;
    }
    
    public void deductCrypto(Crypto crypto, double quantity){
        ownedCrypto.put(crypto, ownedCrypto.getOrDefault(crypto,0.0)- quantity);
    }
    
    public void addBalance(double cost){
        balance +=cost;
    }
    
    public void addCrypto(Crypto crypto, double quantity){
        ownedCrypto.put(crypto, ownedCrypto.getOrDefault(crypto,0.0)+ quantity);
    }

    @Override
    public void update(Order order) {
        if(order.getStatus().equals("Cancelled")){
            pendingOrders.remove(order);
            cancelledOrders.add(order);
        }else if(order.getStatus().equals("Finished")){
            pendingOrders.remove(order);
            successfulOrders.add(order);
        }else if(order.getStatus().equals("Pending")){
            pendingOrders.add(order);
        }
    }

    public boolean cancelOrder(Order order){
        //Can only cancel an order that's pending and belongs to Trader
        if(!pendingOrders.contains(order))return false;
        return order.cancelOrder();
    }
    
    public ArrayList<Order> getPendingOrders() {
        return pendingOrders;
    }

    public ArrayList<Order> getCancelledOrders() {
        return cancelledOrders;
    }

    public ArrayList<Order> getSuccessfulOrders() {
        return successfulOrders;
    }

    public Map<Crypto, Double> getOwnedCrypto() {
        return ownedCrypto;
    }

    public TraderState getState() {
        return state;
    }

    public double getBalance() {
        return balance;
    }
    
}
