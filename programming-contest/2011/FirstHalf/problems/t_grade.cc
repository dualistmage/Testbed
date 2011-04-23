#include <set>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;


/********************************************************************************
  Description: read the entire file into file_string
  Commenets  :
********************************************************************************/
void read_from_file(const string& inputfile, string& file_string)
{
  long size;
  ifstream file (inputfile.c_str(), ios::in|ios::binary|ios::ate);
  if (!file.good()) {
    cerr << "file not good: " << inputfile << endl;
    abort();
  }

  size = file.tellg();
  file.seekg (0, ios::beg);
  char* buffer = new char [size+1];
  file.read (buffer, size);
  buffer[size] = (char)NULL;
  file.close();
  
  file_string = buffer;
  delete [] buffer;
}


/********************************************************************************
  Description: make tokens using the delimiter character and put them into tokens 
  Commenets  :
********************************************************************************/
void make_tokens(const string& str, char delimiter, vector<string>& tokens)
{
  int k = 0; 
  size_t i; 
  for (i = 0; i < str.size(); i++) 
    if (str[i] == delimiter) { 
      if (k == 0) 
	continue; 
      tokens.push_back(str.substr(i-k, k));
      k = 0; 
    } 
    else  
      k++; 
 
  if (k != 0)
    tokens.push_back(str.substr(i - k, k));
}


/********************************************************************************
  Description:
  Commenets  :
********************************************************************************/
void build_map_from_input_file(const string& inputfile, map<int, set<int> >& output_map)
{
  string file_string;
  read_from_file(inputfile, file_string);

  vector<string> lines;
  make_tokens(file_string, '\n', lines);

  for (vector<string>::const_iterator lpos = lines.begin(); lpos != lines.end(); lpos++) {
    vector<string> tokens;
    make_tokens(*lpos, '\t', tokens);
    bool first = true;
    int first_key = 0;
    for (vector<string>::const_iterator tpos = tokens.begin(); tpos != tokens.end(); tpos++) {
      const string& tok = *tpos;
      if (first) {
	set<int> empty_set;
	first_key = atoi(tok.c_str());
	output_map.insert(pair<int, set<int> >(first_key, empty_set));
	first = false;
      }
      else 
	output_map[first_key].insert(atoi(tok.c_str()));
    }
  }
}



/********************************************************************************
  Description:
  Commenets  :
********************************************************************************/
int main(int argc, char** argv)
{
  if (argc != 3) {
    cout << "Usage: t_grade <your_output> <standard_output>" << endl;
    return 0;
  }


  map<int, set<int> > your_output_map, standard_output_map;

  build_map_from_input_file(argv[1], your_output_map);
  build_map_from_input_file(argv[2], standard_output_map);


  int num_errors = 0;
  int num_total = 0;
  for (map<int, set<int> >::const_iterator pos = standard_output_map.begin();
       pos != standard_output_map.end();
       pos++) {
    int key = pos->first;
    const set<int>& standard_set = pos->second;
    num_total += standard_set.size();

    map<int, set<int> >::const_iterator it = your_output_map.find(key);
    if (it == your_output_map.end()) // no such key
      num_errors += standard_set.size();
    else { // key exists.
      const set<int>& your_set = it->second;

      set<int> result;
      set_symmetric_difference(your_set.begin(), your_set.end(),
			       standard_set.begin(), standard_set.end(),
			       inserter<set<int> >(result, result.end())); 
      num_errors += result.size();
      num_total += your_set.size();
    }
  }

  cout << "Num Errors: " << num_errors << endl;
  cout << "Total Entries: " << num_total << endl;
  double v = (double)(num_total - num_errors)/(double)num_total;

  cout << "Your Percentage: " << setw(5) << setprecision(5) << v  << "%" << endl;
  if (v >= 90)
    cout << "Great Job!" << endl;
  else if ( v >= 70)
    cout << "Good Job!" << endl;
  else 
    cout << "Please note that your solution will not be eligible for performance test!" << endl;

  return 0;
}
