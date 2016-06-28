#include "stdafx.h"
#include "DeviceManager.h"
#include "AcBasic.h"
#include "SensorBasic.h"
#include "X10Basic.h"
#include "X10Security.h"
#include "LogBasic.h"
#include "RemoteBasic.h"
#include "DateTimeBasic.h"
#include "../Protocol.h"
#include <shared/exception/Exception.hpp>

namespace xplrules
{
   namespace rfxLanXpl
   {
      CDeviceManager::CDeviceManager()
      {
         m_handledProtocols.push_back(boost::make_shared<CProtocol<CAcBasic> >(CAcBasic::getProtocol()));
         m_handledProtocols.push_back(boost::make_shared<CProtocol<CSensorBasic> >(CSensorBasic::getProtocol()));
         m_handledProtocols.push_back(boost::make_shared<CProtocol<CX10Basic> >(CX10Basic::getProtocol()));
         m_handledProtocols.push_back(boost::make_shared<CProtocol<CLogBasic> >(CLogBasic::getProtocol()));
         m_handledProtocols.push_back(boost::make_shared<CProtocol<CX10Security> >(CX10Security::getProtocol()));
         m_handledProtocols.push_back(boost::make_shared<CProtocol<CRemoteBasic> >(CRemoteBasic::getProtocol()));
         m_handledProtocols.push_back(boost::make_shared<CProtocol<CDateTimeBasic> >(CDateTimeBasic::getProtocol()));
      }

      CDeviceManager::~CDeviceManager()
      {
      }


      // IFactory implementation
      bool CDeviceManager::isHandled(const std::string& hardwareIdentifier)
      {
         //this factory is for the RfxComLan with Xpl firmware
         return boost::starts_with(hardwareIdentifier, "rfxcom-lan.");
      }

      boost::shared_ptr<IRule> CDeviceManager::identifyRule(const std::string& protocolName,
                                                            CRuleInstanceManager& instanceManager)
      {
         for (auto i = m_handledProtocols.begin(); i != m_handledProtocols.end(); ++i)
         {
            if ((*i) && boost::iequals(protocolName, (*i)->getProtocolIdentifier()))
               return (*i)->createRuleInstance(instanceManager);
         }

         //not handled by RfxLanXpl or using standard rule
         return boost::shared_ptr<IRule>();
      }

      // [END] IFactory implementation


      std::vector<std::string> CDeviceManager::getHandledProtocols()
      {
         std::vector<std::string> result;

         for (auto i = m_handledProtocols.begin(); i != m_handledProtocols.end(); ++i)
         {
            if (*i)
               result.push_back((*i)->getProtocolIdentifier());
         }

         return result;
      }

      std::string CDeviceManager::generateVirtualDeviceIdentifier(const std::string& protocolIdentifier,
                                                                  CRuleInstanceManager& instanceManager)
      {
         auto rule = identifyRule(protocolIdentifier, instanceManager);

         if (rule)
         {
            //check if the rule handled commands
            boost::shared_ptr<ICommandRule> commandRule = boost::dynamic_pointer_cast<ICommandRule>(rule);
            if (commandRule)
            {
               return commandRule->generateVirtualDeviceIdentifier();
            }

            auto errorMessage = (boost::format("The protocol %1% for RfxCOMLAn do not support commands") % protocolIdentifier).str();
            throw shared::exception::CException(errorMessage);
         }

         auto errorMessage = (boost::format("The protocol %1% for RfxCOMLAn is not supported") % protocolIdentifier).str();
         throw shared::exception::CException(errorMessage);
      }
   } //namespace rfxLanXpl
} //namespace xplrules


