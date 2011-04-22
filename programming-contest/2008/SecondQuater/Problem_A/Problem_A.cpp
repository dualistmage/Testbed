#include <iostream>
#include <fstream>
#include <string>

#define MAX_STR		"~~~~~~~~~~~~~~"
#define MAX_STR2	"~~~~~~~~~~~~~~~"

using namespace std;

// Global variables
/////////////////////////////////////////////////////////////////////
struct node {
	string key;
	struct node* r;
	struct node* d;
};

node* head;
node* bottom;
node* tail;


// 1-2-3 Skip tree methods
/////////////////////////////////////////////////////////////////////
void Initialize() {
	head = new node;
	bottom = new node;
	tail = new node;

	head->key = MAX_STR;
	head->r = tail;
	head->d = bottom;

	tail->key = MAX_STR2;
	tail->r = tail;
	tail->d = NULL;

	bottom->r = bottom->d = bottom;
}
// ------------------------------------------------------- [ Search ]
node* Search(string v) {
	node * x;
	x = head;
	bottom->key = v;
	while(v != x->key) 
		x = (v.compare(x->key) > 0)? x->d : x->r;
	return x;
} // end - Search()

// ------------------------------------------------------- [ Insert ]
int Insert(string v) {
	node *t, *x;
	x = head;
	bottom->key = v;
	while ( x != bottom) {
		while (v.compare(x->key) > 0) x = x->r;	//	v > x->key
		if ((x->d == bottom) && (v == x->key))
			return 0;
		if ((x->d == bottom) || (x->key == x->d->r->r->r->key)) {
			t = new node;
			t->r = x->r;
			t->d = x->d->r->r;
			x->r = t;
			t->key = x->key;
			x->key = x->d->r->key;
		} // end - if ((x->d == bottom) || (x->key == x->d->r->r->r->key))
		x = x->d;
	}
	if (head->r != tail) {
		t = new node;
		t->d = head;
		t->r = tail;
		t->key = MAX_STR;	// Maximum value of string
		head = t;
	}	
	return 1;
} // end - Insert()

// ------------------------------------------------------- [ Insert ]
int Delete(string v) {
	int iGapNum;
	node *x, *b, *tmp, *uphead;	// b : before x

	b = head;
	x = head->r;
	
	while(x != bottom) {

		iGapNum = 1;

		// Find a gap where we want to drop
		while(v.compare(x->r->key) < 0) {
			b = x;
			x = x->r;
			iGapNum++;
		}
		if ((x->d == bottom) && (v == x->key))	// not found
			return 0;

		// If the gap is only one, lower the x and raise the first of them
		if (iGapNum == 1) {
			tmp = x;
			x = tmp->d;
			b->r = tmp->r;
			delete tmp;
			uphead = b;
		}
		
		// If the gap is at least two, 
		else {
			
		}
		
	}

	


	return 0;
}

// Main
/////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	string mode, str;

	// Open Input file & get n
	if (argc != 3) {
		cout << "Usage : Problem_B <input> <output>" << endl;
		return -1;
	}

    int check;
	ifstream fpin;
	fpin.open(argv[1]);
	if (!fpin.is_open()) {
		cout << "Input file cannot be opened" << endl;
		return -1;
	}
    ofstream fpout;
    fpout.open(argv[2]);

	
	while(1) {	

		fpin >> mode >> str;		
        check = 0;
        if (mode == "INS") {
            if (Insert(str) ==0 ) {
                fpout << "DUP" << " " << str << endl;
            }
            else {
                fpout << mode << " " << str << endl;
            }
        }
        else if (mode == "FIND") {
            if (Search(str) == bottom) {
                fpout << "NONE" << " " << str << endl;
            }
            else {
                fpout << mode << " " << str << endl; 
            }
        }
        else {
            Delete(str);
            fpout << mode << " " << str << endl;
        }

		if (fpin.eof()) break;
	}

    fpin.close();
    fpout.close();

	Initialize();

	return 0;
}

