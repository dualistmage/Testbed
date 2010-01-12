import testPackage.*;

class test
{
    public static void main(String[] args) throws Exception {
        test_class1 obj1 = new test_class1();
        test_class1 obj2 = new test_class1();
        test_class2 modifier = new test_class2();

        modifier.assign( obj1 );
        obj2 = modifier.assign2();

        System.out.println();
        System.out.println("After assinging");
        System.out.println("-----------------------------------------");
        System.out.println("Param  Obj : " + Integer.toString(obj1.value1_) + " , " + obj1.value2_);
        System.out.println("return Obj : " + Integer.toString(obj2.value1_) + " , " + obj2.value2_);
        modifier.print();

        obj1.value1_ = 123;
        obj1.value2_ = "Changed";

        System.out.println("After changing values of Parameter Object");
        System.out.println("-----------------------------------------");
        System.out.println("Param  Obj : " + Integer.toString(obj1.value1_) + " , " + obj1.value2_);
        System.out.println("return Obj : " + Integer.toString(obj2.value1_) + " , " + obj2.value2_);
        modifier.print();

        obj2.value1_ = 987;
        obj2.value2_ = "is Changed";

        System.out.println("After changing values of return Object");
        System.out.println("-----------------------------------------");
        System.out.println("Param  Obj : " + Integer.toString(obj1.value1_) + " , " + obj1.value2_);
        System.out.println("return Obj : " + Integer.toString(obj2.value1_) + " , " + obj2.value2_);
        modifier.print();


    } // end - main()

} // end - test
