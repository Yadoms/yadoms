#include "stdafx.h"
#include "DeviceManager.h"
#include "AcBasic.h"
#include "SensorBasic.h"
#include "../Protocol.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   CDeviceManager::CDeviceManager()
   {
      m_handledProtocols.push_back( boost::shared_ptr< communication::rules::IProtocol >( new CProtocol<CAcBasic>("ac.basic") ) );
      m_handledProtocols.push_back( boost::shared_ptr< communication::rules::IProtocol >( new CProtocol<CSensorBasic>("sensor.basic") ) );

   }

   CDeviceManager::~CDeviceManager()
   {
   }



   // IFactory implementation
   bool CDeviceManager::isHandled(shared::xpl::CXplMessage & message)
   {
      return matchHardware(message.getSource().toString()) || (boost::istarts_with(message.getSource().toString(), "yadoms") && matchHardware(message.getTarget().toString()));
   }

   boost::shared_ptr<IRule> CDeviceManager::identifyRule(shared::xpl::CXplMessage & msg, CRuleInstanceManager & instanceManager)
   {
      return identifyRule(msg.getMessageSchemaIdentifier().toString(), instanceManager);
   }


   bool CDeviceManager::isHandled(database::entities::CDevice & device)
   {
      return matchHardware(device.HardwareIdentifier);
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

      //not handled by RfxLanXpl or using standard rule
      return boost::shared_ptr<IRule>();
   }

   bool CDeviceManager::matchHardware(const std::string & hardwareIdentifier)
   {
      //this factory is for the RfxComLan with Xpl firmware
      return boost::starts_with(hardwareIdentifier, "rfxcom-lan.");
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
         boost::shared_ptr<ICommandRule> commandRule = boost::dynamic_pointer_cast<ICommandRule>(rule);
         if(commandRule)
         {
            return commandRule->generateVirtualDeviceIdentifier();
         }
         else
         {
            std::string errorMessage = (boost::format("The protocol %1% for RfxCOMLAn do not support commands") % protocolIdentifier).str();
            throw shared::exception::CException(errorMessage);
         }
      }
      else
      {
         std::string errorMessage = (boost::format("The protocol %1% for RfxCOMLAn is not supported") % protocolIdentifier).str();
         throw shared::exception::CException(errorMessage);
      }
   }






} //namespace rfxLanXpl
} //namespace rules
} //namespace communication