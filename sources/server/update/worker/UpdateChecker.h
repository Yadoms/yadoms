#pragma once
#include "IUpdateChecker.h"
#include "shared/event/EventHandler.hpp"
#include "pluginSystem/IFactory.h"
#include "pluginSystem/Manager.h"

namespace update
{
   namespace worker
   {
      class CUpdateChecker : public IUpdateChecker
      {
      public:
         CUpdateChecker(const boost::posix_time::time_duration firstScanDelay,
                        const boost::posix_time::time_duration nextScanDelays,
                        boost::shared_ptr<pluginSystem::CManager> pluginManager,
                        boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger);
         virtual ~CUpdateChecker();

         // IUpdateChecker Implementation
         void forceRebuildUpdates() override;
         shared::CDataContainer getUpdates(bool includePreleases) const override;
         // [END] IUpdateChecker Implementation

      protected:
         void doWork(const boost::posix_time::time_duration firstScanDelay,
                     const boost::posix_time::time_duration nextScanDelays);
         void scan();
         shared::CDataContainer buildUpdates(bool includePreleases);
         shared::CDataContainer buildPluginList(const pluginSystem::IFactory::AvailablePluginMap& localVersions,
                                                const shared::CDataContainer& availableVersions,
                                                bool includePreleases);
         static shared::CDataContainer extractReleases(const shared::CDataContainer& inUpdates);
         static shared::CDataContainer extractReleasesFromVersionsNode(const shared::CDataContainer& inVersionsNode);

      private:
         boost::shared_ptr<pluginSystem::CManager> m_pluginManager;
         boost::shared_ptr<dataAccessLayer::IEventLogger> m_eventLogger;

         boost::thread m_thread;
         shared::event::CEventHandler m_evtHandler;

         mutable boost::recursive_mutex m_updateMutex;
         shared::CDataContainer m_allUpdates;
         shared::CDataContainer m_releasesOnlyUpdates;
         bool m_updatesAvailable;
      };
   } // namespace worker
} // namespace update
