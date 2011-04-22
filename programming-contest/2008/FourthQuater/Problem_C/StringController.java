/**********************************************************************
 * String Controller Class - Made by Dohyun Yun 08.04.12
 * 
 */

import java.io.*;
import java.util.*;
import java.lang.String;

public class StringController {
	
// Member variables
///////////////////////////////////////////////////////////////////////
	
	protected BufferedReader	mReader;
	protected PrintWriter		mWriter;
	protected String			mSeparator = null;
	protected StringTokenizer 	mST;
	protected String			mTokenData;
	
// Member Functions
/////////////////////////////////////////////////////////////////////////
	
	// ------------------------------------- [ Constructor & Destructor ]
	public StringController() {}
	
	public StringController(String Input) throws IOException {
		
		mReader = new BufferedReader(new FileReader(new File(Input)));
		
	} // end - StringController(String Input)
	
	public StringController(String Input, String Output) throws IOException {
		
		mReader = 
			new BufferedReader(
			new FileReader(
			new File(Input)));
		mWriter = 
			new PrintWriter(
			new BufferedWriter(
			new FileWriter(
			new File(Output))));
		
	} // end - StringController(String Input, String Output)
	
	// ------------------------------------- [ Input functions ]
	public void	setReader(String Input) throws IOException {
		mReader = new BufferedReader(new FileReader(new File(Input)));		
	}
	public String 	getLine() throws IOException { // null is EOF
		mTokenData = mReader.readLine();		
		return mTokenData;
	}
	
	public void setSeparator(String szSeparator) {
		mSeparator = szSeparator;		
	}
	public String	getToken()throws IOException {
		while (true) {
			if (mST == null) {
				if (mTokenData == null) mTokenData = mReader.readLine();
				if (mSeparator == null) 
					mST = new StringTokenizer(mTokenData);
				else 
					mST = new StringTokenizer(mTokenData, mSeparator);
			}
		
			if (mST.hasMoreTokens()) {
				return mST.nextToken();
			}
			else {
				mTokenData = mReader.readLine();				
				if (mTokenData == null) return null; // EOF
				mST = new StringTokenizer(mTokenData, mSeparator);
			}
		} // end - while(1)
	}	
	public void closeReader() throws IOException {
		mReader.close();		
		mReader = null;
	}
	
	// ------------------------------------- [ Output functions ]
	public void setWriter(String Output) throws IOException {
		mWriter = 
			new PrintWriter(
			new BufferedWriter(
			new FileWriter(
			new File(Output))));
	}
	public void write(String str) throws IOException {
		mWriter.print(str);	
	}
	public void writeln(String str) throws IOException {
		mWriter.println(str);
	}
	public void closeWriter() throws IOException {
		mWriter.close();
		mWriter = null;
	}
}
