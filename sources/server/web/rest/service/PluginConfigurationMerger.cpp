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
                  out->set(item.first + ".content", mergeConfigurationAndSchema(
                              item.second->get<shared::CDataContainer>("content"),
                              instanceConfiguration.get<shared::CDataContainer>(item.first + ".content")));
               else if (parameterType == "string")
                  out->set(item.first + ".value", instanceConfiguration.get<std::string>(item.first));
               else if (parameterType == "bool")
                  out->set(item.first + ".value", instanceConfiguration.get<bool>(item.first));
               else if (parameterType == "int")
                  out->set(item.first + ".value", instanceConfiguration.get<int>(item.first));
               else if (parameterType == "decimal")
                  out->set(item.first + ".value", instanceConfiguration.get<double>(item.first));
               else if (parameterType == "enum")
                  out->set(item.first + ".value", instanceConfiguration.get<std::string>(item.first));
               else
               {
                  //TODO vérifier que tous les types soient gérés
               }
            }

            return out;
         }

         boost::shared_ptr<shared::CDataContainer> CPluginConfigurationMerger::extractConfiguration(
            const shared::CDataContainer& instanceConfigurationAndSchema) const
         {
            auto out = shared::CDataContainer::make();

            for (const auto& item : instanceConfigurationAndSchema.getAsMap<boost::shared_ptr<shared::CDataContainer>>())
            {
               const auto parameterType = item.second->get("type");

               if (parameterType == "section")
                  out->set(item.first + ".content",
                           extractConfiguration(instanceConfigurationAndSchema.get<shared::CDataContainer>(item.first + ".content")));
               else if (parameterType == "string")
                  out->set(item.first, instanceConfigurationAndSchema.get<std::string>(item.first + ".value"));
               else if (parameterType == "bool")
                  out->set(item.first, instanceConfigurationAndSchema.get<bool>(item.first + ".value"));
               else if (parameterType == "int")
                  out->set(item.first, instanceConfigurationAndSchema.get<int>(item.first + ".value"));
               else if (parameterType == "decimal")
                  out->set(item.first, instanceConfigurationAndSchema.get<double>(item.first + ".value"));
               else if (parameterType == "enum")
                  out->set(item.first, instanceConfigurationAndSchema.get<std::string>(item.first + ".value"));
               else
               {
                  //TODO vérifier que tous les types soient gérés
               }
            }

            return out;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
