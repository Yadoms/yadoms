#pragma once

#include "../IDeviceManager.h"
#include "../IProtocol.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   //--------------------------------
   ///\brief Class which manages RfxLanXpl devices
   //--------------------------------
   class CDeviceManager : public IDeviceManager
   {
   public:
      //--------------------------------
      ///\brief Constructor
      //--------------------------------
      CDeviceManager();

      //--------------------------------
      ///\brief Destructor
      //--------------------------------
      virtual ~CDeviceManager();
      
      // IDeviceManager implementation
      virtual bool isHandled(shared::xpl::CXplMessage & message);
      virtual boost::shared_ptr<IRule> identifyRule(shared::xpl::CXplMessage & msg, CRuleInstanceManager & instanceManager);

      virtual bool isHandled(database::entities::CDevice & device);
      virtual boost::shared_ptr<IRule> identifyRule(database::entities::CDevice & device, CRuleInstanceManager & instanceManager);

      virtual bool matchHardware(const std::string & hardwareIdentifier);
      virtual std::vector<std::string> getHandledProtocols();
      virtual std::string generateVirtualDeviceIdentifier(const std::string & protocolIdentifier, CRuleInstanceManager & instanceManager);
      // [END] IDeviceManager implementation
   private:

      //------------------------------------------
      ///\brief  Check if the hardware name is handled by factory
      ///\param [in] hardwareName : the hardware name (i.e. : rfxlan-0x12345)
      ///\return true if the hardware name starts with "rfxlan-"
      //------------------------------------------
      bool isHandled(const std::string & hardwareName);

      //------------------------------------------
      ///\brief  Get the rule for a protocol
      ///\param [in] protocolName : the protocol name (i.e. : ac.basic)
      ///\return the rule for the given protocol (may be null if protocol not handled
      //------------------------------------------
      boost::shared_ptr<IRule> identifyRule(const std::string & protocolName, CRuleInstanceManager & instanceManager);

      //------------------------------------------
      ///\brief  Type : vector of Protocols
      //------------------------------------------
      typedef std::vector< boost::shared_ptr< communication::rules::IProtocol > > Protocols;

      //------------------------------------------
      ///\brief  Vector of handled protocols
      //------------------------------------------
      Protocols m_handledProtocols;
   };
   
} //namespace rfxLanXpl
} //namespace rules
} //namespace communication