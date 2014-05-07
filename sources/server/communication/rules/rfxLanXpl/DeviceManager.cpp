#include "stdafx.h"
#include "DeviceManager.h"
#include "AcBasic.h"
#include "SensorBasic.h"
#include "X10Basic.h"
#include "../Protocol.h"
#include <shared/xpl/XplConstants.h>

namespace communication { namespace rules { namespace rfxLanXpl {

   CDeviceManager::CDeviceManager()
   {
      m_handledProtocols.push_back( boost::shared_ptr< communication::rules::IProtocol >( new CProtocol<CAcBasic>("ac.basic") ) );
      m_handledProtocols.push_back( boost::shared_ptr< communication::rules::IProtocol >( new CProtocol<CSensorBasic>("sensor.basic") ) );
      m_handledProtocols.push_back( boost::shared_ptr< communication::rules::IProtocol >( new CProtocol<CX10Basic>("x10.basic") ) );
   }

   CDeviceManager::~CDeviceManager()
   {
   }



   // IFactory implementation
   bool CDeviceManager::isHandled(const std::string & hardwareIdentifier)
   {
      //this factory is for the RfxComLan with Xpl firmware
      return boost::starts_with(hardwareIdentifier, "rfxcom-lan.");
   }

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

   // [END] IFactory implementation





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