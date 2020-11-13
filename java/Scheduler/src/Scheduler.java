import java.util.*;


enum TaskType {
    ONE_TIME,
    INTERVAL_BASED
}

class Tasks implements Runnable {
    int taskId;
    TaskType taskType;
    String taskName;
    int interval;


    Tasks(int taskId, TaskType taskType, String taskName) {
        this.taskId = taskId;
        this.taskType = taskType;
        this.taskName = taskName;
        this.interval = 0; // default
    }

    public void addIntervalDuration(int interval) {
        if(taskType != TaskType.INTERVAL_BASED) {
            System.out.println("Cannot add interval for non interval type tasks");
        }
        this.interval = interval;
    }

    @Override
    public void run() {
        // some task related stuff
        for(int i=0;i<100;i++) {
            System.out.println("Task is running: " + taskName + "Task Id is: " + taskId);
        }
    }
}


class Schedule implements Runnable {
    public volatile Map<Long, Queue<Tasks>> taskTimeMapper;

    Schedule () {
        taskTimeMapper = new HashMap<>();
        Thread t = new Thread(this, "Schedule Monitor Thread");
        t.start();
    }

    public void addTasks(Tasks taskObj, Long timestamp) {
        if (!taskTimeMapper.containsKey(timestamp)) {
            taskTimeMapper.put(timestamp, new LinkedList<>());
        }
        taskTimeMapper.get(timestamp).add(taskObj);
    }

    public void runTasks(Tasks taskObj) {
        Thread t = new Thread(taskObj);
        t.start();
        // If we do join it will become synchronous one task will finish then other will start
        // try{t.join();} catch (Exception e) {}
    }

    public void runTasksAndWait(Tasks taskObj) {
        Thread t = new Thread(taskObj);
        t.start();
        try{t.join();} catch (Exception e) {}
    }

    @Override
    public void run() {
        scheduleTasksMonitor();
    }

    public synchronized void scheduleTasksMonitor() {
        while (true) {
            Long presentTime = System.currentTimeMillis();
            System.out.println("Present Time : " + presentTime);
            System.out.println(taskTimeMapper);
            if (taskTimeMapper.containsKey(presentTime)) {
                while (!taskTimeMapper.get(presentTime).isEmpty()){
                    Tasks task = taskTimeMapper.get(presentTime).poll();

                    // Handle Waiting tasks using queue 
                    if(task.taskType == TaskType.INTERVAL_BASED){
                        runTasksAndWait(task);
                        Long nextTime = System.currentTimeMillis() + task.interval;
                        addTasks(task, nextTime);
                    }
                    if(task.taskType == TaskType.ONE_TIME) {
                        runTasks(task);
                    }
                }
                if(taskTimeMapper.get(presentTime).size() == 0) {
                    taskTimeMapper.remove(presentTime);
                }
            }
        }
    }
}



public class Scheduler {

    public static Schedule schedule;

    public static void scheduleAnTaskInstance(Tasks taskObj, Long timestamp) {
        schedule.addTasks(taskObj, timestamp);
    }

    public static void Driver() {
        System.out.println("------ INSIDE DRIVER ---------");

        // TC1
        Long presentTime = System.currentTimeMillis();
        Long taskTimeStamp = presentTime + 20;

        System.out.println("Task Timestamp : " + taskTimeStamp);
        int taskId = 1;
        Tasks taskObj1 = new Tasks(taskId, TaskType.ONE_TIME, "one time task");
        scheduleAnTaskInstance(taskObj1, taskTimeStamp);


        // TC2
        System.out.println("Task Timestamp : " + taskTimeStamp);
        taskId = 2;
        Tasks taskObj2 = new Tasks(taskId, TaskType.ONE_TIME, "one time task");
        scheduleAnTaskInstance(taskObj2, taskTimeStamp);

        // TC3
        System.out.println("Task Timestamp : " + taskTimeStamp);
        taskId = 3;
        Tasks taskObj3 = new Tasks(taskId, TaskType.ONE_TIME, "one time task");
        scheduleAnTaskInstance(taskObj3, taskTimeStamp);

        // TC4
        System.out.println("Task Timestamp : " + taskTimeStamp);
        taskId = 4;
        Tasks taskObj4 = new Tasks(taskId, TaskType.INTERVAL_BASED, "interval time task");
        int intervalDuration = 20;
        taskObj4.addIntervalDuration(intervalDuration);
        scheduleAnTaskInstance(taskObj4, presentTime);
    }

    public static void main(String args[]) {
        schedule = new Schedule();
        Driver();
    }
}
