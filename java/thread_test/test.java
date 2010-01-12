public class test
{
    public static void main(String[] args) throws Exception {
        MyRunnable t1 = new MyRunnable(1);
        MyRunnable t2 = new MyRunnable(2);
        Thread a = new Thread(t1);
        Thread b = new Thread(t2);
        a.start();
        b.start();

        a.join();
        b.join();

        System.out.println("Job done.");
    } // end - main()
} // end - test

class MyRunnable implements Runnable {

    private int sleepTime_;

    public MyRunnable(int sleepTime) {
        sleepTime_ = sleepTime;
    } // end - MyRunnable

    public void run() {
        int a = 100;
        while(a > 0) {
            // ---------------------------------------------
            // This is the area which thread is going to do.
            // ---------------------------------------------
            try {
                Thread.sleep( sleepTime_ * 1000 );
            } catch ( InterruptedException e ) {
            }
            System.out.println("Waiting " + sleepTime_ + " secs");
            a--;
        }
    }

} // end - MyRunnable

