#pragma once

#include "../IDeviceManager.h"
#include "../IProtocol.h"

namespace communication { namespace rules { namespace standard {

   //--------------------------------
   ///\brief Class which manages common xpl devices
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
      
      // IFactory implementation
      virtual bool isHandled(shared::xpl::CXplMessage & message);
      virtual boost::shared_ptr<IRule> identifyRule(shared::xpl::CXplMessage & msg, CRuleInstanceManager & instanceManager);
      
      virtual bool isHandled(database::entities::CDevice & device);
      virtual boost::shared_ptr<IRule> identifyRule(database::entities::CDevice & device, CRuleInstanceManager & instanceManager);

      virtual bool matchHardware(const std::string & hardwareIdentifier);
      virtual std::vector<std::string> getHandledProtocols();
      virtual std::string generateVirtualDeviceIdentifier(const std::string & protocolIdentifier, CRuleInstanceManager & instanceManager);
      // [END] IFactory implementation
   private:
      //------------------------------------------
      ///\brief  Get the rule for a protocol
      ///\param [in] protocolName : the protocol name (i.e. : sensor.basic)
      ///\return the rule for the given protocol (may be null if protocol not handled)
      //------------------------------------------
      boost::shared_ptr<IRule> identifyRule(const std::string & protocolName, CRuleInstanceManager & instanceManager);
      // [END] IFactory implementation
     
      //------------------------------------------
      ///\brief  Type : vector of Protocols
      //------------------------------------------
      typedef std::vector< boost::shared_ptr< communication::rules::IProtocol > > Protocols;

      //------------------------------------------
      ///\brief  Vector of handled protocols
      //------------------------------------------
      Protocols m_handledProtocols;
   };
   
} //namespace standard
} //namespace rules
} //namespace communication