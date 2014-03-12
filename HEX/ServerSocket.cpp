// Implementation of the ServerSocket class
#include "ServerSocket.h"
#include "SocketException.h"

ServerSocket::ServerSocket ( const char *hostname )
{
  std::cout << "===================== server init =======================" << "\n";
  
  if( !hostinfo(hostname))
    {
      throw SocketException ( "Could not find host info, unknown host." );
    }
  
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }

}

ServerSocket::~ServerSocket()
{
}


const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}


const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}

bool  ServerSocket::hostinfo(const char *name) const
{
  // get DOT name and IP address of the host
  std::cout << "Now get and show server host info" << "\n";
  hostent *hp = ::gethostbyname(name); 

  if( hp == 0)
    return false;
  std::cout << "Hostname = " <<  hp->h_name << " IP = " <<  inet_ntoa(*(in_addr *)hp->h_addr) << "\n";

  return true;
}

void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Could not accept socket." );
    }
}

