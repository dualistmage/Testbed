#include <YString.h>
#include <DArray.h>



#include <math.h>

#include <iostream>
#include <fstream>
#include <algorithm>


using namespace std;

static int edit[200][200];

class AlignedPair {
public:
  AlignedPair() : i('*'), o('*') {}
  char i;
  char o;

  bool is_insertion() const { return i == '-'; }
  bool is_deletion() const { return o == '-'; }
  bool is_substitution() const { return i != '-' && o != '-'; }
};

class ED {
public:
  ED() : distance(-1) {}
  YString in;
  YString out;
  int distance;
};


int edit_distance(const char* x, size_t m, const char* y, size_t n)
{
  size_t i, j;
  for( i = 0; i <= m; i++ ) edit[i][0] = i;
  for( j = 1; j <= n; j++ ) edit[0][j] = j;

  for( i = 1; i <= m; i++ )
    {
      for( j = 1; j <= n; j++ )
        {
	  edit[i][j] = min(min(edit[i-1][j]+1, edit[i][j-1]+1), edit[i-1][j-1]+(x[i-1]!=y[j-1]));
        }
    }

  return edit[m][n];
}



void process_input(const YString& filename, DynamicArray<ED>& input)
{
  YString in;
  in.read_from_file(filename);

  DynamicArray<YString> tokens;
  in.make_tokens(" \n", tokens, true);

  for (int i = 0; i < tokens.size(); i+=2) {
    int idx = input.size();
    input[idx].in = tokens[i];
    input[idx].out = tokens[i+1];
    input[idx].distance =
      edit_distance(input[idx].in.as_const_char(), input[idx].in.size(), 
		    input[idx].out.as_const_char(), input[idx].out.size());
  }
}



void process_output(const YString& filename, DynamicArray<DynamicArray<AlignedPair> >& aps)
{
  YString input;
  input.read_from_file(filename);

  DynamicArray<YString> tokens;
  input.make_tokens('\n', tokens, true);

  int cnt = 0;
  for (int i = 0; i < tokens.size(); i+=2) {
    AlignedPair t;
    DynamicArray<AlignedPair>& ap = aps[cnt++];

    const YString& in = tokens[i];
    const YString& out = tokens[i+1];

    if (in.size() != out.size()) {
      cout << "Aligned Pair mismatch: " << in.size() << "<->" << out.size() << endl;
      cout << in << endl << out << endl;
      exit(1);
    }

    for (size_t k = 0; k < in.size(); k++) {
      if (in[k] == ' ')
	continue;
      else {
	ap[ap.size()].i = in[k];
	ap[ap.high()].o = out[k];
      }
    }
  }
}


int main(int argc, char **argv)
{
  if (argc < 3) {
    cout << "Usage:: evalb <input> <output>" << endl;
    return 0;
  }


  DynamicArray<ED> input;
  process_input(argv[1], input);

  DynamicArray<DynamicArray<AlignedPair> > aps;
  process_output(argv[2], aps);
    
  int mismatch = 0;
  int nonoptimal = 0;
  int num = input.size();
  // for each AP, apply it to in string to make it to out string.
  for (int i = 0; i < num; i++) {

    // temporary string in array format
    const ED& ed = input[i];
    Array<char> temp(max(ed.in.size(), ed.out.size()));
    int idx = 0;
    int cost = 0;
    // now apply the aligned pair to temp
    const DynamicArray<AlignedPair>& ap = aps[i];
    for (int j = 0; j < ap.size(); j++) {
      if (ap[j].is_insertion()) {
	temp[idx++] = ap[j].o;
	cost++;
      }
      else if (ap[j].is_deletion())
	cost++; // deletion means no processing
      else {
	if (ap[j].i != ap[j].o)
	  cost++;
	temp[idx++] = ap[j].o;
      }
    } 

    // now confirm if this is the same as out.
    for (int k = 0; k < idx; k++) {
      if (temp[k] != ed.out[k]) {
	cout << "Mismatch on input: " << ed.in << " : " << ed.out << endl;
	cout << "            at [" << k << "] --> ("  << temp[k] << ":" << ed.out[k] << ")" << endl;
	mismatch++;
	break;
      }
    }
    if (cost != ed.distance) {
      cout << "Not an optimal path: " << cost << " <--> " << ed.distance << endl;
      nonoptimal++;
    }
  }

  if (mismatch > 0) 
    cout << "On " << num << " cases " << mismatch << " failures! " << endl;
  else if (nonoptimal > 0)
    cout << "On " << num << " cases " << nonoptimal << " non-optimal paths found!" << endl;
  else
    cout << "Success!" << endl;

  return 0;
}



