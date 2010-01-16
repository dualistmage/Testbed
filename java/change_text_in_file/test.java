///
/// @brief  This code will change usecache="y" to "n" and display the text and recover it to the previous one.
/// @author Dohyun Yun
///

import java.io.*;

class test
{
    public static void main(String[] args) throws Exception {
        replace("test.xml", "usecache=\"y\"" , "usecache=\"n\"");
        printFile("test.xml");
        replace("test.xml", "usecache=\"n\"" , "usecache=\"y\"");
        printFile("test.xml");
    } // end - main()

    private static boolean replace(String filename, String from, String to) throws Exception {
        try {
            if ( from.length() != to.length() )
            {
                System.out.println("Lengthes are different.");
                return false;
            }

            File file = new File(filename);
            RandomAccessFile rfile = new RandomAccessFile(file,"rw"); 
            long pos = 0;
            long fileLen = file.length();
            int meetNewLine = 0;
            while ( pos < fileLen )
            {
                rfile.seek(pos);
                String line = rfile.readLine();
                if ( line.contains(from) )
                {
                    line = line.replace(from,to);
                    rfile.seek(pos);
                    rfile.writeBytes( line );
                    break;
                }
                pos = rfile.getFilePointer();
            }
            rfile.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return true;
    }

    private static void printFile(String filename) {
        try {
            File file = new File(filename);
            RandomAccessFile rfile = new RandomAccessFile(file, "rw");

            System.out.println("File : " + filename);
            System.out.println("-----------------------------------------------");
            String line;
            while( (line = rfile.readLine()) != null )
                System.out.println( line );
            System.out.println("-----------------------------------------------");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
} // end - test
