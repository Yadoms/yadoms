#pragma once
#include <shared/plugin/information/IInformation.h>
#include "IInstance.h"
#include "database/sqlite/requesters/Plugin.h"
#include "database/IDataProvider.h"
#include "dataAccessLayer/IDataAccessLayer.h"
#include "IQualifier.h"
#include <shared/event/EventHandler.hpp>

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	Class used to read plugin informations
   //--------------------------------------------------------------
   class IFactory
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Map of all valid plugins (key are plugin file names, values are plugin information)
      //--------------------------------------------------------------
      typedef std::map<std::string, boost::shared_ptr<const shared::plugin::information::IInformation> > AvailablePluginMap;


      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IFactory()
      {
      }

      //--------------------------------------------------------------
      /// \brief	                     Find all installed plugins
      /// \return                      All installed plugins found (included not supported)
      //--------------------------------------------------------------
      virtual AvailablePluginMap findAvailablePlugins() const = 0;

      //--------------------------------------------------------------
      /// \brief	                     Create a plugin instance
      /// \param [in]	pluginInformation          Information on the plugin
      /// \param [in]   instanceData               the plugin instance data
      /// \param [in]   dataProvider               the database accessor
      /// \param [in]   dataAccessLayer            the data access layer
      /// \param [in]   qualifier                  the plugin qualifier
      /// \param [in]   managerEventHandler        the manager event handler
      /// \param [in]   normalStopEventId          Id of the normal instance stop event
      /// \param [in]   abnormalStopEventId        Id of the abnormal instance stop event
      /// \return                      The plugin instance
      //--------------------------------------------------------------
      virtual boost::shared_ptr<IInstance> createInstance(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                          boost::shared_ptr<database::IDataProvider> dataProvider,
                                                          boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                          const boost::shared_ptr<IQualifier> qualifier,
                                                          boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
                                                          int normalStopEventId,
                                                          int abnormalStopEventId) const = 0;
   };
} // namespace pluginSystem
