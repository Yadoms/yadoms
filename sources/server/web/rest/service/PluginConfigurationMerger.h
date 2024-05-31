#pragma once
#include <boost/shared_ptr.hpp>

#include "IPluginConfigurationMerger.h"
#include "pluginSystem/Manager.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         class CPluginConfigurationMerger final : public IPluginConfigurationMerger // TODO rendre static ?
         {
         public:
            explicit CPluginConfigurationMerger() = default;

            ~CPluginConfigurationMerger() override = default;
            CPluginConfigurationMerger(const CPluginConfigurationMerger&) = delete;
            CPluginConfigurationMerger(CPluginConfigurationMerger&&) = delete;
            CPluginConfigurationMerger& operator=(const CPluginConfigurationMerger&) = delete;
            CPluginConfigurationMerger& operator=(const CPluginConfigurationMerger&&) = delete;

            boost::shared_ptr<shared::CDataContainer> mergeConfigurationAndSchema(
               const shared::CDataContainer& configurationSchema,
               const shared::CDataContainer& instanceConfiguration) const override;

            boost::shared_ptr<shared::CDataContainer> extractConfiguration(
               const shared::CDataContainer& instanceConfigurationAndSchema) const override;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
