package com.machinecode.coffeemachine.beverages;

import static com.machinecode.coffeemachine.utils.BeverageConstants.BEVERAGE_BLACK_TEA;
import java.util.logging.Logger;

public class BlackTea implements IBeverage {

    private Logger logger = Logger.getLogger("BlackTea");

    @Override
    public String getType() {
        return BEVERAGE_BLACK_TEA;
    }

    @Override
    public void drink() {
        System.out.println("I am drinking : " + BEVERAGE_BLACK_TEA);
    }
}
