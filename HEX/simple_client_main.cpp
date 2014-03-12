#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include "hex.h"
#include <unistd.h>

//#define _DEBUG_
using namespace std;

int main ( )
{
  try
    {
      // provide the server you want to connect
      cout << "Please input the server name" << endl;
      string Hostname;
      cin >> Hostname;

      cout << "Please insert the port number of Server" << endl;
      int portNumber;
      cin >> portNumber;
      ClientSocket client_socket (Hostname, portNumber );
     
      cout << " Server has been connected, now set up the game board... " << endl;
      try
	{
	  // The game is now set up 
	   int x, y;
	   int  size, difficulty;
	   cout << endl;
	   cout << "============================= HEX Game ===========================" << endl;
	   cout << endl;
	   cout << "Instruction for the difficulty level:" << endl;
	   cout << "for board size smaller than or equal to 7*7, difficulty = 3000 will make it hard to win the computer" << endl;
	   cout << "for board size 9*9, you may need difficulty = 6000+ to make your opponent smart enough" << endl;
	   cout << "for board size 11*11, you'll need at least 12000 as difficulty" << endl;
	   cout << "Please input the board size (from 4 to 11)" << endl;
	   cin >> size;
	   cout << "Please input difficulty" << endl;
	   cin >> difficulty;
	   Hex game(size);
	   
	   string data;
	   game.setBoard();
	    	 
	   cout << game << endl;
	   
	   cout << "Sending board info to server..." << endl;
	   client_socket << to_string(size);
	   sleep(2);
	   client_socket << to_string(difficulty);

	   cout << "Game board setup complete!" << endl;
	   cout << "Here is the rule:\n";
	   cout << "Computer will play white from left to right, user will play black up to down.\n";

	    while(true){
	    cout << "Now it is your turn, pick up a location (x, y):" << endl;
	    
	    while (true){
	      cin >> x >> y;
	      if(game.move(x, y, Color::White))   // white moves first
		break;
	      else cout << "illegal movement, please try agagin" << endl;
	    }
	     	     	     
	    cout << game << endl;
	    // convert to string then send to the socket
	    client_socket << to_string(x);
	    
	    sleep(2);
	    
	    client_socket << to_string(y);

	    if (game.wins()) {
	      cout << "Congratulations! You (White) wins!" << endl;
	      break;
	    }
	    
	    cout << "Now waiting for the computer response..." << endl;

	    client_socket >> data;
	    x = stoi(data);
	    client_socket >> data;
	    y = stoi(data);

	    cout << "Now computer moves to x = " << x << " y = " << y << endl;
	    game.move(x, y, Color::Black);
	    cout << game << endl;
	    if (game.wins()) {
	      cout << "Ohh... You lose, Computer (Black)  wins!" << endl;
	      break;
	    }
	    }
            #ifdef _DEBUG_ 	      
	    while (true){

	      string data;
	      cout << "Now it is your turn, pick up a location (x, y):" << endl;

	      while (true){
	  	cin >> x >> y;
	  	if(game.move(x, y, Color::White))   // white moves first
	  	  break;
	  	else cout << "illegal movement, please try agagin" << endl;
	      }
	     	     	     
	      cout << game << endl;
	      // convert to string then send to the socket
	      client_socket << to_string(x);

	      sleep(2);
	      client_socket << to_string(y);

	      if (game.wins()) {
	  	cout << "Congratulations! You (White) wins!" << endl;
	  	break;
	      }
	      
	      cout << "Now waiting for the computer response..." << endl;

	      while(data.empty()){
	  	client_socket >> data;
	      }
	      x = stoi(data);
	     
	      data.erase(data.begin(), data.end());
	      while(data.empty()){
	      client_socket >> data;
	      }
	      y = stoi(data);

	      cout << "Now computer moves to x = " << x << " y = " << y << endl;
	      game.move(x, y, Color::Black);
	      cout << game << endl;
	      if (game.wins()) {
	  	cout << "Ohh... You lose, Computer (Black)  wins!" << endl;
	  	break;
	      }
	    }
	    #endif
	    

	}
      catch ( SocketException& ) {}
      
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return 0;
}
