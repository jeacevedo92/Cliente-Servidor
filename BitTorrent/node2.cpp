#include <iostream>
#include <fstream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <utility>
#include <chrono> 
#include <thread>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <random>  

using namespace std;
using namespace zmqpp;

class Node {
    private:
        string ip;
        string ip_successor;    

        int id;
        int id_successor;

        pair <string,string> keys;

    public:



        //ip

        string getIp(){
            return ip;
        }


        void setIp(string n){
            ip = n;
        }

        //ip_successor

        string getIp_successor(){
            return ip_successor;
        }


        void setIp_successor(string n){
            ip_successor = n;
        }

        ///id

        int getId(){
            return id;
        }


        void setId(int n){
            id = n;
        }


        //id_successor   
        
        int getId_successor(){
            return id_successor;
        }


        void setId_successor(int n){
            id_successor = n;
        }


        //keys
        string getInferiorLimit(){
            return keys.first;
        }

        string getSuperiorLimit(){
            return keys.second;
        }


        void setSuperiorLimit(string n){
            keys.second = n;
        }

        void setInferiorLimit(string n){
            keys.first = n;
        }
    };


void Server(){

    cout << "servidor !!!"<<endl;
    context ctx_Server;
    socket s_server(ctx_Server, socket_type::rep);
    s_server.bind("tcp://*:5552");


    cout << "Start serving requests!\n";

    while(true){

        message m;
        s_server.receive(m);

        string op;
        m >> op;

        cout << "Action:  " << op << endl;
        
        if(op == "play") {

            message n;
            n << "play" ;
            s_server.send(n);  
        }

    }
} 


int generator_uniform_int_distribution(int SpaceKeys){

    default_random_engine generator;
    uniform_int_distribution<int> distribution(0,SpaceKeys);
    int number = distribution(generator);

    return number;

}

int main(int argc, char** argv) {

    int SpaceKeys = 100;    
    
	cout << "This is the node\n";
    Node node;

    //se lanza el hilo servidor
    thread t1(Server);


    string TypeNode(argv[1]);

    if (TypeNode == "FN"){

        node.setIp("tcp://localhost");
        node.setId(generator_uniform_int_distribution(SpaceKeys));
    }
    else if (TypeNode == "NN"){

        node.setIp("tcp://localhost");
        node.setId(generator_uniform_int_distribution(SpaceKeys));
        
    }


    //client
    context ctx_Client;    
    socket s_client(ctx_Client, socket_type::req);

    cout << "Connecting to tcp port 5551\n";
    s_client.connect("tcp://localhost:5551");

    while(true) {
        
        cout << "\t Bit Torrent\t";
        cout << "\t Enter operation to perform:\t" <<endl;

        string operation;
        cin >> operation;

        message m;
        message answer;
        string result;

        m << operation;
            
        s_client.send(m);
        s_client.receive(answer);
        answer >> result;
        cout <<"result: "<< result << endl;
    }
	
}

