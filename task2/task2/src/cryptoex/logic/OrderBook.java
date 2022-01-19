
package cryptoex.logic;

import java.util.NoSuchElementException;
import java.util.SortedSet;
import java.util.TreeSet;


public class OrderBook implements OrderObserver{
    private final Crypto cryptoType;
    private double averagePrice=1;
    private double quantityBought=0;
    private long orderNo =0;
    private SortedSet<BuyOrder> buyOrders;
    private SortedSet<SellOrder> sellOrders;
    
    public OrderBook(Crypto cryptoType){
        this.cryptoType = cryptoType;
        buyOrders = new TreeSet<>();
        sellOrders = new TreeSet<>();
    }
    
    public Crypto getCryptoType() {
        return cryptoType;
    }
    
    public boolean addBuyOrder(Trader trader, Crypto crypto, double quantity, double bidPrice){
        BuyOrder order = new BuyOrder(trader, crypto, quantity, bidPrice, ++orderNo);
        buyOrders.add(order);
        order.addObserver(this);
        
        //since order added, orders should be checked to see if there's a match
        fillOrders();  
        return true;
    }
    
    public boolean addSellOrder(Trader trader, Crypto crypto, double quantity, double askPrice){
        SellOrder order = new SellOrder(trader, crypto, quantity, askPrice, ++orderNo);
        sellOrders.add(order);
        
        order.addObserver(this);
        
        //since order added, orders should be checked to see if there's a match
        fillOrders();
        return true;
    }
    
    public boolean existsMatch(){
        try{
            return sellOrders.first().getPrice() <= buyOrders.first().getPrice();
        }catch(NoSuchElementException e){
            return false;
        }
    }
    
    public void fillOrders(){
        while(existsMatch()){
            BuyOrder buyOrder = buyOrders.first();
            SellOrder sellOrder = sellOrders.first();
            
            double price = calcPrice(buyOrder, sellOrder);
            double volume = Math.max(buyOrder.getVolLeft(),
                    sellOrder.getVolLeft());
            
            double cost = price * volume;
            
            if(!buyOrder.getOwner().canAfford(cost)){
                buyOrder.cancelOrder();
            }else if(!sellOrder.getOwner().canAfford(cryptoType, volume)){
                sellOrder.cancelOrder();
            }else{
                
            }
        }
    }
    
    /**
     * Calculates compromise price between BuyOrder and SellOrder.
     * If both are market orders, crypto is sold at average price.
     * If one is a market order, return ask/bid of the other order
     * If both are limit orders, favour the buy order by returning the
     * ask price (which is the smaller value).
     *
     * @param buyOrder  BuyOrder being matched with SellOrder
     * @param sellOrder SellOrder being matched to BuyOrder
     * @return
     */
    public double calcPrice(BuyOrder buyOrder, SellOrder sellOrder){
        if(buyOrder.getPrice()==0 && sellOrder.getPrice()==0){
            return averagePrice;
        }else if(sellOrder.getPrice()==0){
            return buyOrder.getPrice();
        }else 
            return sellOrder.getPrice();
    }
    
    @Override
    public void update(Order order) {
        if(order.getStatus().equals("Cancelled") || 
                order.getStatus().equals("Finished")){
            
            if(order.getOrderType().equals("BUY")){
                buyOrders.remove(order);
            }else{
                sellOrders.remove(order);
            }
        }
    }
}
