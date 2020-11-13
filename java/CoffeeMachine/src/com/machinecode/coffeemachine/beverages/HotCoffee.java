package com.machinecode.coffeemachine.beverages;

import static com.machinecode.coffeemachine.utils.BeverageConstants.BEVERAGE_HOT_COFFEE;
import java.util.logging.Logger;

public class HotCoffee implements IBeverage {
    private Logger logger = Logger.getLogger("HotCoffee");

    @Override
    public String getType() {
        return BEVERAGE_HOT_COFFEE;
    }


    @Override
    public void drink() {
        System.out.println("I am drinking : " + BEVERAGE_HOT_COFFEE);
    }
}
