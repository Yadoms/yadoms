#pragma once

#include "IProtocol.h"

namespace communication { namespace rules {

   //------------------------------------
   ///\brief Create an instance of the protocol rule
   ///\param [in] instanceManager : the global instance manager (avoid creating multiple instances of protocols)
   ///\return the rule for the current protocol (may be null if protocol not handled)
   //------------------------------------
   template<class TProtocol>
   class CProtocol : public IProtocol
   {
   public:
      //------------------------------------
      ///\brief Constructor of a protocol container
      ///\param [in] protocolIdentifier : the protocol identifier (i.e. "x10.basic")
      //------------------------------------
      CProtocol(const std::string & protocolIdentifier)
         :m_protocolIdentifier(protocolIdentifier)
      {
      }

      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~CProtocol()
      {
      }

      // IProtocol implementation
      boost::shared_ptr<IRule> createRuleInstance(CRuleInstanceManager & instanceManager)
      {
         return instanceManager.getRule<TProtocol>();
      }

      const std::string & getProtocolIdentifier()
      {
         return m_protocolIdentifier;
      }
      // [END] IProtocol implementation

   private:
      //------------------------------------
      ///\brief the protocol identifier (i.e. "x10.basic")
      //------------------------------------
      std::string m_protocolIdentifier;
   };

} //namespace rules
} //namespace communication