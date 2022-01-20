
package cryptoex.logic;


public class SellOrder extends Order implements Comparable<SellOrder>{
    
    public SellOrder(Trader owner, Crypto type, double quantity, 
            double askPrice, long orderNo) {
        super(owner, type, quantity, askPrice, orderNo, "SELL");
    }

    /**
     * Method compares two sell orders by priority of execution, and returns
     * -1 if this has higher priority, 1 if this has lower priority and 0 if 
     * they are the same object.
     * Priority is calculated by first comparing ask price (lower ask price has
     * higher priority) and then by orderNo (lower orderNo has higher
     * priority - as a lower orderCount indicates the order is older).
     * 
     * Ordering is done such that ordering in ascending order is ordering in
     * highest priority first
     * 
     * @param o SellOrder which is being compared to this
     * @return  returns 1 
     */
    @Override
    public int compareTo(SellOrder o){
        if(getPrice() < o.getPrice())return -1;
        if(getPrice() > o.getPrice())return 1;
        if(getOrderNo() < o.getOrderNo())return -1;
        if(getOrderNo() > o.getOrderNo())return 1;
        return 0;
    }
}
