// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"


ClientSocket::ClientSocket ( std::string host, int port )
{
  std::cout << "====================== client init ================\n";
  
  if( !serverinfo(host.c_str(), port))
    {
      throw SocketException ("Could not find the server info, unknown server. ");
    }
  
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}

 bool  ClientSocket::serverinfo(const char *name, int port) const
{
  // get DOT name and IP address of the server 
  std::cout << "Now get and show server host info" << "\n";
  hostent *hp = ::gethostbyname(name); 

  if( hp == 0)
    return false;
  std::cout << "Server name = " <<  hp->h_name << " IP = " <<  inet_ntoa(*(in_addr *)hp->h_addr) << " and PORT = " << port << "\n";

  return true;
}
