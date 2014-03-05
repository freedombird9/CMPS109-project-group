#include "ServerSocket.h"
#include "SocketException.h"
#include "hex.h"
#include <string>
#include <iostream>
using namespace std;


int main ( )
{
  int x, y; // coodinates for each move
  string servername;
  cout << "Please insert the server name" << endl;
  cin >> servername;

  try
    {
      // Create the socket
      ServerSocket server ( servername.c_str() );
      Hex game;
      while ( true )
	{

	  ServerSocket new_sock;
	  server.accept ( new_sock );

	  try
	    {
	      cout << "The game has been set up\n";
	      game.setBoard();
	      cout << game << endl;
	      while ( true )
		{
		  string data;
		  cout << "Now waiting for the user move" << endl;
		  // recieve data from client
		  new_sock >> data;
		  x = stoi(data);
		  new_sock >> data;
		  y = stoi(data);

		  cout << "what we recieve is "<<  x << y << endl;
		  game.move(x,y,Color::White);
		  cout << game << endl;

		  if (game.wins()) {
		    cout << "User (White) wins!" << endl;
		    break;
		  }

		  cout << "Now computer plays..." << endl;
		  game.AI(x, y, 20000);
		  game.move(x, y, Color::Black);

		  // sending the data back to client
                  new_sock << to_string(x);
		  new_sock << to_string(y);
		  cout << game << endl;
		  if (game.wins()) {
		    cout << "Computer (Black) wins!" << endl;
		    break;
		  }
		}
	    }
	  catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}
