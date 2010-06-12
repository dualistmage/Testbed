///
/// @brief  This code will read utf8 code and print cp949 output.
/// @author Dohyun Yun
///

import java.io.*;


class test
{
    public static void main(String[] args) throws Exception {
        String lastLine = null;
        try {
            BufferedReader in = new BufferedReader(
                    new InputStreamReader(
                        new FileInputStream("test.utf8"), "UTF-8"
                        )
                    );
            BufferedWriter out = new BufferedWriter(
                    new OutputStreamWriter(
                        new FileOutputStream( "test.cp949" ) , "MS949" 
                        )
                    );

            /*
            String line = null;
            while( (line = in.readLine()) != null ) {
                out.write(line);
                out.newLine();
            }
                */
            out.write("임마 나다");
            in.close();
            out.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
} // end - test
