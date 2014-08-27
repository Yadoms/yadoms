//-----------------------------------------------------------------------------
//
//	FatalErrorException.h
//
//	Exception for openzwave
//-----------------------------------------------------------------------------

#ifndef _FatalErrorException_H
#define _FatalErrorException_H

namespace OpenZWave
{
   class CFatalErrorException : public std::exception
   {
   public:
   	//-----------------------------------------------------------------------------
      // Construction
      //-----------------------------------------------------------------------------
      CFatalErrorException(uint32 exitCode)
         :std::exception(GetExceptionText(exitCode).c_str()), m_exitCode(exitCode)
      {
      }
      
      virtual ~CFatalErrorException()
      {
      }
      
      //-----------------------------------------------------------------------------
      // Accessor methods 
      //-----------------------------------------------------------------------------
      uint32 GetExitCode() { return m_exitCode; }
   
   
   private:
      static std::string GetExceptionText(uint32 exitCode)
      {
         char buf[20];
         sprintf_s( buf, sizeof(buf), "%d ", exitCode );
         return std::string(buf);
      }
   
      //-----------------------------------------------------------------------------
      // Member variables
      //-----------------------------------------------------------------------------
      uint32   m_exitCode;
   };
}

#endif // _FatalErrorException_H
