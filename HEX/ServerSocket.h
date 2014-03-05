// Definition of the ServerSocket class
#ifndef ServerSocket_class
#define ServerSocket_class

#include "Socket.h"

// private inheritence from base class Socket
class ServerSocket : private Socket
{
 public:

  ServerSocket (const char *hostname);
  ServerSocket (){};
  virtual ~ServerSocket();

  const ServerSocket& operator << ( const std::string& ) const;
  const ServerSocket& operator >> ( std::string& ) const;

  // print out the host info
  bool hostinfo(const char *name) const;

  void accept ( ServerSocket& );

};


#endif
