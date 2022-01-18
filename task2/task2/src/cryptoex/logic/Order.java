
package cryptoex.logic;


public abstract class Order implements Comparable<Order>{
    static long orderCount =0;  //maintains how many orders have been made
    
    private Trader owner;       //trader who made the order
    private Crypto type;        //type of Crypto of the order
    private long orderNo;       //order number (used to rank priority)
    
    private double quantity;     //Quantity of crypto requested/offered
    private double volExecuted=0;     //Quantity of crypto filled
    
    public Order(Trader owner, Crypto type, double quantity){
        this.owner = owner;
        this.type = type;
        this.quantity = quantity;
        this.orderNo = ++orderCount;    //increments orderCount
    }
    
    abstract boolean makeOrder();
    abstract boolean cancelOrder();
    abstract boolean executable(double price, double quantity);
    abstract boolean executeOrder(double price, double quantity);
    
    @Override
    public int compareTo(Order o){
        if(orderCount < o.getOrderCount())return -1;
        if(orderCount > o.getOrderCount())return 1;
        return 0;
    }
    
    public Trader getOwner() {
        return owner;
    }

    public Crypto getType() {
        return type;
    }
    
    public long getOrderCount(){
        return orderCount;
    }

    public double getQuantity() {
        return quantity;
    }

    public double getVolExecuted() {
        return volExecuted;
    }
}
