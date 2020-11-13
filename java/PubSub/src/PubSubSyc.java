
class Top {
    int data;
    boolean valueSet;
    Top() {
        valueSet = false;
        data = 0;
    }

    public synchronized Integer getData() {
        while(!valueSet) {
            try{wait();} catch (Exception e) {}
        }
        valueSet = false;
        notify();
        return data;
    }

    public synchronized void insertData(int value) {
        while(valueSet) {
            try{wait();} catch (Exception e) {}
        }
        data = value;
        valueSet = true;
        notify();
    }
}

class Pub implements Runnable {
    Top topic;
    Pub(Top topic) {
        this.topic = topic;
        Thread t = new Thread(this, "Pub");
        t.start();
    }

    @Override
    public void run() {
        int no = 0;
        while(true) {
            no += 1;
            topic.insertData(no);
            System.out.println("Pub: " + no);
            try {Thread.sleep(500); } catch (Exception e) {}
        }
    }
}

class Sub implements Runnable {
    Top topic;
    Sub(Top topic) {
        this.topic = topic;
        Thread t = new Thread(this, "Sub");
        t.start();
    }

    @Override
    public void run() {
        while(true) {
            int value = topic.getData();
            System.out.println("Sub: " + value);
            try {Thread.sleep(1000);} catch (Exception e) {}
        }
    }
}

public class PubSubSyc {
    public static void main(String args[]) {
        Top topic = new Top();
        new Sub(topic);
        new Pub(topic);
    }
}
