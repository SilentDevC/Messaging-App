#ifndef CXX_SERVER_TYPES
#define CXX_SERVER_TYPES

#include "Server.hpp"
#include <string>
	//bolean types
#define BOOL bool 

//signed integer types
#define SQLMININT char 
#define SHORTINT short 
#define INT int
#define LONGINT long 
#define SQLMAXINT long long 

//unsigned integer types
#define UMININT unsigned char 
#define USHORTINT unsigned short 
#define UINT unsigned int
#define ULONGINT unsigned long 
#define UMAXINT unsigned long long 

// fixed size string types // mirroring the SQL 
#define SQLSTRING(T)  std::string(T)
#define SQLSTRINGVIEW(T) std::string_view(T)

// fixed size string types // mirroring the SQL 
#define cSQLSTRING(T)  const std::string(T)
#define cSQLSTRINGVIEW(T) const std::string_view(T)


//long integer types 
#define INT32 uint32_t 
#define INT64 uint64_t
#ifdef BOOST_MP_CPP_INT_HPP
#define INT128 uint128_t 
#endif

#endif //CXX_SERVER_TYPES
