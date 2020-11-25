import java.util.ArrayList;
import java.util.Arrays;

enum FloorButtonDir {UP, DOWN};
enum GateState {OPEN, CLOSE};
enum ElevatorDir {UP, DOWN, STALE};

class Floor {
    int topFloor;
    int bottomFloor;
    int currentFloor;

    public Floor(int topFloor, int bottomFloor, int currentFloor) {
        this.topFloor = topFloor;
        this.bottomFloor = bottomFloor;
        this.currentFloor = currentFloor;
    }
};

class ElevatorState {
    ElevatorDir dir;
    boolean mark[];

    ElevatorState() {
        mark = new boolean[100005];
        Arrays.fill(mark, false);
        dir = ElevatorDir.STALE;
    }

    void setDirection(ElevatorDir dir) {
        this.dir = dir;
    }

    void setElevatorStops(int floorNo) {
        mark[floorNo] = true;
    }

    void unsetElevatorStop(int floorNo) {
        mark[floorNo] = false;
    }
};

class Elevator implements Runnable {
    int id;
    GateState gateState;
    Floor floorDetails;
    ElevatorState elevatorState;

    public Elevator(int elevatorId, int topFloor, int bottomFloor) {
        id = elevatorId;
        int currentFloor = bottomFloor; // starts from bottom floor only
        floorDetails = new Floor(topFloor, bottomFloor, currentFloor);
        gateState = GateState.CLOSE;
        elevatorState = new ElevatorState();

        Thread t = new Thread(this, Integer.toString(id));
        t.start();
    }


    public void setFloorFromInside(int destinationFloor) {
        synchronized (elevatorState) {
            if(elevatorState.dir == ElevatorDir.STALE) {
                if(floorDetails.currentFloor > destinationFloor) {
                    elevatorState.dir = ElevatorDir.DOWN;
                } else if(floorDetails.currentFloor < destinationFloor) {
                    elevatorState.dir = ElevatorDir.UP;
                } else {
                    // let it be stale only since already at destination floor
                    elevatorState.dir = ElevatorDir.STALE;
                }
            }
        }
        assignFloorToOpenAt(destinationFloor);
    }

    public void assignFloorToOpenAt(int destinationFloor) {
        if(destinationFloor == -1 || destinationFloor > floorDetails.topFloor || destinationFloor < floorDetails.bottomFloor) {
            System.out.println("Invalid Floor: " + destinationFloor);
        }
        elevatorState.setElevatorStops(destinationFloor);
    }

    public void gateOpenAndClose() {
        if(gateState.equals(GateState.OPEN)) {
            System.out.println("Gate Already Open");
            return;
        }
        elevatorState.unsetElevatorStop(floorDetails.currentFloor);
        System.out.println("Elevator with id: " + id + " gate opened" );
        try { Thread.sleep(2000); } catch (Exception e) {};
        System.out.println("Elevator with id: " + id + " gate closed" );
        gateState = GateState.CLOSE;
    }


    @Override
    public void run() {
        startLift();
    }

    public void startLift() {
        while(true) {
            if(elevatorState.dir == ElevatorDir.STALE)  {
                continue;
            }
            if(floorDetails.currentFloor == floorDetails.bottomFloor) {
                elevatorState.dir = ElevatorDir.UP;
            }
            if(floorDetails.currentFloor == floorDetails.topFloor) {
                elevatorState.dir = ElevatorDir.DOWN;
            }

            if(elevatorState.mark[floorDetails.currentFloor]) {
                gateOpenAndClose();
            }

            boolean anyFloorSet = false;
            for(int i = floorDetails.bottomFloor; i<= floorDetails.topFloor;i++) {
                if(elevatorState.mark[i]) {
                    anyFloorSet = true;
                    break;
                }
            }

            if(!anyFloorSet) {
                elevatorState.dir = ElevatorDir.STALE;
                continue;
            }

            try { Thread.sleep(5000); } catch (Exception e) {};
            if(elevatorState.dir == ElevatorDir.DOWN) {
                floorDetails.currentFloor -= 1;
            } else {
                floorDetails.currentFloor += 1;
            }
        }
    }
};


class ElevatorOrchestrator {
    ArrayList<Elevator> elevators;
    int topFloor;
    int bottomFloor;
    public ElevatorOrchestrator (int numberOfElevators, int topFloor, int bottomFloor) {
        this.topFloor = topFloor;
        this.bottomFloor = bottomFloor;
        elevators = new ArrayList<>();
        for (int i=0;i<numberOfElevators;i++) {
            elevators.add(new Elevator(i, topFloor, bottomFloor));
        }
    }

    public void assignElevator(FloorButtonDir direction, int currentFloor) {
        int liftAssignedId = 0; // by default if any other elevator is not found
        int minDist = topFloor - bottomFloor + 1;
        for(int i=0;i<elevators.size();i++) {
            Elevator presentElevator = elevators.get(i);
            if ((direction.equals(FloorButtonDir.UP) && presentElevator.elevatorState.dir.equals(ElevatorDir.UP))
                || (direction.equals(FloorButtonDir.DOWN) && presentElevator.elevatorState.dir.equals(ElevatorDir.DOWN))
                || presentElevator.elevatorState.dir.equals(ElevatorDir.STALE)) {
                // Lift of concern
                if(minDist > Math.abs(presentElevator.floorDetails.currentFloor - currentFloor)) {
                    minDist = Math.abs(presentElevator.floorDetails.currentFloor - currentFloor);
                    liftAssignedId = i;
                }
            }
        }
        elevators.get(liftAssignedId).assignFloorToOpenAt(currentFloor);
        System.out.println("Assigned Lift: " + elevators.get(liftAssignedId).id);
    }
};

public class ElevatorSystem {
    public static void main(String args[]) {

    }
}
