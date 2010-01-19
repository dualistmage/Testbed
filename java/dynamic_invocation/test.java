///
/// @brief  This code will show how to use dynamic invocation in java.
/// @author Dohyun Yun
///
import java.lang.reflect.Method;

public class test
{
    public static void main(String[] args) throws Exception {

        Class testClassInfo = TestClass.class;
        Method[] methods = testClassInfo.getDeclaredMethods();
        TestClass obj = new TestClass();

        for (Method method : methods)
        {
            String methodName = method.getName();
            System.out.println( "------------------------------------------" );
            System.out.println( "Method Name : " + methodName );
            System.out.println( "Method Name with param : " + method.toString() );
            System.out.println( "Method Param No : " + method.getParameterTypes().length );

            if ( methodName.equals("testMethodOne") )
                    method.invoke(obj);
            if ( methodName.equals("testMethodTwo") )
                    method.invoke(obj, 10);
            if ( methodName.equals("testMethodThree") )
                    method.invoke(obj, 10, "Test String");
        }

    } // end - main()
} // end - test

class TestClass {

    public void testMethodOne()
    {
        System.out.println("testMethodOne is called");
    }
    public void testMethodTwo(int i)
    {
        System.out.println("testMethodTwo is called : " + Integer.toString(i) );
    }
    public void testMethodThree(int i, String t)
    {
        System.out.println("testMethodThree is called" + Integer.toString(i) + " " + t);
    }
    private void testMethodFour(int i, String t, String t2)
    {
        System.out.println("testMethodFour is called" + Integer.toString(i) + " " + t + t2);
    }

} // end - TestClass
