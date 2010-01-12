package testPackage;

import testPackage.*;

public class test_class2
{
    test_class1 in_;

    public test_class2()
    {
        in_ = new test_class1();
        in_.value1_ = 13;
        in_.value2_ = "Hello";
    }

    public void assign(test_class1 obj)
    {
        obj = in_;
    }
    public test_class1 assign2()
    {
        return in_;
    } 
    public void print()
    {
        System.out.println("member Obj : " + Integer.toString(in_.value1_) + " , " + in_.value2_);
        System.out.println();
    }
}
