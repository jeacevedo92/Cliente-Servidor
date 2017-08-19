#include <iostream>
#include <fstream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <SFML/Audio.hpp>

#include <thread>

#include <queue>
#include <mutex>
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

void ControlPlaySongs(Music *music){
	while(true){
		//cout << "por fuera" << endl;
		while(music->getStatus() == SoundSource::Playing){
			//cout<< "cancion sonando !!!";
		}

    }
    cout << "Finished!" << endl;
}


void messageToFile(const message& msg, const string& fileName) {
	const void *data;
	msg.get(&data, 1);
	size_t size = msg.size(1);

	ofstream ofs(fileName, ios::binary);
	ofs.write((char*)data, size);
}

int main(int argc, char** argv) {
	cout << "This is the client\n";

	context ctx;
	socket s(ctx, socket_type::req);
	cout << "Connecting to tcp port 5555\n";
	s.connect("tcp://localhost:5555");

	Music music;

	thread t1(ControlPlaySongs,&music);

	//SafeQueue PlayList;

	queue<string> PlayList;


	while(true){
		cout << "\tMusic PLayer\t";
		cout << "\tIngrese Operacion a realizar:\t" <<endl;
		string operation;
		cin >> operation;

		//cout << "Sending  some work!\n";


		message m;
		//m << operation;
		
	
		//string operation(argv[1]);

		//if(argc == 3) {
		//	string file(argv[2]);
		//	m << file;
		//}

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
				//PlayList.enqueue(SongName);
				PlayList.push(SongName);
				} else if (operation == "list"){
					m << operation;
					}else if (operation == "exit"){
						cout << "Hasta pronto ...";
						return 0;
					}

		/*while(!PlayList.empty()){
			cout << PlayList.front()<<endl;
			PlayList.pop();
		}
		*/
		
		//PlayList.PrintQueue();

		string result;
		message answer;


		if (operation!="add"){

			s.send(m);
			
			s.receive(answer); //sincrono
			
			answer >> result;
			}else{
				result = "add";
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
			messageToFile(answer,"song.ogg");
			music.openFromFile("song.ogg");
			music.play();//asincrono					

			//int x;
			//cin >> x;
		} else if (result == "add"){
			cout <<"element add"<<endl;
			}else{
				cout << "Don't know what to do!!!" << endl;
			}

		//return 0;
	}
}
