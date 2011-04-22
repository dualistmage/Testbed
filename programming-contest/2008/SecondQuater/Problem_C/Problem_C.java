import java.io.*;

public class Problem_C {
	
	public static void main(String[] args) throws IOException {
		
		if(args.length != 2) {
			System.out.println("Usage : Problem_C.sh <input_file> <output_file>");
			System.exit(1);
		}
		
		// Local Variables
		int i, j;
		int testno;
		int startX, startZ;
		int lenX, lenZ;		
		String X, Z;		
		StringController SC = new StringController(args[0],args[1]);
		
		testno = Integer.parseInt(SC.getLine());
		
		while (testno > 0) {
			
			// Get the strings of data and pattern
			X = SC.getLine();	Z = SC.getLine();
			lenX = X.length();	lenZ = Z.length();
			startX = lenX - 1;	startZ = lenZ - 1;
			int[][] table = null;
			
			
			table = new int[lenZ][];
			
			for (i = 0; i < lenZ; i++) {
				table[i] = new int[lenX];
			}
			
			for(i = startZ; i >= 0; i--) {
				for(j = startX; j >= 0; j--) {					
					if ( Z.charAt(i) == X.charAt(j)) {
						if (i == startZ && j == startX) {
							table[i][j] ++;
						}
						else if (i == startZ) {
							table[i][j] = table[i][j+1] + 1;
						}
						else if (j == startX) {	}
						else {
							table[i][j] = table[i][j+1] + table[i+1][j+1];
						}						
					}
					else {
						if (j != startX) {
							table[i][j] = table[i][j+1];							
						}
					}
				}				
			}

			// Output the result
			SC.writeln(Integer.toString(table[0][0]));			
			
			testno--;
		} // end - while (testno > 0)
		
		SC.closeReader();
		SC.closeWriter();

	}

}
