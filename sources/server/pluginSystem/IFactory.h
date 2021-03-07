#pragma once
#include <shared/plugin/information/IInformation.h>
#include "IInstance.h"
#include "database/entities/Entities.h"
#include "database/IDataProvider.h"
#include "dataAccessLayer/IDataAccessLayer.h"
#include "IQualifier.h"

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	Class used to read plugin information
   //--------------------------------------------------------------
   class IFactory
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Map of all valid plugins (key are plugin file names, values are plugin information)
      //--------------------------------------------------------------
      typedef std::map<std::string, boost::shared_ptr<const shared::plugin::information::IInformation>> AvailablePluginMap;


      virtual ~IFactory() = default;

      //--------------------------------------------------------------
      /// \brief	                     Find all installed plugins
      /// \return                      All installed plugins found (included not supported)
      //--------------------------------------------------------------
      virtual AvailablePluginMap findAvailablePlugins() const = 0;

      //--------------------------------------------------------------
      /// \brief	                     Create a plugin instance
      /// \param [in]   instanceData               the plugin instance data
      /// \param [in]   dataProvider               the database accessor
      /// \param [in]   dataAccessLayer            the data access layer
      /// \param [in]   qualifier                  the plugin qualifier
      /// \param [in]   onPluginsStoppedFct        Function called when plugin is stopped
      /// \return                      The plugin instance
      //--------------------------------------------------------------
      virtual boost::shared_ptr<IInstance> createInstance(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                          boost::shared_ptr<database::IDataProvider> dataProvider,
                                                          boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer,
                                                          boost::shared_ptr<IQualifier> qualifier,
                                                          boost::function1<void, int> onPluginsStoppedFct) const = 0;

      //--------------------------------------------------------------
      /// \brief	                  Get the plugin instance log file path
      /// \param [in]	instanceId  The plugin instance id
      /// \return                   The plugin instance log file path
      //--------------------------------------------------------------
      virtual boost::filesystem::path pluginLogFile(int instanceId) const = 0;

      //--------------------------------------------------------------
      /// \brief	                  Get the plugin instance data path
      /// \param [in]	instanceId  The plugin instance id
      /// \return                   The plugin instance data path
      //--------------------------------------------------------------
      virtual boost::filesystem::path pluginDataPath(int instanceId) const = 0;
   };
} // namespace pluginSystem
