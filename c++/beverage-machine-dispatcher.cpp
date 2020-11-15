#include "bits/stdc++.h"
using namespace std;

/* 2 Hrs Round
Vending machine(beverage)
 - items→ coke, soda, pepsi
 - Denominations -> c1(1), c2(5), c3(10), c4(25)
 - Coke → 25, soda→ 35, pepsi→ 45
 - Initialise the inventory→ coke-->5, soda-->5 pepsi→ 5
 - Inventory of coins→ 5 coin each

Actions of user->
 - Check the price of item
 - Buy the item(only 1 item at a time)
 - Pay for the item
 - Get the change
*/

enum DenominationType { C1, C2, C3, C4 };
enum BeverageType {COKE, SODA, PEPSI};

class Beverage {
public:
    int cost;
    int amountOfBeverage;
    BeverageType type;
};

class Coke: public Beverage {
public:
    Coke(int cost, int amountOfBeverage) {
        this->cost = cost;
        this->amountOfBeverage = amountOfBeverage;
        this->type = COKE;
    }
};

class Soda: public Beverage {
public:
    Soda(int cost, int amountOfBeverage) {
        this->cost = cost;
        this->amountOfBeverage = amountOfBeverage;
        this->type = SODA;
    }
};

class Pepsi: public Beverage {
public:
    Pepsi(int cost, int amountOfBeverage) {
        this->cost = cost;
        this->amountOfBeverage = amountOfBeverage;
        this->type = PEPSI;
    }
};

struct Money {
    DenominationType type;
    int amount;

    Money(DenominationType type, int amount) {
        this->type = type;
        this->amount = amount;
    }
};

class VendingMachine {
public:
    map<DenominationType, int> cashierMap;
    map<DenominationType, int> denominationExchangePrice;
    map<BeverageType, Beverage*>beverages;
    map<BeverageType, int> beverageCost;
    vector<Money> extraChange;

    VendingMachine(map<DenominationType, int>& denominationExchangePrice) {
        this->denominationExchangePrice = denominationExchangePrice;
    }

    void addSetupMoney(const vector<Money>& money)
    {
        for(int i=0;i<money.size();i++)
        {
            _addDenomination(money[i].type, money[i].amount);
        }
    }

    void setupBeverage(BeverageType type, const int cost, const int amount) {
        switch (type) {
            case COKE:
                beverageCost[COKE] = cost;
                if(beverages.count(type) == 0)
                    beverages[type] = new Coke(cost, amount);
                else
                {
                    beverages[type]->cost = cost;
                    beverages[type]->amountOfBeverage += amount;
                }
                break;
            case PEPSI:
                beverageCost[PEPSI] = cost;
                if(beverages.count(type) == 0)
                    beverages[type] = new Pepsi(cost, amount);
                else
                {
                    beverages[type]->cost = cost;
                    beverages[type]->amountOfBeverage += amount;
                }
                break;
            case SODA:
                beverageCost[SODA] = cost;
                if(beverages.count(type) == 0)
                    beverages[type] = new Soda(cost, amount);
                else
                {
                    beverages[type]->cost = cost;
                    beverages[type]->amountOfBeverage += amount;
                }
                break;
        }
    }

    void _addDenomination(DenominationType type, int coins)
    {
        cashierMap[type] += coins;
    }

    void _subtractDenomination(DenominationType type, int coins)
    {
        cashierMap[type] -= coins;
    }

    int getPriceOfBeverage(BeverageType type)
    {
        if(beverageCost.count(type) == 0)
        {
            cout<<"Beverage Cost for beverage not set"<<endl;
            return -1;
        }
        cout<<"Beverage Cost is : "<<beverageCost[type]<<endl;
        return beverageCost[type];
    }


    int _countMoneyTotal(const vector<Money>& money)
    {
        int total = 0;
        for(int i=0;i<money.size();i++)
        {
            total += denominationExchangePrice[money[i].type] * money[i].amount;
        }
        return total;
    }

