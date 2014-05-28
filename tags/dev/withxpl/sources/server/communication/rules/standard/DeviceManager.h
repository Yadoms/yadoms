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
      virtual bool isHandled(const std::string & hardwareIdentifier);
      virtual boost::shared_ptr<IRule> identifyRule(const std::string & protocol, CRuleInstanceManager & instanceManager);
      virtual std::vector<std::string> getHandledProtocols();
      virtual std::string generateVirtualDeviceIdentifier(const std::string & protocolIdentifier, CRuleInstanceManager & instanceManager);
      // [END] IFactory implementation
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
   
} //namespace standard
} //namespace rules
} //namespace communication