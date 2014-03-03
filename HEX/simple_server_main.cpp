#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <iostream>

int main ( int argc, char* argv[] )
{
  std::cout << "running....\n";
  if(argc < 2)
    {
      std::cout << "Needs to input the server name\n";
      exit(1);
    }

  try
    {
      // Create the socket
      ServerSocket server ( argv[1] );

      while ( true )
	{

	  ServerSocket new_sock;
	  server.accept ( new_sock );

	  try
	    {
	      while ( true )
		{
		  std::string data;
		  new_sock >> data;
		  new_sock << data;
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
