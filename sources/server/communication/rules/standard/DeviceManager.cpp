#include "stdafx.h"
#include "DeviceManager.h"
#include "SensorBasic.h"
#include "X10Basic.h"

namespace communication { namespace rules { namespace standard {

   CDeviceManager::CDeviceManager()
   {
   }

   CDeviceManager::~CDeviceManager()
   {
   }



   // IFactory implementation
   bool CDeviceManager::isHandled(shared::xpl::CXplMessage & message)
   {
      return isHandled(message.getSource().toString()) || isHandled(message.getTarget().toString());
   }

   boost::shared_ptr<IRule> CDeviceManager::identifyRule(shared::xpl::CXplMessage & msg, CRuleInstanceManager & instanceManager)
   {
      return identifyRule(msg.getMessageSchemaIdentifier().toString(), instanceManager);
   }
      
   bool CDeviceManager::isHandled(database::entities::CDevice & device)
   {
      return isHandled(device.HardwareIdentifier);
   }

   boost::shared_ptr<IRule> CDeviceManager::identifyRule(database::entities::CDevice & device, CRuleInstanceManager & instanceManager)
   {
      return identifyRule(device.Protocol, instanceManager);
   }
   // [END] IFactory implementation




   bool CDeviceManager::isHandled(const std::string & hardwareName)
   {
      //this factory is for the Xpl Standard
      //so all messages are handled
      return true;
   }


   boost::shared_ptr<IRule> CDeviceManager::identifyRule(const std::string & protocolName, CRuleInstanceManager & instanceManager)
   {
      if(boost::iequals(protocolName, "x10.basic"))
         return instanceManager.getRule<CX10Basic>();

      if(boost::iequals(protocolName, "sensor.basic"))
         return instanceManager.getRule<CSensorBasic>();

      //not handled
      return boost::shared_ptr<IRule>();
   }

} //namespace standard
} //namespace rules
} //namespace communication