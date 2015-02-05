//-----------------------------------------------------------------------------
//
//	FatalErrorException.h
//
//	Exception for openzwave
//-----------------------------------------------------------------------------

#ifndef _FatalErrorException_H
#define _FatalErrorException_H

#include <sstream>

namespace OpenZWave
{
   class CFatalErrorException : public std::exception
   {
   public:
   	//-----------------------------------------------------------------------------
      // Construction
      //-----------------------------------------------------------------------------
      CFatalErrorException(uint32 exitCode)
       :m_exitCode(exitCode)
      {
      }
      
       //--------------------------------------------------------------
       /// \brief      Destructor
       //--------------------------------------------------------------
       virtual ~CFatalErrorException() throw()
       {
       }

       
      //-----------------------------------------------------------------------------
      // Accessor methods 
      //-----------------------------------------------------------------------------
      uint32 GetExitCode() { return m_exitCode; }
   
       //--------------------------------------------------------------
       /// \brief	    Build full message explaining exception reason
       /// \return     message explaining exception reason
       //--------------------------------------------------------------
       virtual const char* what() const throw()
       {
           std::stringstream ss;
           ss << m_exitCode;
           return ss.str().c_str();
       }
   
   private:
      //-----------------------------------------------------------------------------
      // Member variables
      //-----------------------------------------------------------------------------
      uint32   m_exitCode;
   };
}

#endif // _FatalErrorException_H
