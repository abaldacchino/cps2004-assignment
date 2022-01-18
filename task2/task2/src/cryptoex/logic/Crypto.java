
package cryptoex.logic;


public class Crypto {
    private char symbol;
    private int decimals;
    private long supply;

    public Crypto(char symbol, int decimals, long supply) {
        this.symbol = symbol;
        this.decimals = decimals;
        this.supply = supply;
    }

    public char getSymbol() {
        return symbol;
    }

    public int getDecimals() {
        return decimals;
    }
    
    public long getSupply(){
        return supply;
    }
    
}
