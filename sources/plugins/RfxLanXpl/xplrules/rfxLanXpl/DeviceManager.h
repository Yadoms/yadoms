#pragma once

#include "../IDeviceManager.h"
#include "../IProtocol.h"

namespace xplrules
{
   namespace rfxLanXpl
   {
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
         bool isHandled(const std::string& hardwareIdentifier) override;
         boost::shared_ptr<IRule> identifyRule(const std::string& protocol, CRuleInstanceManager& instanceManager) override;
         std::vector<std::string> getHandledProtocols() override;
         std::string generateVirtualDeviceIdentifier(const std::string& protocolIdentifier, CRuleInstanceManager& instanceManager) override;
         // [END] IDeviceManager implementation
      private:
         //------------------------------------------
         ///\brief  Type : vector of Protocols
         //------------------------------------------
         typedef std::vector<boost::shared_ptr<IProtocol> > Protocols;

         //------------------------------------------
         ///\brief  Vector of handled protocols
         //------------------------------------------
         Protocols m_handledProtocols;
      };
   } //namespace rfxLanXpl
} //namespace xplrules


