#pragma once

#include "../IDeviceManager.h"
#include "../IProtocol.h"

namespace communication { namespace rules { namespace extensions {

   //--------------------------------
   ///\brief Class which manages xpl extensions
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
      virtual bool isHandled(const std::string & hardwareIdentifier);
      virtual boost::shared_ptr<IRule> identifyRule(const std::string & protocol, CRuleInstanceManager & instanceManager);
      virtual std::vector<std::string> getHandledProtocols();
      virtual std::string generateVirtualDeviceIdentifier(const std::string & protocolIdentifier, CRuleInstanceManager & instanceManager);
      // [END] IDeviceManager implementation
   private:
      //------------------------------------------
      ///\brief  Type : vector of Protocols
      //------------------------------------------
      typedef std::vector< boost::shared_ptr< communication::rules::IProtocol > > Protocols;

      //------------------------------------------
      ///\brief  Vector of handled protocols
      //------------------------------------------
      Protocols m_handledProtocols;
   };
   
} //namespace extensions
} //namespace rules
} //namespace communication