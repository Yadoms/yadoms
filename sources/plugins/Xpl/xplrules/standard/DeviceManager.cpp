#include "stdafx.h"
#include "DeviceManager.h"
#include "SensorBasic.h"
#include "X10Basic.h"
#include "../Protocol.h"
#include "../ICommandRule.h"
#include <shared/exception/Exception.hpp>

namespace xplrules { namespace standard {

   CDeviceManager::CDeviceManager()
   {
      m_handledProtocols.push_back(boost::shared_ptr< xplrules::IProtocol >(new CProtocol<CX10Basic>(CX10Basic::getProtocol())));
      m_handledProtocols.push_back(boost::shared_ptr< xplrules::IProtocol >(new CProtocol<CSensorBasic>(CSensorBasic::getProtocol())));
   }

   CDeviceManager::~CDeviceManager()
   {
   }



   // IFactory implementation
   bool CDeviceManager::isHandled(const std::string & message)
   {
      //this factory is for the Xpl Standard
      //so all messages are handled
      return true;
   }

   boost::shared_ptr<IRule> CDeviceManager::identifyRule(const std::string & protocolName, CRuleInstanceManager & instanceManager)
   {
      BOOST_FOREACH(boost::shared_ptr< xplrules::IProtocol > handledProtocols, m_handledProtocols)
      {
         if(handledProtocols && boost::iequals(protocolName, handledProtocols->getProtocolIdentifier()))
            return handledProtocols->createRuleInstance(instanceManager);
      }

      //not handled
      return boost::shared_ptr<IRule>();
   }
   // [END] IFactory implementation


   std::vector<std::string> CDeviceManager::getHandledProtocols()
   {
      std::vector<std::string> result;

      BOOST_FOREACH(boost::shared_ptr< xplrules::IProtocol > handledProtocols, m_handledProtocols)
      {
         if(handledProtocols)
            result.push_back(handledProtocols->getProtocolIdentifier());
      }

      return result;
   }
   
   std::string CDeviceManager::generateVirtualDeviceIdentifier(const std::string & protocolIdentifier, CRuleInstanceManager & instanceManager)
   {
      boost::shared_ptr<IRule> rule = identifyRule(protocolIdentifier, instanceManager);

      if(rule)
      {
         //check if the rule handled commands
         boost::shared_ptr<xplrules::ICommandRule> commandRule = boost::dynamic_pointer_cast<ICommandRule>(rule);
         if(commandRule)
         {
            return commandRule->generateVirtualDeviceIdentifier();
         }
         else
         {
            std::string errorMessage = (boost::format("The protocol %1% for Standard XPL do not support commands") % protocolIdentifier).str();
            throw shared::exception::CException(errorMessage);
         }
      }
      else
      {
         std::string errorMessage = (boost::format("The protocol %1% for Standard XPL is not supported") % protocolIdentifier).str();
         throw shared::exception::CException(errorMessage);
      }
   }


} //namespace standard
} //namespace xplrules
