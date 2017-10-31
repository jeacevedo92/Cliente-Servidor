#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include <cassert>

#include "quicksort.hh"

using namespace std;


void prepareFile(string fileName, vector<int> &rows, vector<int> &cols, vector<int> &nodes) {
  ifstream infile(fileName);

  string line;
  while (getline(infile, line)) {

    istringstream iss(line);
    if (line[0] == 'p') {
      string s1, s2;
      int nodes;
      iss >> s1 >> s2 >> nodes;

      cout <<"p edge " << nodes << " ";

    } else if (line[0] == 'a') {
      char e;
      int row, col, weight;
      iss >> e >> row >> col >> weight;

      rows.push_back(row); 
      cols.push_back(col); 
      nodes.push_back(weight); 
    }
  }
  cout << rows.size()<<endl;

  quicksort(rows,cols,nodes, 0, rows.size() - 1);

  for (int i = 0; i < rows.size(); ++i){
     cout<<"e "<< rows[i]<< " "<<cols[i] << " " << nodes[i]<<endl;
  }

}

int main(int argc, char **argv)
{

  vector<int> rows,cols,nodes;

  string fileName(argv[1]);
  prepareFile(fileName, rows,cols,nodes);

  
}
