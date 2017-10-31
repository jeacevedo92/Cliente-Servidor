#include "../lib/sparseMat.hh"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

void readGraphS(string fileName, sparseMat<int> *m) {
  ifstream infile(fileName);

  string line;
  while (getline(infile, line)) {
    istringstream iss(line);
    if (line[0] == 'p') {
      string s1, s2;
      int nodes;
      iss >> s1 >> s2 >> nodes;

      m->setDimension(nodes);

    } else if (line[0] == 'e') {
      char e;
      int row, col, weight;
      iss >> e >> row >> col >> weight;

      m->setValue(weight);
      m->setCol_ind(col);
      m->setRow_ptr(row);

    }
  }
}


vector<int> sparseADMult(sparseMat<int> &ma, sparseMat<int> &mb ) {

  vector<int> v;
  vector<int> d;

  d.push_back(0);
  d.push_back(2);
  // d.push_back(numeric_limits<int>::max());
  d.push_back(3);
  d.push_back(1); 
  // d.push_back(numeric_limits<int>::max());
  // d.push_back(numeric_limits<int>::max());
  // d.push_back(numeric_limits<int>::max());


  int mini = numeric_limits<int>::max();

  cout << "row_ptr.size(): "<< ma.row_ptr.size()<<endl;

  for (int i = 0; i < ma.row_ptr.size(); ++i){
    for (int k = ma.row_ptr[i]; k < ma.row_ptr[i+1]; k++){
        for (int l = 0; l < ma.row_ptr.size(); ++l){
          for (int m = ma.row_ptr[l]; m < ma.row_ptr[l+1]; m++){
            if (ma.col_ind[k]==ma.col_ind[m]){

              cout << ma.val[k] << " + " << ma.val[m] << endl;
            }
            else{
              
            }

    //   mini = min(mini, d[k] + ma.val[ma.col_ind[k]-1]);
    //   cout << ma.val[k] << " + " << d[ma.col_ind[k]-1] << " = " << ma.val[k] + d[ma.col_ind[k]] << endl;
    }
    cout<<endl;
    // v.push_back(mini);
    // mini = numeric_limits<int>::max();
  }
}
}

 
  return v;
}


vector<int> sparseADMult2(sparseMat<int> &ma, sparseMat<int> &mb ) {

  vector<int> v;
  int mini = numeric_limits<int>::max();

  cout << "row_ptr.size(): "<< ma.row_ptr.size()<<endl;

  for (int i = 0; i < ma.row_ptr.size() ; ++i)
  {
    cout << ma.row_ptr[i]<<endl;
  }


/*  cout << "row_ptr.size(): "<< ma.row_ptr.size()<<endl;
  cout << "row_ptr.size(): "<< ma.row_ptr[7] <<endl;
  cout << "row_ptr.size(): "<< ma.row_ptr[8] <<endl;
  cout << "row_ptr.size(): "<< ma.row_ptr[9] <<endl;*/

  for (int i = 0; i < ma.row_ptr.size(); ++i){
    for (int k = ma.row_ptr[i]; k < ma.row_ptr[i+1]; k++){
      for (int j = mb.row_ptr[ma.col_ind[k]-1]; j < mb.row_ptr[ma.col_ind[k]]; j++){

        mini = min(mini, ma.val[k] + mb.val[j]);
        cout << ma.val[k] << " + " << mb.val[j] << endl;
      }
      cout <<endl;
      v.push_back(mini);
      mini = numeric_limits<int>::max();
    }
    cout <<endl<<endl;
  }

  return v;
}


int main(int argc, char **argv){

	sparseMat<int> Mat1;
  //sparseMat<int> MatR;
  vector<int> rowsCompressed;
  vector<int> resultado;

  vector<int> weight = Mat1.GetVectorval();

  string fileName(argv[1]);
  readGraphS(fileName, &Mat1);

  int nodes = Mat1.getDimension();
  int arcs = Mat1.GetVectorRow().size();
  int cont = 0;
  
  rowsCompressed.push_back(cont);
  cont++;

  for (int i = 1; i < arcs; ++i){
    if(Mat1.getRow_ptr(i) != cont){
      rowsCompressed.push_back(i);
      cont++;
    }
  }

  Mat1.row_ptr.clear();
  //Mat1.row_ptr.erase(Mat1.row_ptr.begin(),Mat1.row_ptr.end());
  Mat1.row_ptr.resize(rowsCompressed.size());
  Mat1.row_ptr = rowsCompressed;

  Mat1.row_ptr.push_back(Mat1.col_ind.size());

  /*cout << "tamano del comprimido: " <<rowsCompressed.size();
  cout << "tamano del comprimido: " <<rowsCompressed[9];
  cout << "tamano del comprimido: " <<rowsCompressed[8];*/

  /*for (int i = 0; i < Mat1.row_ptr.size() ; ++i){

    cout <<"vector comprimido " << endl;
    cout <<Mat1.row_ptr[i]<<endl;
  }*/

  cout <<"vector resultado " << endl;

  //resultado = sparseADMult(Mat1, Mat1);
  resultado = sparseADMult2(Mat1, Mat1);

  cout << "tamaño resultado: " << resultado.size()<<endl; 

  for (int i = 0; i < resultado.size() ; ++i){

    cout << resultado[i]<<endl;
  }

}
