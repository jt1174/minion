/*
* Minion http://minion.sourceforge.net
* Copyright (C) 2006-09
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef DEBUG_H
#define DEBUG_H

#include "basic_headers.h"

template<typename T>
inline void CheckNotBound(const T& t, std::string s, std::string s2 = "")
{
  for(SysInt i = 0; i < t.size(); ++i)
  {
    if(t[i].isBound())
    {
      std::cerr << "Cannot use '" << s << "' with BOUND or SPARSEBOUND variables.\n";
      if(s2 != "")
        std::cerr << "Please use '" << s2 << "' as a replacement or";
      std::cerr << "Please use DISCRETE variables instead.\n";
      abort();
    }
  }
}

template<typename T>
inline void CheckNotBoundSingle(const T& t, std::string s, std::string s2 = "")
{
    if(t.isBound())
    {
      std::cerr << "Cannot use " << s << " with BOUND or SPARSEBOUND variables.\n";
      if(s2 != "")
        std::cerr << "Please use " << s2 << " as a replacement or ";
      std::cerr << "Please use DISCRETE variables instead.\n";
      abort();
    }
}

#ifdef MINION_DEBUG_PRINT
  #ifndef MINION_DEBUG
  #define MINION_DEBUG
  #endif
#endif

#ifndef MINION_DEBUG
  #ifndef NO_DEBUG
  #define NO_DEBUG
  #endif
#endif



struct parse_exception : public std::exception
{
  string error;
  parse_exception(string s) : error(s)
  {}
  
  virtual const char* what() const throw() 
  { return error.c_str(); }
  
  virtual ~parse_exception() throw()
  {}
};

inline void USER_ERROR(std::string s)
{
    cerr << "A fatal error has occurred:\n" << endl;
    cerr << s << endl;
    exit(1);
}

#define D_FATAL_ERROR(s) { D_FATAL_ERROR2(s,  __FILE__, to_string(__LINE__)); throw 0; }

#define INPUT_ERROR(s) { cout << "There was a problem in your input file:\n" << s << endl; exit(1); }
// These functions are defined in debug_functions.cpp

extern bool debug_crash;
void D_FATAL_ERROR2(string s, string file, string line);
void _NORETURN FAIL_EXIT(string s = "");

struct assert_fail {};

void error_printing_function(std::string a, std::string f, SysInt line) _NORETURN;
void user_error_printing_function(std::string a, std::string f, SysInt line) _NORETURN;

void FATAL_REPORTABLE_ERROR()  _NORETURN;


#define CHECK(x, y) { if(!(x)) { user_error_printing_function(y, __FILE__, __LINE__); } }

// Check a value doesn't overflow, to be used in ctor of cts 
#define CHECKSIZE( x, message ) CHECK( x <= ((BigInt) checked_cast<SysInt>(DomainInt_Max)) && x>= ((BigInt) checked_cast<SysInt>(DomainInt_Min)) , message )

#ifdef MINION_DEBUG

#ifndef BOUNDS_CHECK
#define BOUNDS_CHECK
#endif

#define D_ASSERT(x) {if(!(x)) {error_printing_function(#x, __FILE__, __LINE__); } }
#define D_DATA(x) x

enum DebugTypes
{ DI_SOLVER, DI_SUMCON, DI_BOOLCON, DI_ANDCON, DI_ARRAYAND, DI_QUEUE, DI_REIFY,
  DI_LEXCON, DI_TABLECON, DI_TEST, DI_DYSUMCON, DI_DYNAMICTRIG, DI_DYELEMENT, DI_INTCON, DI_LONGINTCON,
  DI_INTCONTAINER, DI_BOUNDCONTAINER, DI_GACELEMENT, DI_CHECKCON, DI_VECNEQ, DI_MEMBLOCK, DI_POINTER,
  DI_OR, DI_GADGET
};
  
#define DEBUG_CASE(x) case x: std::cerr << #x; break;

#else

#define D_DATA(x)
#define D_ASSERT(x)
#endif

inline bool DOMAIN_CHECK(BigInt v)
{ return v < DomainInt_Max && v > DomainInt_Min; }

// These are just to catch cases where the user didn't cast to BigInt
// themselves, which makes the function useless.
inline void DOMAIN_CHECK(DomainInt);
inline void DOMAIN_CHECK(SysInt);
inline void DOMAIN_CHECK(UnsignedSysInt);
        
#endif //DEBUG_H

