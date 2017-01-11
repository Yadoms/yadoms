#pragma once

#include <shared/exception/Exception.hpp>


//--------------------------------------------------------------
/// \brief	Exception handling for automation rule
//--------------------------------------------------------------
class CRuleException : public shared::exception::CException//TODO virer ?
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   explicit CRuleException(const std::string& message)
      :CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CRuleException() throw()
   {
   }
};

