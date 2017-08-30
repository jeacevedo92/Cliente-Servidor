#include <iostream>
#include <fstream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <mutex>
#include <queue>

#include <condition_variable>

using namespace std;
using namespace zmqpp;
using namespace sf;


template <class T>
class SafeQueue
{
public:
  SafeQueue(void)
    : q()
    , m()
    , c()
  {}

  ~SafeQueue(void)
  {}

  // Add an element to the queue.
  void enqueue(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  bool Isempty(void){
  	//std::unique_lock<std::mutex> lock(m);
  	if (q.empty()){
  		return true;
  	}else return false;
  }

  // Get the "front"-element.
  // If the queue is empty, wait till a element is avaiable.
  T dequeue(void)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty())
    {
      // release lock as long as the wait and reaquire it afterwards.
      c.wait(lock);
    }
    T val = q.front();
    q.pop();
    return val;
  }

  void PrintQueue(){
  	std::unique_lock<std::mutex> lock(m);
  	if (q.front() == NULL){
  		cout<<"\nNothing to Display\n";
  		}
  		else{
  			while(q.front()!=NULL){
  				T val = q.front();
  				//siguiente 
  				cout << val;
  				}
  			}
  	}


private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};


void messageToFile(const message& msg, const string& fileName,const int Num) {
	int cont = 2;
	const void *data;
	size_t size;

	ofstream ofs(fileName, ios::binary);
	ofs.clear();

	while (cont <= (Num+2)){

		msg.get(&data, cont);
		size = msg.size(cont);
		ofs.write((char*)data, size);
		cont++;
	}
}

void ControlPlaySongs(Music *music, SafeQueue<string> *List){
	while(true){

		string song = List->dequeue();

		while(music->getStatus() == SoundSource::Playing){			
		}

		context ctx;
		socket s(ctx, socket_type::req);
		cout << "Connecting to tcp port 5555 since thread\n";
		s.connect("tcp://localhost:5555");

		message m;
		message answer;
		string result;

		m << "play";
		m << song;

		s.send(m);			
		s.receive(answer);

		answer >> result;
		string NumParts;
		int Num;

		answer >> NumParts;

		Num = stoi(NumParts);


		messageToFile(answer,"song.ogg",Num);
		music->openFromFile("song.ogg");
		music->play();//asincrono



    }
    cout << "Finished!" << endl;
}



int main(int argc, char** argv) {
	cout << "This is the client\n";

	context ctx;
	socket s(ctx, socket_type::req);
	cout << "Connecting to tcp port 5555\n";
	s.connect("tcp://localhost:5555");

	Music music;
	SafeQueue<string> PlayList;

	thread t1(ControlPlaySongs,&music,&PlayList);


	while(true){
		cout << "\t\t-------- Music PLayer --------\t\t"<<endl;
		cout << "\t* Ingrese Operacion a realizar:\t" <<endl;
		string operation;
		cin >> operation;

		message m;

		if (operation == "play"){

			string file;
			cout << "ingrese el nombre de la cancion" << endl; 
			cin >> file;
			m << operation;
			m << file;

			}else if (operation == "add"){
				string SongName;
				cout << "ingrese el nombre de la cancion a agregar: " << endl;
				cin >> SongName;
				PlayList.enqueue(SongName);
				cout << "Song Enqueue";
				} else if (operation == "list"){
					m << operation;
					}else if (operation == "next"){
							music.pause();
							cout << "Next song";
							
						}else if (operation == "exit")
						{
							cout << "Chaolin ";
							return 0;
						}

		string result;
		message answer;
		string NumParts;
		int Num;


		if (operation!="add" && operation!="next" ){

			s.send(m);
			cout << "envia";
			
			s.receive(answer); //sincrono
			cout << "recibe";
		
			answer >> result;

			if (operation == "play")
			{
				answer >> NumParts;
				Num = stoi(NumParts);
				cout << "numero de partes" <<Num<<endl ; 
				
			}
			
			
			}			

		if (result == "list") {
			size_t numSongs;
			answer >> numSongs;
			cout << "Available songs: " << numSongs << endl;
			for(int i = 0; i < numSongs; i++) {
				string s;
				answer >> s;
				cout << s << endl;
			}

		} else if (result == "file"){
			messageToFile(answer,"song.ogg",Num);
			music.openFromFile("song.ogg");
			music.play();//asincrono					

			//int x;
			//cin >> x;
		}
	}
}
