#pragma once
#include <shared/plugin/information/IInformation.h>
#include <shared/process/ILogger.h>
#include "IInstance.h"
#include "IInstanceStateHandler.h"
#include "database/sqlite/requesters/Plugin.h"
#include "database/IDataProvider.h"
#include "dataAccessLayer/IDeviceManager.h"
#include "dataAccessLayer/IAcquisitionHistorizer.h"
#include "dataAccessLayer/IEventLogger.h"
#include "IQualifier.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	Class used to read plugin informations
   //--------------------------------------------------------------
   class IFactory
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IFactory() {}

      //--------------------------------------------------------------
      /// \brief	                     Get plugin informations
      /// \param [in] pluginPath       The plugin path
      /// \return                      The plugin information
      /// \throw                       CInvalidPluginException if plugin is not recognized
      //--------------------------------------------------------------
      virtual boost::shared_ptr<const shared::plugin::information::IInformation> createInformation(
         const boost::filesystem::path& pluginPath) const = 0;

      //--------------------------------------------------------------
      /// \brief	                     Create a plugin instance
      /// \param [in]	pluginInformation          Information on the plugin
      /// \param [in]   instanceData               the plugin instance data
      /// \param [in]   dataProvider               the database accessor
      /// \param [in]   deviceManager              the device manager
      /// \param [in]   acquisitionHistorizer      the acquisition historizer
      /// \param [in]   qualifier                  the plugin qualifier
      /// \param [in]   supervisor                 the supervisor event handler
      /// \param [in]   pluginManagerEventId       The ID to use to send events to supervisor
      /// \return                      The plugin instance
      //--------------------------------------------------------------
      virtual boost::shared_ptr<IInstance> createInstance(
         const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
         boost::shared_ptr<const database::entities::CPlugin> instanceData,
         boost::shared_ptr<database::IDataProvider> dataProvider,
         boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
         boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
         const boost::shared_ptr<IQualifier> qualifier,
         boost::shared_ptr<shared::event::CEventHandler> supervisor,//TODO virer
         int pluginManagerEventId) const = 0;//TODO probablement à virer

      //--------------------------------------------------------------
      /// \brief	                     Create the process logger
      /// \return                      The process logger
      //--------------------------------------------------------------
      virtual boost::shared_ptr<shared::process::ILogger> createProcessLogger() const = 0;

      //--------------------------------------------------------------
      /// \brief	                     Create the instance state handler
      ///\param [in] dbRequester                   Database requester
      ///\param [in] eventLogger                   Main event logger
      ///\param [in] managerEventHandler           the manager event handler
      ///\param [in] instanceId                    the plugin instance ID
      /// \return                      The state handler
      //--------------------------------------------------------------
      virtual boost::shared_ptr<IInstanceStateHandler> createInstanceStateHandler(
         boost::shared_ptr<database::IPluginRequester> dbRequester,
         boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
         boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
         int instanceId) const;
   };
} // namespace pluginSystem
