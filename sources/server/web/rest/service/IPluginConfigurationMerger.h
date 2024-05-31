#pragma once
#include <boost/shared_ptr.hpp>

#include "shared/DataContainer.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         class IPluginConfigurationMerger
         {
         public:
            IPluginConfigurationMerger() = default;
            virtual ~IPluginConfigurationMerger() = default;
            IPluginConfigurationMerger(const IPluginConfigurationMerger&) = delete;
            IPluginConfigurationMerger(IPluginConfigurationMerger&&) = delete;
            IPluginConfigurationMerger& operator=(const IPluginConfigurationMerger&) = delete;
            IPluginConfigurationMerger& operator=(const IPluginConfigurationMerger&&) = delete;

            virtual boost::shared_ptr<shared::CDataContainer> mergeConfigurationAndSchema(
               const shared::CDataContainer& configurationSchema,
               const shared::CDataContainer& instanceConfiguration) const = 0;

            virtual boost::shared_ptr<shared::CDataContainer> extractConfiguration(
               const shared::CDataContainer& instanceConfigurationAndSchema) const = 0;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
