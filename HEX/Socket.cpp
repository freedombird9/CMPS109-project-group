// Implementation of the Socket class.
#include "Socket.h"
#include <cstring>
#include <cerrno>
#include <fcntl.h>

// Base class Socket 
Socket::Socket() :
  m_sock ( -1 )
{

  memset ( &m_addr,
	   0,
	   sizeof ( m_addr ) );
}

// destructor to call global function close
Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}

// The creation of socket
bool Socket::create()
{
  std::cout << "Creat a socket ...\n"; 
  m_sock = socket ( AF_INET,
		    SOCK_STREAM,
		    0 );

  // Check if socket creation succeeds
  if ( ! is_valid() ){
    return false;
  }

  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof ( on ) ) == -1 )
    return false;
  
  std::cout << "\n Socket has been successfully created!\n";
  return true;
}

// Associate socket id with an address
bool Socket::bind ()
{
  std::cout << "Binding socket to host info ...\n"; 
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = htonl(INADDR_ANY); // This HOST IP address
  m_addr.sin_port = 0; // let kernel assign port

  int bind_return = ::bind ( m_sock,
			     (sockaddr *) (&m_addr),
			     sizeof ( m_addr ) );

  if ( bind_return == -1 )
    {
      return false;
    }
 
  std::cout << "\nNow the socket has been bounded.\n";
  std::cout << "Retrieving info on PORT number ...\n";
  // find out Kernel assigned PORT number and show it
  int length = sizeof(name_addr);
  int r = ::getsockname(m_sock, (sockaddr *)&name_addr,(socklen_t *)&length);

  if(r < 0)
    {
      std::cout << "fail to get socketname\n"; 
      return false;
    } 
  
  int port = ntohs(name_addr.sin_port);
  std::cout << "Port = " << port << "  Succeed!\n";
  return true;
}

// Server listen to the connection requests
bool Socket::listen() const
{
  std::cout << "Server is now listening ..." << "\n";
  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );

  if ( listen_return == -1 )
    {
      return false;
    }

  return true;
}

// Server accepts the client requests
bool Socket::accept ( Socket& new_socket ) const
{
  std::cout << "Server: accepting new connections ... " << "\n";
  int addr_length = sizeof ( m_addr );

  // now m_addr stores the client information
  new_socket.m_sock = ::accept ( m_sock, (sockaddr *) (&m_addr), (socklen_t*) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
 
    std::cout <<  "Server: accepted a client connection from\n";
    std::cout << "-----------------------------------------------\n";
    std::cout << " IP = " << inet_ntoa(m_addr.sin_addr) << " port = " << ntohs(m_addr.sin_port) << "\n";
    std::cout << "-----------------------------------------------\n";
    return true;
}

// The function to send data
bool Socket::send ( const std::string s ) const
{
  std::cout << "Sending data ..." << "\n";
  int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  if ( status == -1 )
    {
      return false;
    }
  else
    {
      return true;
    }
}

// The function to recieve data
int Socket::recv ( std::string& s ) const
{
  std::cout << "Recieving data ... " << "\n"; 
  char buf [ MAXRECV + 1 ];
  s = "";
  memset ( buf, 0, MAXRECV + 1 );
  
  int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( status == -1 )
    {
      std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
      return 0;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      s = buf;
      return status;
    }
}

// Client to connect to the server
bool Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;

  std::cout << "Client connecting to server ... \n"; 
  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( m_sock, (sockaddr *) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

void Socket::set_non_blocking ( const bool b )
{

  int opts;

  opts = fcntl ( m_sock,
		 F_GETFL );

  if ( opts < 0 )
    {
      return;
    }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock,
	  F_SETFL,opts );

}
