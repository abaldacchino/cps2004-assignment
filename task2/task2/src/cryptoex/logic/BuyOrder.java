
package cryptoex.logic;


public class BuyOrder extends Order implements Comparable<BuyOrder>{
    
    public BuyOrder(Trader owner, Crypto type, double quantity, double bidPrice,
            long orderNo) {
        super(owner, type, quantity, bidPrice, orderNo, "BUY");
    }

    /**
     * Method compares two buy orders by priority of execution, and returns
     * -1 if this has higher priority, 1 if this has lower priority and 0 if 
     * they are the same object.
     * Priority is calculated by first comparing bid price (higher price has
     * higher priority) and then by orderNo (lower orderNo has higher
     * priority - as a lower orderCount indicates the order is older).
     * Note that market orders (with price 0) take precedence.
     * 
     * Ordering is done such that ordering in ascending order is ordering in
     * highest priority first
     * 
     * @param o SellOrder which is being compared to this
     * @return  returns 1 
     */
    @Override
    public int compareTo(BuyOrder o){
        // Account for special case where order is Market
        if(getPrice()==0 || o.getPrice()==0){
            if(getPrice() !=0)return 1;
            if(o.getPrice() !=0)return -1;
        }else{
            if(getPrice() < o.getPrice())return 1;
            if(getPrice() > o.getPrice())return -1;
        }
        
        if(getOrderNo() < o.getOrderNo())return -1;
        if(getOrderNo() > o.getOrderNo())return 1;
        return 0;
    }
}
