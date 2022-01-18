
package cryptoex.logic;


public class OrderBook {
    private final Crypto cryptoType;

    public Crypto getCryptoType() {
        return cryptoType;
    }
    
    public OrderBook(Crypto cryptoType){
        this.cryptoType = cryptoType;
    }
    
}
