#pragma once
#include <boost/shared_ptr.hpp>

#include "pluginSystem/Manager.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         class CConfigurationMerger final
         {
         public:
            CConfigurationMerger() = delete;
            ~CConfigurationMerger() = delete;
            CConfigurationMerger(const CConfigurationMerger&) = delete;
            CConfigurationMerger(CConfigurationMerger&&) = delete;
            CConfigurationMerger& operator=(const CConfigurationMerger&) = delete;
            CConfigurationMerger& operator=(const CConfigurationMerger&&) = delete;

            static boost::shared_ptr<shared::CDataContainer> mergeConfigurationAndSchema(
               const shared::CDataContainer& configurationSchema,
               const shared::CDataContainer& instanceConfiguration);

            static boost::shared_ptr<shared::CDataContainer> extractConfiguration(
               const shared::CDataContainer& instanceConfigurationAndSchema);
         };
      } //namespace service
   } //namespace rest
} //namespace web 
