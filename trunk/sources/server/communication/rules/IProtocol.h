#pragma once

#include "IRule.h"
#include "RuleInstanceManager.hpp"

namespace communication { namespace rules {

   //------------------------------------
   ///\brief Interface for Rule protocol
   //------------------------------------
   class IProtocol
   {
   public:
      //------------------------------------
      ///\brief Create an instance of the protocol rule
      ///\param [in] instanceManager : the global instance manager (avoid creating multiple instances of protocols)
      ///\return the rule for the current protocol (may be null if protocol not handled)
      //------------------------------------
      virtual boost::shared_ptr<IRule> createRuleInstance(CRuleInstanceManager & instanceManager) = 0;

      //------------------------------------
      ///\brief get the protocol identifier i.e. : "x10.basic"
      ///\return the protocol identifier
      //------------------------------------
      virtual const std::string & getProtocolIdentifier() = 0;

      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~IProtocol() {}
   };

} //namespace rules
} //namespace communication