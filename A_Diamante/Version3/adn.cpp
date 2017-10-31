#include "../lib/sparseMat.hh"
#include "../lib/timer.hh"
//#include "../lib/thread_pool_Simple.hh"
#include "../lib/threadpool.hh"
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cmath>
#include <thread>

using namespace std;

class SparseAcumulator
{

public:
  vector<int> w;
  vector<bool> b;
  vector<int> ls;
  SparseAcumulator() {}
  ~SparseAcumulator() {}

  void set(int dimension){
    w.resize(dimension, numeric_limits<int>::max());
    b.resize(dimension, false);
  }

  void reset(){
    for (int i = 0; i < ls.size(); ++i)
    {
      w[ls[i]] = numeric_limits<int>::max();
      b[ls[i]] = false;
    }
    ls.clear();
  }

  void print(){
    for (int i = 0; i < w.size(); ++i)
    {
      if(w[i]==numeric_limits<int>::max())
        cout << "F - ";
      else
        cout << w[i] << " - ";
    }
    cout << endl;
  }

};

void scatterSPA(SparseAcumulator &SPA, int value, int pos){

 // cout <<"tamano de SPA.B: "<<SPA.b.size()<<endl;
 // cout <<"pos: "<<pos << endl;
  
  if(!SPA.b[pos]){   
    SPA.w[pos] = value;   
    SPA.b[pos] = true;    
    SPA.ls.push_back(pos);
  } else {
     
    SPA.w[pos] = min(SPA.w[pos], value);
    
  }
}

void gatherSPA(SparseAcumulator &SPA, sparseMat<int> &mr){
  for (int i = 0; i < SPA.ls.size(); ++i)
  {
    mr.col_ind.push_back(SPA.ls[i]);
    mr.val.push_back(SPA.w[SPA.ls[i]]);
  }
}

void compressVec(vector<int> &row_ptr){

  int cont = 0;
  
  std::vector<int> compressedRow;
  compressedRow.push_back(cont);

  cont++;

  for (int i = 1; i < row_ptr.size(); ++i){
    if(row_ptr[i] != cont){
      compressedRow.push_back(i);
      cont++;
    }
  }

  row_ptr.clear();
  row_ptr.resize(compressedRow.size());
  row_ptr = compressedRow;

}

void readGraphS(string fileName, sparseMat<int> &m) {
  ifstream infile(fileName);

  string line;
  while (getline(infile, line)) {
    istringstream iss(line);
    if (line[0] == 'p') {
      string s1, s2;
      int nodes;
      iss >> s1 >> s2 >> nodes;

      m.setDimension(nodes);

      // m.resize(nodes);
      // for (int i = 0; i < nodes; i++) {
      //   m[i].resize(nodes);
      // }
      // cout << "Graph with " << nodes << " nodes" << endl;
    } else if (line[0] == 'e') {
      char e;
      int row, col, weight;
      iss >> e >> row >> col >> weight;

      m.setValue(weight);
      m.setCol_ind(col);
      m.setRow_ptr(row);
    }
  }
}

void DiamondThread(sparseMat<int> &ma, sparseMat<int> &mb, int i, SparseAcumulator &SPA ){

   for (int k = ma.row_ptr[i]; k < ma.row_ptr[i+1]; ++k)
    {
      for (int j = mb.row_ptr[ma.col_ind[k]-1]; j < mb.row_ptr[ma.col_ind[k]]; ++j)
      {

     //pool.submit([&SPA, value, pos]() { scatterSPA(SPA, ma.val[k]+mb.val[j], mb.col_ind[j]); });
        scatterSPA(SPA, ma.val[k]+mb.val[j], mb.col_ind[j]);
      }
    }


}


void sparseADMult(sparseMat<int> &ma, sparseMat<int> &mb, sparseMat<int> &mr) {


  SparseAcumulator SPA;
  SPA.set(ma.getDimension());
  mr.row_ptr.push_back(0);


  thread_pool pool;


  for (int i = 0; i < ma.row_ptr.size()-1; ++i){
    
  cout<<"pueba3"<<endl;

    pool.submit([&ma, &mb, i, &SPA]() { DiamondThread(ma, mb,i,SPA); });

    gatherSPA(SPA, mr);
    mr.row_ptr.push_back(mr.col_ind.size());
    SPA.reset();
  }
}

void MUltLog(sparseMat<int> &Mat1, sparseMat<int> &MatR){

  int timesMult = ceil(log2(Mat1.getDimension()));
 

  for (int i = 0; i < timesMult; ++i)
  {  
    cout<<"pueba"<<endl;
    sparseADMult(Mat1, Mat1, MatR);
    cout<<"pueba1"<<endl;
    MatR.setDimension(Mat1.getDimension()); 

    Mat1.val.clear();
    Mat1.col_ind.clear();    
    Mat1.row_ptr.clear();

    Mat1.val.resize(MatR.val.size());
    Mat1.col_ind.resize(MatR.col_ind.size());
    Mat1.row_ptr.resize(MatR.row_ptr.size());  


    Mat1.val = MatR.val;    
    Mat1.dimension = MatR.dimension;
    Mat1.col_ind = MatR.col_ind;
    Mat1.row_ptr = MatR.row_ptr;

    MatR.val.clear();
    MatR.col_ind.clear();
    MatR.row_ptr.clear(); 

  }



}



int main(int argc, char const **argv)
{

  clock_t start, end;
  double time_used;


  sparseMat<int> Mat1, MatR;

  string fileName(argv[1]);

  readGraphS(fileName, Mat1);

  compressVec(Mat1.row_ptr);
  Mat1.row_ptr.push_back(Mat1.col_ind.size());

  //inicia reloj
  start = clock();

  MUltLog(Mat1,MatR);  

  //Se obtiene el tiempo final  
  end = clock();

  cout <<"tiempo de Ejecucion: ";
  time_used = ((double) (end - start)) /CLOCKS_PER_SEC;
  
  printf ("%0.10lf \n",time_used);

  Mat1.print1();
  cout<<endl<<endl;
  
}