import java.io.*;
import java.util.*;

public class Problem_C {

	public static void main(String[] args) throws IOException {

		if ( args.length != 2 )
		{
			System.out.println("Usage : Problem_C.sh <input_file> <output_file>");
			System.exit(1);
		} // end - if

		StringController SC = new StringController(args[0],args[1]);
		SC.setSeparator(" ");

		int i, j, k;
		int numOfPerson;
		int[]	weightList = new int[101];
		int[][] teams = new int[50500][101];

		int totalWeight, teamAverageWeight;
		int newWeight;
		int minWeight;
		int testno = Integer.parseInt( SC.getToken() ); 
		
		String result;

		while(testno > 0)
		{
			numOfPerson = Integer.parseInt( SC.getToken() ); 

			totalWeight = 0;
			minWeight = 500; // Set minWeight to the maximum weight

			// Get weight list of each person with finding minimum weight.
			for(i = 1; i <= numOfPerson; i++)
			{
				weightList[i] = Integer.parseInt( SC.getToken() ); 
				totalWeight += weightList[i];
				if (minWeight > weightList[i] )
					minWeight = weightList[i];
			} // end - for

			teamAverageWeight = totalWeight / 2;
	
			// Initialize array
			for(i = 0; i <= numOfPerson; i++)
			{
				for(j = 1; j <= teamAverageWeight; j++)
					teams[j][i] = 0;
				if ( teams[weightList[i]][0] == 0)
					teams[weightList[i]][0] = teams[weightList[i]][i] = 1;

			} // end - for

			// Dynamic Process to get the right value
			for(i = 1; i <= numOfPerson; i++)
			{
				for(j = 1; j <= teamAverageWeight; j++)
				{
					if(        (teams[j][0] > 0) 
							&& ((j+weightList[i]) <= teamAverageWeight) 
							&& (teams[j][i] == 0) )
					{

						// Solve the duplicated weight problem.
						if ( teams[j + weightList[i]][0] != 0 
							&& !((teams[j][0] == (numOfPerson/2 - 1)) && ((j + weightList[i] + minWeight) > teamAverageWeight) ) )
							continue;

						// Store the weight to array.
						newWeight = j + weightList[i];
						teams[newWeight][0] = teams[j][0] + 1;
						for(k = 1; k <= numOfPerson; k++)
							teams[newWeight][k] = teams[j][k];
						teams[newWeight][i]++;

					} // end - for
				} // end - for
			} // end - for

			int diff = 0;
			int minOptWeight;
			int aveNum1 = numOfPerson / 2;
			int aveNum2 = numOfPerson / 2 + numOfPerson % 2;

			minOptWeight = teamAverageWeight;

			while(true)
			{
				if ( ( teams[minOptWeight][0] == aveNum1 ) || ( teams[minOptWeight][0] == aveNum2 ) )
					break;
				else 
					minOptWeight--;
			} // end - while

			result = Integer.toString(minOptWeight) + " " + Integer.toString(totalWeight-minOptWeight);

			// write the result to the output file
			SC.writeln(result);			

			testno--;
		} // end - while

		SC.closeReader();
		SC.closeWriter();
	} // end - main()

} // end - class Problem_C