    bool _findChange(int idx, int amount, vector<Money>& total, vector<Money> res)
    {
        if(amount < 0)
            return false;
        if(amount == 0)
        {
            extraChange = res;
            return true;
        }
        if(idx == total.size())
            return false;

        bool changePresent = false;
        for(int i=1;i<=total[idx].amount;i++)
        {
            res.push_back(Money(total[idx].type, i));
            changePresent |= _findChange(idx+1, amount - (denominationExchangePrice[total[idx].type] * i), total, res);
            res.pop_back();
            if(changePresent)
                return changePresent;
        }
        changePresent |= _findChange(idx+1, amount, total, res);
        return changePresent;
    }

    bool _checkChange(int amount, const vector<Money>& userMoney)
    {
        extraChange.clear();
        vector<Money>total;
        vector<Money>res;
        map<DenominationType, int> tempCashier;
        tempCashier = cashierMap;
        for(int i=0;i<userMoney.size();i++)
        {
            tempCashier[userMoney[i].type] += userMoney[i].amount;
        }
        for(auto i: tempCashier)
        {
            total.push_back(Money(i.first, i.second));
        }
        return _findChange(0, amount, total, res);
    }

    void _giveBeverage(BeverageType type, const vector<Money>& money)
    {
        beverages[type]->amountOfBeverage -= 1;
        cout<<"New Beverage Amount: "<< beverages[type]->amountOfBeverage<<endl;
        for(int i=0;i<money.size();i++)
        {
            _addDenomination(money[i].type, money[i].amount);
        }

        cout<<"---- Giving Change ----"<<endl;
        for(int i=0;i<extraChange.size();i++)
        {
            cout<<"> "<<extraChange[i].type<<" "<<" no of coins : "<<extraChange[i].amount<<endl;
            _subtractDenomination(extraChange[i].type, extraChange[i].amount);
        }
        cout<<"----- Change Given ----"<<endl<<endl;
        return;
    }

    bool buyBeverage(BeverageType type, const vector<Money>& money)
    {
        if(beverageCost.count(type) == 0)
        {
            cout<<"Beverage is not present"<<endl;
            return false;
        }

        if(beverages[type]->amountOfBeverage <= 0)
        {
            cout<<"Beverage is sold"<<endl;
            return false;
        }

        int totalMoney = _countMoneyTotal(money);
        if(totalMoney < beverageCost[type])
        {
            cout<<"Insufficeint amount of money"<<endl;
            return false;
        }

        if(!_checkChange(totalMoney - beverageCost[type], money))
        {
            cout<<"Do not have change for the amount : "<<totalMoney<<endl;
            return false;
        }
        cout<<"User gave money : "<<totalMoney<<endl;
        cout<<"Beverage given"<<endl;
        _giveBeverage(type, money);

        return true;
    }

    void findVMMoney()
    {
        cout<<"--- Money in Vending Machine ---"<<endl;
        vector<Money>wallet;
        for(auto i: cashierMap)
        {
            cout<<i.first<<", No of Coins : "<<i.second<<endl;
            wallet.push_back(Money(i.first, i.second));
        }
        cout<<"Total Money: "<<_countMoneyTotal(wallet)<<endl;
        cout<<"--------------------------------"<<endl<<endl;
    }
};

VendingMachine* setupConfig()
{
    vector<Money> money;
    map<DenominationType, int> denominationExchangePrice;
    denominationExchangePrice[C1] = 1;
    denominationExchangePrice[C2] = 5;
    denominationExchangePrice[C3] = 10;
    denominationExchangePrice[C4] = 25;

    money.push_back({C1, 5});
    money.push_back({C2, 5});
    money.push_back({C3, 5});
    money.push_back({C4, 5});

    VendingMachine* vm = new VendingMachine(denominationExchangePrice);
    vm->addSetupMoney(money);
    vm->setupBeverage(COKE, 25, 5);
    vm->setupBeverage(SODA, 35, 5);
    vm->setupBeverage(PEPSI, 45, 5);

    return vm;
}

void Driver()
{
    VendingMachine* vm = setupConfig();
    vm->getPriceOfBeverage(COKE);

    // TC1
    vm->findVMMoney();
    vector<Money> money;
    money.push_back({C3, 5});
    money.push_back({C4, 5});
    vm->buyBeverage(COKE, money);
    vm->findVMMoney();
}

int main()
{
    Driver();
    return 0;
}
