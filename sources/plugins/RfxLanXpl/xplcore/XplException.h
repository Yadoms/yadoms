#pragma once

namespace xplcore
{

   //--------------------------------------------------------------
   /// \class Exception for xpl exception
   //--------------------------------------------------------------
   class CXplException : public std::exception
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param [in] message : Message to display
      //--------------------------------------------------------------
      explicit CXplException(const std::string & message) : m_message(message)
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CXplException() throw()
      {
      }

      //--------------------------------------------------------------
      /// \brief	    Build full message explaining exception reason
      /// \return     message explaining exception reason
      //--------------------------------------------------------------
      virtual const char* what() const throw() { return m_message.c_str(); }

   private:
      std::string m_message;
   };

} // namespace xplcore