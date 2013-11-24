#pragma once

//--------------------------------------------------------------
/// \class Exception for non implemented part of code
//--------------------------------------------------------------
class CNotImplementedException : public std::exception
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CNotImplementedException()
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CNotImplementedException() throw()
   {
   }

   //--------------------------------------------------------------
   /// \brief	    Build full message explaining exception reason
   /// \return     message explaining exception reason
   //--------------------------------------------------------------
   virtual const char* what() const throw() { return "Not yet implemented"; }
};