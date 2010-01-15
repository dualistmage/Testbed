///
/// @brief  This code will read last line of given file and print it.
/// @author Dohyun Yun
///

import java.io.File;
import java.io.RandomAccessFile;


class test
{
    public static void main(String[] args) throws Exception {
        try {
            File file = new File("test.txt");
            RandomAccessFile rfile = new RandomAccessFile(file,"r"); 
            long pos = file.length(); // pos will go to the end of file.
            int meetNewLine=0;
            while ( pos > 0 )
            {
                byte curByte = rfile.readByte();
                if ( curByte == (byte)'\n' ) // only for linux
                {
                    meetNewLine++;
                    if ( meetNewLine == 2 ) break;
                }
                //System.out.println("--------------------------------------------");
                //System.out.println("Cur Byte    : " + Byte.toString( curByte ) );
                //System.out.println("Pos         : " + Long.toString( pos ) );
                //System.out.println("meetNewLine : " + meetNewLine );
                pos--;
                rfile.seek(pos);
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
    } // end - main()

} // end - test
