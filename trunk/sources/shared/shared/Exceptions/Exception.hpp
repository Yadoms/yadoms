#pragma once

#include <string>
#include <exception>

//--------------------------------------------------------------
/// \class Standard exception with some text
//--------------------------------------------------------------
class CException : public std::exception
{
protected:
   //--------------------------------------------------------------
   /// \brief	                        Default constructor (only internal usage)
   //--------------------------------------------------------------
   CException() {}

public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   CException(const char * message)
   {
      if(message != NULL)
         m_message=std::string(message);
   }
   CException(const std::string& message)
      :m_message(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CException() throw()
   {
   }

   //--------------------------------------------------------------
   /// \brief	    Build full message explaining exception reason
   /// \return     message explaining exception reason
   //--------------------------------------------------------------
   virtual const char* what() const throw() { return m_message.c_str(); }

protected:
   //--------------------------------------------------------------
   /// \brief      Message container
   //--------------------------------------------------------------
   std::string m_message;
};