#pragma once

#include <string>
#include <exception>

//--------------------------------------------------------------
/// \class Standard excption with some text
//--------------------------------------------------------------
class CException : public std::exception
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Excpetion message
   //--------------------------------------------------------------
   CException(const char * message);

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CException() throw();

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