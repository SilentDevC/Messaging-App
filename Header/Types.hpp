#ifndef CXX_SERVER_TYPES
#define CXX_SERVER_TYPES

#include "Server.hpp"

//bolean types
#define BOOL bool 

//signed integer types
#define MININT char 
#define SHORTINT short 
#define INT int
#define LONGINT long 
#define MAXINT long long 

//unsigned integer types
#define UMININT unsigned char 
#define USHORTINT unsigned short 
#define UINT unsigned int
#define ULONGINT unsigned long 
#define UMAXINT unsigned long long 

// fixed size string types // mirroring the SQL 
#define STRING32(T)  char T[32]
#define STRING64(T)  char T[64]
#define STRING128(T)  char T[128]
#define STRING256(T)  char T[256]
#define STRING512(T)  char T[512]
#define STRINGGEN  char*

// fixed size string types // mirroring the SQL 
#define cSTRING32(T) const char T[32]
#define cSTRING64(T) const char T[64]
#define cSTRING128(T) const char T[128]
#define cSTRING256(T) const char T[256]
#define cSTRING512(T) const char T[512]
#define cSTRINGGEN const char*

//long integer types 
#define INT32 uint32_t 
#define INT64 uint64_t
#ifdef BOOST_MP_CPP_INT_HPP
	#define INT128 uint128_t 
#endif


#endif //CXX_SERVER_TYPES
