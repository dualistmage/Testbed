///
/// @brief  This code will copy test to test.prev and add sentence "hello" at the end of test file
/// @author Dohyun Yun
/// @email  dualistmage@nate.com
///

import java.io.*;

class test
{
    public static void main(String[] args) throws Exception {
        String sourceName = new String("test");
        String backupName = new String("test.prev");
        File source = new File(sourceName);
        File backup = new File(backupName);

        if ( source.exists() )
        { // Copy test -> test.prev
            System.out.println("Copy test to test.prev");
            copyFile( source , backup );
        }

        printFile( sourceName );
        addStringToTail( source, "hello\nI am the test text" );
        printFile( sourceName );
        copyFile( backup , source );
        printFile( sourceName );

    } // end - main()

    private static void addStringToTail(File file, String str) {
        try {
            RandomAccessFile rFile = new RandomAccessFile( file, "rw" );
            rFile.seek( file.length() );
            rFile.writeBytes( str );
            rFile.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
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

    private static void copyFile(File from, File to) {
        try {
        InputStream in = new FileInputStream( from );
        OutputStream out = new FileOutputStream( to );
        byte[] buf = new byte[1024];
        int len;
        while(( len = in.read(buf)) > 0)
            out.write(buf,0,len);
        in.close();
        out.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
} // end - test
