#pragma once

#include <string>
#include <exception>

//--------------------------------------------------------------
/// \class Exception for accessing NULL objects
//--------------------------------------------------------------
class CNullReferenceException : public std::exception
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CNullReferenceException(const char * reference = NULL);

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CNullReferenceException() throw();

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