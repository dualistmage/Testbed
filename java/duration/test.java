///
/// @brief  This code will check the duration time.
/// @author Dohyun Yun
/// @email  dualistmage@nate.com
///

import java.io.*;

class test
{
    public static void main(String[] args) throws Exception {
        TimeChecker timer = new TimeChecker();

        timer.begin();
        Thread.sleep( 1234 );
        timer.end();
        System.out.println("Duration : " + timer.toString() );

    } // end - main()

    static class TimeChecker
    {
        private long nanoStart_;
        private long nanoEnd_;
        private double duration_;

        public TimeChecker() {
            nanoStart_  = 0;
            nanoEnd_    = 0;
            duration_   = 0.0;
        }
        public void begin()  { nanoStart_   = System.nanoTime(); }
        public void end()    {
            nanoEnd_     = System.nanoTime();
            duration_ = (nanoEnd_ - nanoStart_) / Math.pow(10 , 9);
        }
        public String toString() { return Double.toString(duration_); }
        public double getDuration() { return duration_; }
    }

} // end - test
