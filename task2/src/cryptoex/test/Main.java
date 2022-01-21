package cryptoex.test;


import java.util.List;
import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;



public class Main{
    public static void main(String[] args){
        Result result;
        int totalFailures=0;
        System.out.println("Starting Unit Tests for CryptoExchange...");

        
        System.out.println("\nAdminTest tests");
        result = JUnitCore.runClasses(AdminTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }
        System.out.println("\nApprovedTraderStateTest tests");
        result = JUnitCore.runClasses(ApprovedTraderStateTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }
        System.out.println("\nBuyOrderTest tests");
        result = JUnitCore.runClasses(BuyOrderTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }
        System.out.println("\nFrozenTraderStateTest tests");
        result = JUnitCore.runClasses(FrozenTraderStateTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }
        System.out.println("\nOrderBookMapTest tests");
        result = JUnitCore.runClasses(OrderBookMapTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }
        System.out.println("\nOrderBookTest tests");
        result = JUnitCore.runClasses(OrderBookTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }

        System.out.println("\nOrderTest tests");
        result = JUnitCore.runClasses(OrderTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }

        System.out.println("\nPendingTraderStateTest tests");
        result = JUnitCore.runClasses(PendingTraderStateTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }

        System.out.println("\nSellOrderTest tests");
        result = JUnitCore.runClasses(SellOrderTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }

        System.out.println("\nTraderTest tests");
        result = JUnitCore.runClasses(TraderTest.class);

        if(result.wasSuccessful()){
            System.out.println("Tests successful");
        }else{
            for(Failure failure : result.getFailures()){
                System.out.println(failure.toString());
                totalFailures++;
            }
        }

        System.out.println("\n-------------------------------------------------");
        if(totalFailures==0)System.out.println("All tests completed successfully");
        else System.out.println(totalFailures+ " failures found");
        
        System.out.println("-------------------------------------------------");

    }
}