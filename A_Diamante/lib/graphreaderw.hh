#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> readGraph(string fileName) {
  vector<vector<int>> m;
  ifstream infile(fileName);

  string line;
  while (getline(infile, line)) {
    istringstream iss(line);
    if (line[0] == 'p') {
      string s1, s2;
      int nodes;
      iss >> s1 >> s2 >> nodes;
      m.resize(nodes);
      for (int i = 0; i < nodes; i++) {
        m[i].resize(nodes);
      }
      cout << "Graph with " << nodes << " nodes" << endl;
    } else if (line[0] == 'e') {
      char e;
      int s, t, w;
      iss >> e >> s >> t >> w;
      m[s - 1][t - 1] = w;
      m[t - 1][s - 1] = w;
    }
  }
  return m;
}