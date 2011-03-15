import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Random;
import java.util.Set;
import java.util.Vector;
import java.util.Collections;

public class SCDGenerator
{

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		// TODO Auto-generated method stub
		String [] inputFileNames = 
        {
            "./B-00-201012171435-00004-I.C.SCD"
        };

        String [] outputFileNames = 
        {
            "./B-00-201012171435-00004-I.C.SCD-dd"
        };

        String ddProperty = "CONTENT";

        try
        {
            final int smBoundary = 500;
            final int mlBoundary = 3000;
            final int minDoc = 10;
            Vector<Integer> shortStr    = new Vector<Integer>();
            Vector<Integer> middleStr   = new Vector<Integer>();
            Vector<Integer> longStr     = new Vector<Integer>();

            for(int idx = 0; idx < inputFileNames.length; idx++)
            {
                Random rand = new Random(System.currentTimeMillis());

                Vector<String> SCDContent = new Vector<String>();

                // read file
                BufferedReader in = new BufferedReader(new FileReader(inputFileNames[idx]));
                String line;
                int docId = 0;
                int randSkip = rand.nextInt(1000);
                while ((line = in.readLine()) != null)
                {
                    if ( line.indexOf(ddProperty) != -1 )
                    {
                        SCDContent.add(line);
                        int len = line.length();
                        if ( len < smBoundary )
                            shortStr.add(docId);
                        else if ( len < mlBoundary )
                            middleStr.add(docId);
                        else
                            longStr.add(docId);
                        docId++;
                    }
                }
                in.close();

                // Check if each Str list has minDocs.
                if ( shortStr.size() < minDoc )
                {
                    System.out.println("Error - ShortStr Size is limited : " + shortStr.size() + "-" + middleStr.size() + "-" + longStr.size());
                    return;
                }
                if ( middleStr.size() < minDoc )
                {
                    System.out.println("Error - middleStr Size is limited : " + shortStr.size() + "-" + middleStr.size() + "-" + longStr.size());
                    return;
                }
                if ( longStr.size() < minDoc )
                {
                    System.out.println("Error - longStr Size is limited : " + shortStr.size() + "-" + middleStr.size() + "-" + longStr.size());
                    return;
                }


                // write results
                BufferedWriter out = new BufferedWriter(new FileWriter(outputFileNames[idx]));


                Collections.shuffle(shortStr);
                Collections.shuffle(middleStr);
                Collections.shuffle(longStr);

                for(int i = 0; i < 10; i++)
                {
                    generateDuplicatedCandidates(rand, out, shortStr.get(i), SCDContent, "Short");
                    generateDuplicatedCandidates(rand, out, middleStr.get(i), SCDContent, "Middle");
                    generateDuplicatedCandidates(rand, out, longStr.get(i), SCDContent, "Long");
                }

                out.close();

                System.out.println("Finished: " + inputFileNames[idx]);
            }
            ////////////////////////////////////////////////////////////////
        } catch (IOException e)
        {
            System.err.println(e);
            System.exit(1);
        }
    }

    static void generateDuplicatedCandidates(Random rand, BufferedWriter out, int kdx, Vector<String> SCDContent, String titleSuffix)
    {
        try {
        String [] randomString = "KT 내년 2월 15년 위성방송출력 기존 25% 향상된 올레 1호 위성 고화질(HD) 3차원(3D) 고품질 위성 방송 서비스 한반도전역 제공 올레 1호 위성발사 로켓분리 54분 07시21분 6천500km 상공 천이궤도 안정적진입 성공 10일 네 엔진점화 3만6천km 상공 정지궤도 안테나 태양전지판 위성중계기 경기도용인 KT위성관제센터 지상관제 테스트 2월초 본격적인 상용서비스 들어간다".split(" ");

        out.write(("<DOCID>" + kdx + "\n"));
        out.write(("<TITLE>original - " + titleSuffix + "\n"));
        //out.write(("<CONTENT>"));
        out.write(SCDContent.get(kdx));
        out.write("\n");

        String Doc = SCDContent.get(kdx);
        int docFront = 0;
        int docMiddle = docFront + Doc.length() / 3;
        int docRear = docMiddle + Doc.length() / 3;

        int docRand = rand.nextInt(Doc.length() / 3);

        // a word
        // modification
        {
            int index;

            docRand = rand.nextInt(Doc.length() / 3);

            // front part
            index = Doc.indexOf(" ", docFront + docRand);
            if(index == -1) index = docFront;
            {
                String randPart = randomString[rand.nextInt(randomString.length)];
                String resDoc = Doc.substring(0, index) + " " + randPart + " " + Doc.substring(index, Doc.length());
                out.write(("<DOCID>" + ((kdx + 1) * 100 + 1) + "\n"));
                out.write(("<TITLE>one word: front - " + titleSuffix + "\n"));
                //out.write(("<CONTENT>"));
                out.write(resDoc);
                out.write("\n");
            }

            docRand = rand.nextInt(Doc.length() / 3);

            // middle part
            index = Doc.indexOf(" ", docMiddle + docRand);
            if(index == -1) index = docMiddle;
            {
                String randPart = randomString[rand.nextInt(randomString.length)];
                String resDoc = Doc.substring(0, index) + " " + randPart + " " + Doc.substring(index, Doc.length());
                out.write(("<DOCID>" + ((kdx + 1) * 100 + 2) + "\n"));
                out.write(("<TITLE>one word: middle - " + titleSuffix + "\n"));
                //out.write(("<CONTENT>"));
                out.write(resDoc);
                out.write("\n");
            }

            docRand = rand.nextInt(Doc.length() / 3);

            // rear part
            index = Doc.indexOf(" ", docRear + docRand);
            if(index == -1) index = docRear;
            {
                String randPart = randomString[rand.nextInt(randomString.length)];
                String resDoc = Doc.substring(0, index) + " " + randPart + " " + Doc.substring(index, Doc.length());
                out.write(("<DOCID>" + ((kdx + 1) * 100 + 3) + "\n"));
                out.write(("<TITLE>one word: rear - " + titleSuffix + "\n"));
                //out.write(("<CONTENT>"));
                out.write(resDoc);
                out.write("\n");
            }
        }

        // two word
        // modification
        {
            int index;

            docRand = rand.nextInt(Doc.length() / 3);

            // front part
            index = Doc.indexOf(" ", docFront + docRand);
            if(index == -1) index = docFront;
            {
                String randPart = randomString[rand.nextInt(randomString.length)];
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                String resDoc = Doc.substring(0, index) + " " + randPart + " " + Doc.substring(index, Doc.length());
                out.write(("<DOCID>" + ((kdx + 1) * 100 + 4) + "\n"));
                out.write(("<TITLE>two word: front - " + titleSuffix + "\n"));
                //out.write(("<CONTENT>"));
                out.write(resDoc);
                out.write("\n");
            }

            docRand = rand.nextInt(Doc.length() / 3);

            // middle part
            index = Doc.indexOf(" ", docMiddle + docRand);
            if(index == -1) index = docMiddle;
            {
                String randPart = randomString[rand.nextInt(randomString.length)];
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                String resDoc = Doc.substring(0, index) + " " + randPart + " " + Doc.substring(index, Doc.length());
                out.write(("<DOCID>" + ((kdx + 1) * 100 + 5) + "\n"));
                out.write(("<TITLE>two word: middle - " + titleSuffix + "\n"));
                //out.write(("<CONTENT>"));
                out.write(resDoc);
                out.write("\n");
            }

            docRand = rand.nextInt(Doc.length() / 3);

            // rear part
            index = Doc.indexOf(" ", docRear + docRand);
            if(index == -1) index = docRear;
            {
                String randPart = randomString[rand.nextInt(randomString.length)];
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                String resDoc = Doc.substring(0, index) + " " + randPart + " " + Doc.substring(index, Doc.length());
                out.write(("<DOCID>" + ((kdx + 1) * 100 + 6) + "\n"));
                out.write(("<TITLE>two word: rear - " + titleSuffix + "\n"));
                //out.write(("<CONTENT>"));
                out.write(resDoc);
                out.write("\n");
            }
        }

        // eight words modification
        // modification
        {
            int index;

            docRand = rand.nextInt(Doc.length() / 3);

            // front part
            index = Doc.indexOf(" ", docFront + docRand);
            if(index == -1) index = docFront;
            {
                String randPart = randomString[rand.nextInt(randomString.length)];
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                String resDoc = Doc.substring(0, index) + " " + randPart + " " + Doc.substring(index, Doc.length());
                out.write(("<DOCID>" + ((kdx + 1) * 100 + 7) + "\n"));
                out.write(("<TITLE>eight word: front - " + titleSuffix + "\n"));
                //out.write(("<CONTENT>"));
                out.write(resDoc);
                out.write("\n");
            }

            docRand = rand.nextInt(Doc.length() / 3);

            // middle part
            index = Doc.indexOf(" ", docMiddle + docRand);
            if(index == -1) index = docMiddle;
            {
                String randPart = randomString[rand.nextInt(randomString.length)];
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                String resDoc = Doc.substring(0, index) + " " + randPart + " " + Doc.substring(index, Doc.length());
                out.write(("<DOCID>" + ((kdx + 1) * 100 + 8) + "\n"));
                out.write(("<TITLE>eight word: middle - " + titleSuffix + "\n"));
                //out.write(("<CONTENT>"));
                out.write(resDoc);
                out.write("\n");
            }

            docRand = rand.nextInt(Doc.length() / 3);

            // rear part
            index = Doc.indexOf(" ", docRear + docRand);
            if(index == -1) index = docRear;
            {
                String randPart = randomString[rand.nextInt(randomString.length)];
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);
                randPart += (" " + randomString[rand.nextInt(randomString.length)]);

                String resDoc = Doc.substring(0, index) + " " + randPart + " " + Doc.substring(index, Doc.length());
                out.write(("<DOCID>" + ((kdx + 1) * 100 + 9) + "\n"));
                out.write(("<TITLE>eight word: rear - " + titleSuffix + "\n"));
                //out.write(("<CONTENT>"));
                out.write(resDoc);
                out.write("\n");
            }
        }

        } catch (IOException e) {
            System.err.println(e);
            System.exit(1);
        }
    }


}
