import java.util.LinkedList;
import java.util.Queue;

class Topic {
    Queue<Integer> data;
    Topic() {
        data = new LinkedList<>();
    }

    public synchronized Integer getData() {
        if (data.size() == 0) {
            System.out.println("Empty");
            return -1;
        }
        return data.poll();
    }

    public synchronized void insertData(int value) {
        data.add(value);
    }
}

class Publisher implements Runnable {
    Topic topic;
    Publisher(Topic topic) {
        this.topic = topic;
        Thread t = new Thread(this, "Publisher");
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

class Subscriber implements Runnable {
    Topic topic;
    Subscriber(Topic topic) {
        this.topic = topic;
        Thread t = new Thread(this, "Subscriber");
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





public class PubSub {
    public static void main(String args[]) {
        Topic topic = new Topic();
        new Subscriber(topic);
        new Publisher(topic);
    }
}
