import java.io.*;

public class Problem_C {
	
	public static void main(String[] args) throws IOException {
		
		if(args.length != 2) {
			System.out.println("Usage : Problem_C.sh <input_file> <output_file>");
			System.exit(1);
		}
		
		// Local Variables
		int i;
		int start, end, diff, steps;
		int prev_sum, sum, next_sum, sum_step_num;
		int testno;

		StringController SC = new StringController(args[0],args[1]);
		
		SC.setSeparator(" ");
		testno = Integer.parseInt(SC.getToken());
		
		while (testno > 0) {
			
			// Get the strings of data and pattern

			start = Integer.parseInt(SC.getToken());
			end = Integer.parseInt(SC.getToken());

			// Initialization
			diff = end - start;
			steps = 0;
			prev_sum = 0;
			sum = 0;
			next_sum = 1;
			sum_step_num = 0;

			while(diff != 0)
			{
				// Incresing sum step
				if (next_sum <= diff)
				{
					sum_step_num++;
					prev_sum = sum;
					sum = next_sum;
					next_sum = sum + sum_step_num + 1;
				}
				// Decresing sum step
				else if (sum > diff)
				{
					sum_step_num--;
					next_sum = sum;
					sum = prev_sum;
					prev_sum = sum - sum_step_num;
				}

				diff -= sum_step_num;
				steps++;
			}

			// Output the result
			SC.writeln(Integer.toString(steps));			
			
			testno--;
		} // end - while (testno > 0)

		SC.closeReader();
		SC.closeWriter();
	}

}
