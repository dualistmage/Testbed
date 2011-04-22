#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
	// Local number
	int i;
	int n, p, q, x;
	int iNumOfPrime;

	// Open Input file & get n
	if (argc != 3) {
		cout << "Usage : Problem_B <input> <output>" << endl;
		return -1;
	}

	ifstream fpin;
	fpin.open(argv[1]);
	if (!fpin.is_open()) {
		cout << "Input file cannot be opened" << endl;
		return -1;
	}
	fpin >> n;
	fpin.close();

	// Initialization
	char *S = new char[n + 1];
	for(i = 0; i <= n; i++) 
		S[i] = 0;	// 1 : deleted
	p = 2;
	iNumOfPrime = n-1;

	while(p*p <= n) {
		q = p;
		while(p*q <= n) {
			x = p * q;
			while(x <= n) {
				S[x] = 1;	// deleted
				iNumOfPrime--;
				x *= p;
			}
			
			while(1) {	// next q
				if (S[++q] == 0) break;
			}
		} // end - while(p*q <= n)
		while(1) {	// next q
				if (S[++p] == 0) break;
		}
	} // end - while(p*p <= n)


	// Record the result
	ofstream fpout;
	fpout.open(argv[2]);
	if (!fpout.is_open()) {
		cout << "Output file cannot be created" << endl;
		return -1;
	}
	fpout << iNumOfPrime;
	for(i=2; i<=n; i++) {
		
		if (S[i] == 0){
			fpout << endl;
			fpout << i;
		}
	}
	
	return 0;
}
