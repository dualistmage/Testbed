///
/// @brief  This code will read last line of given file and print it.
/// @author Dohyun Yun
///

import java.io.*;


class test
{
    public static void main(String[] args) throws Exception {
        method1();
        method2();
    } // end - main()

    ///
    /// @brief Fast and memory usage is efficient, but multi-byte character is broken.
    ///
    private static void method1() throws Exception {
        try {
            File file = new File("test.txt");
            RandomAccessFile rfile = new RandomAccessFile(file,"r"); 
            long pos = file.length()-1; // pos will go to one ahead of EOF.
            int meetNewLine=0;
            while ( pos > 0 )
            {
                rfile.seek(pos);
                if ( rfile.readByte() == (byte)'\n' ) // only for linux
                {
                    meetNewLine++;
                    if ( meetNewLine == 2 ) break;
                }
                pos--;
            }
            pos++;
            rfile.seek(pos);
            String lastLine = rfile.readLine();

            System.out.println( "----------------------------------------------------" );
            System.out.println( "Last line string : " + lastLine );
            System.out.println( "----------------------------------------------------" );

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    ///
    /// @brief Safe for multi-byte character but it spends memory as much as the size of the file.
    ///
    private static void method2() throws Exception { 
        String lastLine = null;
        try {
            FileInputStream fpin = new FileInputStream( "test.txt" );
            BufferedReader br = new BufferedReader(new InputStreamReader(fpin) );

            String tmp;
            while( (tmp = br.readLine()) != null)
                lastLine = tmp;

            fpin.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println( "----------------------------------------------------" );
        System.out.println( "Last line string : " + lastLine );
        System.out.println( "----------------------------------------------------" );
    }
} // end - test
