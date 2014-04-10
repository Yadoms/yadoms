#include "stdafx.h"
#include "DeviceManager.h"
#include "SensorBasic.h"
#include "X10Basic.h"
#include "../Protocol.h"
#include "../ICommandRule.h"

namespace communication { namespace rules { namespace standard {

   CDeviceManager::CDeviceManager()
   {
      m_handledProtocols.push_back( boost::shared_ptr< communication::rules::IProtocol >( new CProtocol<CX10Basic>("x10.basic") ) );
      m_handledProtocols.push_back( boost::shared_ptr< communication::rules::IProtocol >( new CProtocol<CSensorBasic>("sensor.basic") ) );
   }

   CDeviceManager::~CDeviceManager()
   {
   }



   // IFactory implementation
   bool CDeviceManager::isHandled(shared::xpl::CXplMessage & message)
   {
      //this factory is for the Xpl Standard
      //so all messages are handled
      return true;
   }

   boost::shared_ptr<IRule> CDeviceManager::identifyRule(shared::xpl::CXplMessage & msg, CRuleInstanceManager & instanceManager)
   {
      return identifyRule(msg.getMessageSchemaIdentifier().toString(), instanceManager);
   }
      
   bool CDeviceManager::isHandled(database::entities::CDevice & device)
   {
      //this factory is for the Xpl Standard
      //so all messages are handled
      return true;
   }

   boost::shared_ptr<IRule> CDeviceManager::identifyRule(database::entities::CDevice & device, CRuleInstanceManager & instanceManager)
   {
      return identifyRule(device.Protocol, instanceManager);
   }
   // [END] IFactory implementation


   boost::shared_ptr<IRule> CDeviceManager::identifyRule(const std::string & protocolName, CRuleInstanceManager & instanceManager)
   {
      BOOST_FOREACH(boost::shared_ptr< communication::rules::IProtocol > handledProtocols, m_handledProtocols)
      {
         if(handledProtocols && boost::iequals(protocolName, handledProtocols->getProtocolIdentifier()))
            return handledProtocols->createRuleInstance(instanceManager);
      }

      //not handled
      return boost::shared_ptr<IRule>();
   }


   bool CDeviceManager::matchHardware(const std::string & hardwareIdentifier)
   {
      //this factory is for the Xpl Standard
      //so all hardwares
      return true;
   }

   std::vector<std::string> CDeviceManager::getHandledProtocols()
   {
      std::vector<std::string> result;

      BOOST_FOREACH(boost::shared_ptr< communication::rules::IProtocol > handledProtocols, m_handledProtocols)
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
         boost::shared_ptr<communication::rules::ICommandRule> commandRule = boost::dynamic_pointer_cast<ICommandRule>(rule);
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
} //namespace rules
} //namespace communication