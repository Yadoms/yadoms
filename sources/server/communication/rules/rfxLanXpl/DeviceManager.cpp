#include "stdafx.h"
#include "DeviceManager.h"
#include "AcBasic.h"
#include "SensorBasic.h"

namespace communication { namespace rules { namespace rfxLanXpl {

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
      //this factory is for the RfxComLan with Xpl firmware
      return boost::starts_with(hardwareName, "rfxcom-lan.");
   }

   boost::shared_ptr<IRule> CDeviceManager::identifyRule(const std::string & protocolName, CRuleInstanceManager & instanceManager)
   {
      if(boost::iequals(protocolName, "ac.basic"))
         return instanceManager.getRule<CAcBasic>();
      
      if(boost::iequals(protocolName, "sensor.basic"))
         return instanceManager.getRule<CSensorBasic>();
         
      //not handled by RfxLanXpl or using standard rule
      return boost::shared_ptr<IRule>();
   }







} //namespace rfxLanXpl
} //namespace rules
} //namespace communication