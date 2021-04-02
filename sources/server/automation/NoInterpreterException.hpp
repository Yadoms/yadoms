#pragma once

#include "RuleException.hpp"


//--------------------------------------------------------------
/// \brief	Exception handling for automation rule when no corresponding interpreter found
//--------------------------------------------------------------
class CNoInterpreterException : public CRuleException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   explicit CNoInterpreterException(const std::string& message)
      :CRuleException(message)
   {
   }

   virtual ~CNoInterpreterException() noexcept = default;
};

