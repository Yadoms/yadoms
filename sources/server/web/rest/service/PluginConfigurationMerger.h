#pragma once
#include <boost/shared_ptr.hpp>

#include "pluginSystem/Manager.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         class CPluginConfigurationMerger final
         {
         public:
            CPluginConfigurationMerger() = delete;
            ~CPluginConfigurationMerger() = delete;
            CPluginConfigurationMerger(const CPluginConfigurationMerger&) = delete;
            CPluginConfigurationMerger(CPluginConfigurationMerger&&) = delete;
            CPluginConfigurationMerger& operator=(const CPluginConfigurationMerger&) = delete;
            CPluginConfigurationMerger& operator=(const CPluginConfigurationMerger&&) = delete;

            static boost::shared_ptr<shared::CDataContainer> mergeConfigurationAndSchema(
               const shared::CDataContainer& configurationSchema,
               const shared::CDataContainer& instanceConfiguration);

            static boost::shared_ptr<shared::CDataContainer> extractConfiguration(
               const shared::CDataContainer& instanceConfigurationAndSchema);
         };
      } //namespace service
   } //namespace rest
} //namespace web 
