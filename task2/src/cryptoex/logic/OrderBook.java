
package cryptoex.logic;

import java.util.ArrayList;
import java.util.SortedSet;
import java.util.TreeSet;


public class OrderBook implements OrderObserver{
    private final Crypto cryptoType;
    private double averagePrice=1;
    private double quantityBought=0;
    private double totalSpent=0;
    private long orderNo =0;
    private SortedSet<BuyOrder> buyOrders;
    private SortedSet<SellOrder> sellOrders;
    private ArrayList<Order> finishedOrders;
    private ArrayList<Order> cancelledOrders;
    
    public OrderBook(Crypto cryptoType){
        this.cryptoType = cryptoType;
        buyOrders = new TreeSet<>();
        sellOrders = new TreeSet<>();
        finishedOrders = new ArrayList<>();
        cancelledOrders = new ArrayList<>();
    }
    
    public boolean addBuyOrder(Trader trader, Crypto crypto, double quantity, 
            double bidPrice){
        BuyOrder order = new BuyOrder(trader, crypto, quantity, bidPrice, 
                ++orderNo);
        order.addObserver(this);
        buyOrders.add(order);
        //since order added, orders should be checked to see if there's a match
        fillOrders();  
        return true;
    }
    
    public boolean addSellOrder(Trader trader, Crypto crypto, double quantity, 
            double askPrice){
        SellOrder order = new SellOrder(trader, crypto, quantity, askPrice, 
                ++orderNo);
        order.addObserver(this);
        sellOrders.add(order);
        
        //since order added, orders should be checked to see if there's a match
        fillOrders();
        return true;
    }
    
    public boolean existsMatch(){
        if(sellOrders.isEmpty() || buyOrders.isEmpty()){
            return false;
        }else{
            //Case when buy is a market order (always matches)
            if(buyOrders.first().getPrice()==0)return true;
            return sellOrders.first().getPrice() 
                    <= buyOrders.first().getPrice();
        }
    }
    
    private void fillOrders(){
        while(existsMatch()){
            BuyOrder buyOrder = buyOrders.first();
            SellOrder sellOrder = sellOrders.first();
            
            double price = calcPrice(buyOrder, sellOrder);
            double volume = Math.min(buyOrder.getVolLeft(),
                    sellOrder.getVolLeft());
            
            double cost = price * volume;
            
            if(!buyOrder.getOwner().canAfford(cost)){
                buyOrder.cancelOrder();
            }else if(!sellOrder.getOwner().canAfford(cryptoType, volume)){
                sellOrder.cancelOrder();
            }else{
                //Deducting + adding funds
                buyOrder.getOwner().deductBalance(cost);
                sellOrder.getOwner().deductCrypto(cryptoType, volume);
                buyOrder.getOwner().addCrypto(cryptoType, volume);
                sellOrder.getOwner().addBalance(cost);
                //Filling orders
                buyOrder.fillOrder(volume);
                sellOrder.fillOrder(volume);
                //Finding the new average
                quantityBought+=volume;
                totalSpent+=cost;
                averagePrice = totalSpent/quantityBought;
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
        if(order.getStatus().equals("Cancelled")){
            if(order.getOrderType().equals("BUY")){
                buyOrders.remove(order);
            }else{
                sellOrders.remove(order);
            }
            cancelledOrders.add(order);
        }if(order.getStatus().equals("Finished")){
            if(order.getOrderType().equals("BUY")){
                buyOrders.remove(order);
            }else{
                sellOrders.remove(order);
            }
            finishedOrders.add(order);
        }
    }
    
    public Crypto getCryptoType() {
        return cryptoType;
    }

    public long getOrderNo() {
        return orderNo;
    }
    
    public double getAveragePrice() {
        return averagePrice;
    }

    public SortedSet<BuyOrder> getBuyOrders() {
        return buyOrders;
    }

    public SortedSet<SellOrder> getSellOrders() {
        return sellOrders;
    }

    public ArrayList<Order> getFinishedOrders() {
        return finishedOrders;
    }

    public ArrayList<Order> getCancelledOrders() {
        return cancelledOrders;
    }
}
