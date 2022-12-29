#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <future>
#include <thread> 
#include <filesystem>
#include<dirent.h>
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
using namespace std;

int cnn = 1;
//void *sendMessage(string userInput, int sock);
//void *receiveMessage(char buffer[4096], int sock);
string yyy(){
   struct dirent *d;
   DIR *dr;
   dr = opendir(".");
   if(dr!=NULL)
   {
      string s = "";
      for(d=readdir(dr); d!=NULL; d=readdir(dr))
      {
         string t = "\n";
         string a = d->d_name+t;
         s = s + a;
      }
      closedir(dr);
      return s;
   }
   else
      cout<<"\nError Occurred!";
   cout<<endl;
   return 0;
}


void sendMessage(string userInput, int sock) {
    while(cnn == 1){
        if(cnn == 0){
            cout << "snd" << endl;
            break;
        }
        // input message to send
        cout << ">> ";
        getline(cin, userInput);

        // send message
        int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendResult == -1) {
            cout << "Could not send to server." << endl;
        }
        }
}


void receiveMessage(char buffer[4096], int sock) {
    while(cnn == 1){
    cout << cnn;
    int bytesReceived = recv(sock, buffer, 4096, 0);
    if(cnn == 0){
        cout << "rcv" << endl;
        break;
    }
    if (bytesReceived == -1) {
        cout << "There has been a server-issue." << endl;
    }
    else if (bytesReceived == 0) {
        cnn = 0;
        cout << cnn << endl;
        break;
        cout << "Server closed." << endl;
    }

    else {
        string receivedString = string(buffer, 0, bytesReceived);

        string checkEcho = receivedString.substr(0, 6);
        if (checkEcho.compare("<echo>") == 0) {
            ;
        }
        else {
            //cout << "<< " << string(buffer, bytesReceived) << endl;
            cout << "<< " << buffer << endl;
            if(strcmp(buffer,"/p")==0){
                char buff[FILENAME_MAX];
                GetCurrentDir( buff, FILENAME_MAX );
                string current_working_dir(buff);
                string a = "\n Current folder : \n  "+current_working_dir+"\n"+yyy();
                send(sock, a.c_str(), a.size() + 1, 0);
            }else if(strcmp(buffer,"/c")){
                ;
            }
        }    
    }
}}


int main() {
    
    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //fcntl(sock, F_SETFL, O_NONBLOCK);

    if (sock == -1) {
        return 1;
    }

    // port and server-ip
    int port = 54000;
    string ipAddress = "127.0.0.1";

    // create buffer for messages
    char buffer[4096];

    // ipv4 socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // try connect to server
    int connectResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectResult == -1) {
        cout << "Could not connect to server." << endl;
        return -1;
    }
    else {
        cout << "connected" << endl;
        //receiveMessage(buffer, sock);
    }

    string userInput;

    while (true) {
        
        if(cnn == 0){
            cout << "main" << endl;
            break;
        }
        // receive from server
        memset(buffer, 0, 4096);

        thread t1(sendMessage, userInput, sock);
        thread t2(receiveMessage, buffer, sock);
        //sendMessage(userInput, sock);
        //receiveMessage(buffer, sock);

        t1.join();
        t2.join();
        
    }

    // close socket
    close(sock);

    return 0;
}