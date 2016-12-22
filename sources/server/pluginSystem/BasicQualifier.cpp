#include "stdafx.h"
#include "BasicQualifier.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>

namespace pluginSystem
{
   CBasicQualifier::CBasicQualifier(boost::shared_ptr<database::IPluginEventLoggerRequester> pluginLogger,
                                    boost::shared_ptr<dataAccessLayer::IEventLogger> mainLogger)
      : m_pluginLogger(pluginLogger),
        m_mainLogger(mainLogger)
   {
   }

   CBasicQualifier::~CBasicQualifier()
   {
   }

   void CBasicQualifier::signalLoad(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      // Insert event in database
      AddEventToDatabase(pluginInformation, database::entities::EEventType::kLoad);
   }

   void CBasicQualifier::signalUnload(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      // Insert event in database
      AddEventToDatabase(pluginInformation, database::entities::EEventType::kUnload);
   }

   void CBasicQualifier::signalCrash(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, const std::string& reason)
   {
      // Insert event in database
      AddEventToDatabase(pluginInformation, database::entities::EEventType::kCrash, reason);
   }

   void CBasicQualifier::AddEventToDatabase(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, database::entities::EEventType eventType, const std::string& reason) const
   {
      try
      {
         // Add event into plugin event logger table
         m_pluginLogger->addEvent(
            pluginInformation->getType(),
            pluginInformation->getVersion().toString(),
            eventType,
            reason);

         // Only crashes have to be logged in the main event logger table
         if (eventType == database::entities::EEventType::kCrash)
            m_mainLogger->addEvent(database::entities::ESystemEventCode::kPluginCrash, "plugin " + pluginInformation->getIdentity(), reason);
      }
      catch (shared::exception::CEmptyResult& e)
      {
         // Just log the error
         YADOMS_LOG(error) << "Error when adding plugin " << eventType << " event in database : " << e.what();
      }
   }

   bool CBasicQualifier::isSafe(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      // With basic qualifier, plugin is always considered as safe
      return true;
   }

   int CBasicQualifier::getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      // With basic qualifier, plugin is always considered as perfect
      return 100;
   }
} // namespace pluginSystem


