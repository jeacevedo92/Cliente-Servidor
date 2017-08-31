#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <dirent.h>
#include <sstream>
#include <vector>
//#include <list>
//#include <filesystem>
//#include <experimental/filesystem> 


using namespace std;
using namespace zmqpp;
//namespace fs = std::filesystem;

vector<char> readFileToBytesSplit(const string& fileName,size_t ChunkSize, size_t pos) {


  ifstream ifs(fileName, ios::binary);
  //ifstream::pos_type pos = ifs.tellg();

  vector<char> result(ChunkSize);

  ifs.seekg(pos, ios::beg);
  ifs.read(result.data(), ChunkSize);

  return result;
}


size_t FileSize(const string& fileName) {


  ifstream ifs(fileName, ios::binary | ios::ate);
  ifstream::pos_type pos = ifs.tellg();


  return pos;
}

void BytesToFileSplit(const vector<char>& bytes, const string& fileName) {
  /*const void *data;
  msg.get(&data, 1);
  size_t size = msg.size(1);
  */

  ofstream ofs(fileName, ios::binary);
  ofs.write(bytes.data(), bytes.size());
}


vector<char> readFileToBytes(const string& fileName) {
  ifstream ifs(fileName, ios::binary | ios::ate);
  ifstream::pos_type pos = ifs.tellg();

  vector<char> result(pos);

  ifs.seekg(0, ios::beg);
  ifs.read(result.data(), pos);

  return result;
}

void fileToMesage(const string& fileName, message& msg) {
  vector<char> bytes = readFileToBytes(fileName);
  cout <<"el tamano del vector de las partes: " << bytes.size()<<endl;
  msg.add_raw(bytes.data(), bytes.size());
}
    

int main(int argc, char** argv) {

  //------declaration of variables

  context ctx;
  socket s(ctx, socket_type::rep);
  s.bind("tcp://*:5555");

  string path(argv[1]);

  string ruta = "/home/jhon/Cliente-Servidor/Proyect_1/player0.0/music/";
  string rutaSplits = "/home/jhon/Cliente-Servidor/Proyect_1/player0.0/music/Splits/";
  int ChunkSize = 1024000;

  //list<string> SongsList;
  unordered_map<string,string> songs;
  string aux;

  DIR *dir;
struct dirent *ent;
if ((dir = opendir ("/home/jhon/Cliente-Servidor/Proyect_1/player0.0/music/")) != NULL) {
  /* print all the files and directories within directory */
  while ((ent = readdir (dir)) != NULL) {
    //printf ("%s\n", ent->d_name);
    aux = path + ent->d_name;
    //printf("%s\n",aux );
    songs.insert({ent->d_name,aux});
  }
  closedir (dir);
} else {
  /* could not open directory */
  perror ("");
  return EXIT_FAILURE;
}
  cout << "Start serving requests!\n";
  while(true) {
    message m;
    s.receive(m);

    string op;
    m >> op;

    cout << "Action:  " << op << endl;
    if (op == "list") {  // Use case 1: Send the songs
      message n;
      n << "list" << songs.size();
      for(const auto& p : songs)
        n << p.first;
      s.send(n);
    } else if(op == "play") {
      // Use case 2: Send song file
      string chunkName;
      m >> chunkName;// esta es la cancion que entra por mensaje

      string archivo = ruta + chunkName;
      size_t Tamano = FileSize(archivo);
      float NumParts = Tamano/ChunkSize;

      cout <<"NumParts: " << NumParts << endl ;
     
      int count=0;
      int pos;

      message n;
      n << "file";
      n << to_string(NumParts);

      while (count <= NumParts){

        string fullChunkName;
        fullChunkName.clear();
        fullChunkName.append(chunkName);
        fullChunkName.pop_back();
        fullChunkName.pop_back();
        fullChunkName.pop_back();

        stringstream ss;
        ss << count;
        string myString = ss.str();

        fullChunkName.append(myString);
        pos = ChunkSize * count;

        if (pos > (Tamano-ChunkSize)){
          ChunkSize = (Tamano-pos);
        }

        cout << "pos: " << pos << "ChunkSize " << ChunkSize << "pos + ChunkSize: " << pos+ChunkSize<<endl<<endl;


        vector<char> v = readFileToBytesSplit(archivo,ChunkSize,pos);
        BytesToFileSplit(v,rutaSplits+fullChunkName);

        //send song parts

        cout << "sending song " << chunkName << "  part: " << count <<endl;

        cout << "fullChunkName  " << fullChunkName <<endl;
       
        fileToMesage(rutaSplits+fullChunkName, n);

        //cout << "ruta..." << rutaSplits+fullChunkName <<endl ;
       
        count ++;
      }

      s.send(n); // mensaje queda ["file, NumPArts, Song"]

      cout << "mensaje enviado .. "<<endl;


      /*
      cout << "sending song " << chunkName
           << " at " << songs[chunkName] << endl;
      message n;
      n << "file";
      fileToMesage(songs[chunkName], n);
      s.send(n);
      */
    } else {
      cout << "Invalid operation requested!!\n";
    }
  }

  cout << "Finished\n";
  return 0;
}
