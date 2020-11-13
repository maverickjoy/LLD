import java.util.*;
import java.util.concurrent.ConcurrentHashMap;


class Throttler {

    // bucket is based up on seconds
    class Bucket {
        int count;
        Long timestamp;

        public Bucket(int count, Long timestamp) {
            this.count = count;
            this.timestamp = timestamp;
        }
    }

    class DataPipe {
        Long duration;
        Deque<Bucket> queue;
        Long totalRequests;

        public DataPipe(Long duration, Deque<Bucket> queue, Long totalRequests) {
            this.duration = duration;
            this.queue = queue;
            this.totalRequests = totalRequests;
        }
    }

    ConcurrentHashMap<String, DataPipe> requestPerIP;

    private void maintainDataPipeSize(DataPipe dp) {
        Long lastTimeStamp = dp.queue.peekLast().timestamp;
        while (!dp.queue.isEmpty()) {
            Long firstTimeStamp = dp.queue.peekFirst().timestamp;
            if (lastTimeStamp - firstTimeStamp > dp.duration) {
                Bucket firstBucket = dp.queue.pollFirst();
                dp.totalRequests -= firstBucket.count;
            } else {
                break;
            }
        }
    }

    public boolean canAddRequest(Client client) {
        if (!requestPerIP.contains(client.ip)) {
            requestPerIP.put(client.ip, new DataPipe(client.slaContract.duration, new LinkedList<>(), 0L));
        }
        Long presentTimestamp = System.currentTimeMillis();
        if(requestPerIP.get(client.ip).queue.isEmpty()) {
            requestPerIP.get(client.ip).queue.addLast(new Bucket(1, presentTimestamp));
            requestPerIP.get(client.ip).totalRequests += 1;
            return true;
        } else {
            Long currentSecond = presentTimestamp / 1000;
            Long lastSecond = requestPerIP.get(client.ip).queue.peekLast().timestamp / 1000;
            if (currentSecond == lastSecond) {
                Bucket lastBucket = requestPerIP.get(client.ip).queue.peekLast();
                lastBucket.count += 1;
                requestPerIP.get(client.ip).queue.addLast(lastBucket);
            } else {
                requestPerIP.get(client.ip).queue.addLast(new Bucket(1, presentTimestamp));
            }
            requestPerIP.get(client.ip).totalRequests += 1;
            maintainDataPipeSize(requestPerIP.get(client.ip));
            return client.slaContract.noOfRequests <= requestPerIP.get(client.ip).totalRequests;
        }
    } 
}

class Vip {

    // bucket is based up on seconds
    class Bucket {
        int count;
        Long timestamp;

        public Bucket(int count, Long timestamp) {
            this.count = count;
            this.timestamp = timestamp;
        }
    }
    ConcurrentHashMap<String, List<Bucket> > totalRequestsPerIP;
    ConcurrentHashMap<String, List<Bucket> > totalRequestsPerBrowser;

    public void addRequestPerBrowser(Client client) {
        if (!totalRequestsPerBrowser.contains(client.browserName)) {
            totalRequestsPerBrowser.put(client.browserName, new ArrayList<>());
        }
        Long presentTimestamp = System.currentTimeMillis();
        if(totalRequestsPerBrowser.get(client.browserName).isEmpty()) {
            totalRequestsPerBrowser.get(client.browserName).add(new Bucket(1, presentTimestamp));
        } else {
            Long currentSecond = presentTimestamp / 1000;
            List<Bucket> requests = totalRequestsPerBrowser.get(client.browserName);
            Long lastSecond = requests.get(requests.size()-1).timestamp / 1000;
            if (currentSecond == lastSecond) {
                Bucket newBucket = requests.get(requests.size()-1);
                newBucket.count += 1;
                requests.set(requests.size()-1, newBucket);
            } else {
                int preSum = requests.get(requests.size()-1).count;
                preSum += 1;
                requests.add(new Bucket(preSum, presentTimestamp));
            }
        }
    }

    public void addRequestPerIp(Client client) {
        if (!totalRequestsPerIP.contains(client.ip)) {
            totalRequestsPerIP.put(client.ip, new ArrayList<>());
        }
        Long presentTimestamp = System.currentTimeMillis();
        if(totalRequestsPerIP.get(client.ip).isEmpty()) {
            totalRequestsPerIP.get(client.ip).add(new Bucket(1, presentTimestamp));
        } else {
            Long currentSecond = presentTimestamp / 1000;
            List<Bucket> requests = totalRequestsPerIP.get(client.ip);
            Long lastSecond = requests.get(requests.size()-1).timestamp / 1000;
            if (currentSecond == lastSecond) {
                Bucket newBucket = requests.get(requests.size()-1);
                newBucket.count += 1;
                requests.set(requests.size()-1, newBucket);
            } else {
                int preSum = requests.get(requests.size()-1).count;
                preSum += 1;
                requests.add(new Bucket(preSum, presentTimestamp));
            }
        }
    }

    public void addRequest(Client client) {
        addRequestPerBrowser(client);
        addRequestPerIp(client);
    }

    public int getNumberOfRequestsInBrowser(String browserName, int lastNSeconds) {
        // TODO
        int noOfRequests = 0;
        return noOfRequests;
    }

    public int getNumberOfRequestsInIP(String ip, int lastNSeconds) {
        // TODO
        int noOfRequests = 0;
        return noOfRequests;
    }

    public void getRequest(Client client) {
        addRequest(client);

    }
}

class SLA {
    Long duration;
    Long noOfRequests;
}

class Client implements Runnable {
    SLA slaContract;
    String clientName;
    String browserName;
    String ip;

    public Client(SLA slaContract, String clientName, String browserName, String ip) {
        this.slaContract = slaContract;
        this.clientName = clientName;
        this.browserName = browserName;
        this.ip = ip;
    }

    @Override
    public void run() {

    }
}

public class RateLimitter {

    public static void Driver() {
        //
    }

    public static void main(String args[]) {

    }
}

