#include "bits/stdc++.h"
using namespace std;

class Ticket {
public:
    int ticketNo;
    int price;
    int slotId;

    Ticket(int ticketNo, int price, int slotId) {
        this->ticketNo = ticketNo;
        this->price = price;
        this->slotId = slotId;
    }
};

class TicketMaster {
    const int price = 10;
    ParkingLot* parkingLot;
    TicketMaster(ParkingLot* parkingLot) {
        srand(time(0));
        this->parkingLot = parkingLot;
    }

    int getTicketNo()
    {
        return rand();
    }

    Ticket* getTicket()
    {
        this->parkingLot->allocateSlot();
        Ticket* ticket = new Ticket(getTicketNo(), price, );
    }

};

abstract class Iprovisioner {
public:
    virtual Slot* allocateSlot() = 0;
    virtual bool deallocateSlot() = 0;
};

enum VehicleType { SMALL, BIG, MEDIUM };

class Provisioner: public Iprovisioner {
    int size;
    vector<Slot*> slots;
    unordered_map<VehicleType, SlotType> vechicleToSlotMapping;
public:
    Provisioner(int size) {
        this->size = size;
        for(int i=0;i<size/3;i++)
            slots.push_back(new SmallSlot(slots.size()));
        for(int i=0;i<size/3;i++)
            slots.push_back(new MidSlot(slots.size()));
        for(int i=0;i<size/3;i++)
            slots.push_back(new BigSlot(slots.size()));

        vechicleToSlotMapping[VehicleType.SMALL] = SlotType.SMALL_SLOT;
        vechicleToSlotMapping[VehicleType.MEDIUM] = SlotType.MEDIUM_SLOT;
        vechicleToSlotMapping[VehicleType.BIG] = SlotType.BIG_SLOT;
    }

    Slot* allocateSlot(VehicleType vt) {
        for(int i=0;i<size;i++)
            if(slots[i]->type == vechicleToSlotMapping[vt] && !slots[i]->isOccupied)
            {
                slots[slotId]->isOccupied = true;
                return slots[i];
            }
        return NULL;
    }

    bool deallocateSlot(int slotId) {
        if(slots.size() > slotId && slots[slotId]->isOccupied)
        {
            slots[slotId]->isOccupied = false;
            return true;
        }
        return false;
    }
};

class ParkingLot {
public:
    int size;
    Iprovisioner* provisioner;
    ParkingLot(Iprovisioner* provisioner) {
        this->provisioner = provisioner;
    }

    Slot* allocateSlot(VehicleType vt) {
        return provisioner->allocateSlot(vt);
    }

    bool deallocateSlot(int slotId) {
        return provisioner->deallocateSlot(slotId);
    }
};

class Slot {
    int id;
public:
    const SlotType type;
    bool isOccupied;
    bool isElectric;
    int length, width;

    int getSlotId()
    {
        return id;
    }

    void setSlotId(int id)
    {
        this->id = id;
    }
};

enum SlotType { SMALL_SLOT, BIG_SLOT, MEDIUM_SLOT };
const int MID_LENGTH = 20;
const int MID_WIDTH = 10;
class MidSlot: public Slot {
public:
    MidSlot(int id) {
        this->setSlotId(id);
        this->isOccupied = false;
        this->type = MEDIUM_SLOT;
        this->length = MID_LENGTH;
        this->width = MID_WIDTH;
    }
};

const int SMALL_LENGTH = 10;
const int SMALL_WIDTH = 5;
class SmallSlot: public Slot {
public:
    SmallSlot(int id) {
        this->setSlotId(id);
        this->isOccupied = false;
        this->type = SMALL_SLOT;
        this->length = SMALL_LENGTH;
        this->width = SMALL_WIDTH;
    }
};

const int BIG_LENGTH = 30;
const int BIG_WIDTH = 10;
class BigSlot: public Slot {
public:
    BigSlot(int id) {
        this->setSlotId(id);
        this->isOccupied = false;
        this->type = BIG_SLOT;
        this->length = BIG_LENGTH;
        this->width = BIG_WIDTH;
    }
};

void Driver()
{

    return;
}

int main()
{
    Driver();
    return;
}
