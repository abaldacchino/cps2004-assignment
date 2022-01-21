
package cryptoex.logic;

import java.util.ArrayList;


public abstract class Order{
    private Trader owner;       //trader who made the order
    private Crypto type;        //type of Crypto of the order
    private long orderNo;       //order number (used to rank priority)

    protected double price;     //Stores bid/ask price
    private double quantity;     //Quantity of crypto requested/offered
    private double volExecuted=0;     //Quantity of crypto filled
    private String status;
    private String orderType;
    
    private final ArrayList<OrderObserver> observers;
    
    public Order(Trader owner, Crypto type, double quantity, double price, 
            long orderNo, String orderType){
        
        this.owner = owner;
        this.type = type;
        this.quantity = quantity;
        this.orderNo = orderNo;
        this.price = price;
        this.status = "Pending";
        this.orderType = orderType;
        
        observers = new ArrayList<>();
        observers.add(owner);
        notifyObservers();
    }
    
    private void notifyObservers(){
        for(OrderObserver obs : observers){
            obs.update(this);
        }
    }
    
    
    public Trader getOwner() {
        return owner;
    }

    public Crypto getType() {
        return type;
    }

    public long getOrderNo() {
        return orderNo;
    }

    public double getPrice() {
        return price;
    }

    public double getQuantity() {
        return quantity;
    }

    public double getVolExecuted() {
        return volExecuted;
    }

    public String getStatus() {
        return status;
    }
    
    public String getOrderType() {
        return orderType;
    }
    
    public void addObserver(OrderObserver o) {
        observers.add(o);
    }
    
    public double getVolLeft(){
        return quantity - volExecuted;
    }
    
    public void fillOrder(double vol) {
        this.volExecuted += vol;
        if(volExecuted ==quantity){
            setStatus("Finished");
        }
    }
    
    public boolean cancelOrder(){
        if(status.equals("Finished") || status.equals("Cancelled"))return false;
        setStatus("Cancelled");
        return true;
    }

    private void setStatus(String status) {
        this.status = status;
        notifyObservers();
    }
    
    @Override
    public boolean equals(Object o){
        if(o == this) return true;
        if(!(o instanceof Order))return false;
        Order order = (Order)o;
        return orderNo == order.getOrderNo();
    }
}
