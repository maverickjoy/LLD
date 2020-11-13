package com.machinecode.coffeemachine.beverages;

import static com.machinecode.coffeemachine.utils.BeverageConstants.*;

public class BeverageFactory {
    public static IBeverage getBeverage(String beverageType) {
        switch (beverageType) {
            case BEVERAGE_GREEN_TEA: return new GreenTea();
            case BEVERAGE_BLACK_TEA: return new BlackTea();
            case BEVERAGE_HOT_TEA: return new HotTea();
            case BEVERAGE_HOT_COFFEE: return new HotCoffee();
            default: return null;
        }
    }
}
