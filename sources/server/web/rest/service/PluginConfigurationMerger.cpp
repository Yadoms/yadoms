#include "stdafx.h"

#include "PluginConfigurationMerger.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         boost::shared_ptr<shared::CDataContainer> CPluginConfigurationMerger::mergeConfigurationAndSchema(
            const shared::CDataContainer& configurationSchema,
            const shared::CDataContainer& instanceConfiguration) const
         {
            boost::shared_ptr<shared::CDataContainer> out = configurationSchema.copy();

            for (const auto& item : out->getAsMap<boost::shared_ptr<shared::CDataContainer>>())
            {
               const auto parameterType = item.second->get("type");
                  
               if (parameterType == "section")
               {
                  out->set(item.first + ".content", mergeConfigurationAndSchema(
                              item.second->get<shared::CDataContainer>("content"),
                              instanceConfiguration.get<shared::CDataContainer>(item.first + ".content")));
               }
               else if (parameterType == "string")
               {
                  out->set(item.first + ".value", instanceConfiguration.get<std::string>(item.first));
               }
               else if (parameterType == "bool")
               {
                  out->set(item.first + ".value", instanceConfiguration.get<bool>(item.first));
               }
               else if (parameterType == "int")
               {
                  out->set(item.first + ".value", instanceConfiguration.get<int>(item.first));
               }
               else if (parameterType == "decimal")
               {
                  out->set(item.first + ".value", instanceConfiguration.get<double>(item.first));
               }
               else if (parameterType == "enum")
               {
                  out->set(item.first + ".value", instanceConfiguration.get<std::string>(item.first));
               }
               else
               {
                  //TODO
               }
            }

            return out;
         }

         boost::shared_ptr<shared::CDataContainer> CPluginConfigurationMerger::extractConfiguration(
            const shared::CDataContainer& instanceConfigurationAndSchema) const
         {
            return nullptr; //TODO
         }
      } //namespace service
   } //namespace rest
} //namespace web 
