
package cryptoex.logic;

import java.util.ArrayList;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Trader extends User{
    // Contains all orders made by a user
    ArrayList<Order> orders = new ArrayList<>();
    // Maps each Crypto to the amount of that the user owns
    private final Map<Crypto, Double> owned_crypto;
    
    boolean authorised;
    
    public Trader(String name, String username, String password){
        super(name, username, password);
        owned_crypto = new ConcurrentHashMap<>();
    }
    
    public boolean makeBuyOrder(Crypto crypto, double bidPrice){
        return true;
    }
    
    public boolean makeBuyOrder(Crypto crypto){
        return true;
    }
    
    public boolean makeSellOrder(Crypto crypto, double askPrice){
        return true;
    }
    
    public boolean makeSellOrder(Crypto crypto){
        return true;
    }
}
