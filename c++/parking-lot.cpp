#include "bits/stdc++.h"
using namespace std;

class Ticket {
public:
    int ticketNo;
    int basePrice;
    int slotId;
    time_t timestamp;

    Ticket(int ticketNo, int price, int slotId) {
        this->ticketNo = ticketNo;
        this->basePrice = price;
        this->slotId = slotId;
        this->timestamp = time(0);
    }

    void printTicket()
    {
        cout<<"---- Ticket Details ----"<<endl;
        cout<<"Ticket Id : "<<ticketNo<<endl;
        cout<<"Slot Id : "<<slotId<<endl;
        cout<<"TimeStamp : "<<timestamp<<endl;
        cout<<"------------------------"<<endl<<endl;
    }
};

enum VehicleType { SMALL, BIG, MEDIUM };
enum SlotType { SMALL_SLOT, BIG_SLOT, MEDIUM_SLOT };
class Slot {
    int id;
public:
    SlotType type;
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

class Iprovisioner {
public:
    virtual Slot* allocateSlot(VehicleType vt) = 0;
    virtual bool deallocateSlot(int slotId) = 0;
};

class Provisioner: public Iprovisioner {
    int parkingLotSize;
    vector<Slot*> slots;
    unordered_map<VehicleType, SlotType> vechicleToSlotMapping;
public:
    Provisioner(int size) {
        this->parkingLotSize = size;
        for(int i=0;i<parkingLotSize/3;i++)
            slots.push_back(new SmallSlot(slots.size()));
        for(int i=0;i<parkingLotSize/3;i++)
            slots.push_back(new MidSlot(slots.size()));
        for(int i=0;i<parkingLotSize/3;i++)
            slots.push_back(new BigSlot(slots.size()));

        vechicleToSlotMapping[SMALL] = SMALL_SLOT;
        vechicleToSlotMapping[MEDIUM] = MEDIUM_SLOT;
        vechicleToSlotMapping[BIG] = BIG_SLOT;
    }

    Slot* allocateSlot(VehicleType vt) {
        for(int slotId=0;slotId<parkingLotSize;slotId++)
            if(slots[slotId]->type == vechicleToSlotMapping[vt] && !slots[slotId]->isOccupied)
            {
                slots[slotId]->isOccupied = true;
                return slots[slotId];
            }
        return NULL;
    }

    bool deallocateSlot(int slotId) {
        if(slots.size() > slotId && slotId >= 0 && slots[slotId]->isOccupied)
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


class TicketMaster {
public:
    const int basePrice = 10;
    ParkingLot* parkingLot;

    TicketMaster(ParkingLot* parkingLot) {
        srand(time(0));
        this->parkingLot = parkingLot;
    }

    int getTicketNo()
    {
        return rand();
    }

    Ticket* getTicket(VehicleType vt)
    {
        Slot* slot = this->parkingLot->allocateSlot(vt);
        Ticket* ticket = new Ticket(getTicketNo(), basePrice, slot->getSlotId());
        return ticket;
    }

    int returnTicketAndCalculateFare(Ticket* ticket)
    {
        bool isDeallocated = this->parkingLot->deallocateSlot(ticket->slotId);
        if(!isDeallocated) {
            cout<<"Wrong SlotID or already deallocated slot"<<endl;
            return -1;
        }
        return basePrice + 10; // some business logic for price
    }

    void printTicket(Ticket* ticket)
    {
        ticket->printTicket();
    }
};


class TicketMasterFactory {
public:
    const static int parkingLotSize = 30;
    static TicketMaster* getTicketMaster() {
        Iprovisioner* iprovisioner = new Provisioner(parkingLotSize);
        ParkingLot* parkingLot = new ParkingLot(iprovisioner);
        return new TicketMaster(parkingLot);
    }
};


void Driver()
{
    TicketMaster* ticketMaster = TicketMasterFactory::getTicketMaster();

    // TC 1
    VehicleType vt = BIG;
    Ticket* ticket = ticketMaster->getTicket(vt);
    ticketMaster->printTicket(ticket);
    int price = ticketMaster->returnTicketAndCalculateFare(ticket);
    cout<<"Price : "<<price<<endl;

    // TC 2
    price = ticketMaster->returnTicketAndCalculateFare(ticket);
    cout<<"Price : "<<price<<endl;
    return;
}

int main()
{
    Driver();
    return 0;
}
