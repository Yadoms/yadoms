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
            const shared::CDataContainer& instanceConfiguration)
         {
            boost::shared_ptr<shared::CDataContainer> out = configurationSchema.copy();

            for (const auto& item : out->getAsMap<boost::shared_ptr<shared::CDataContainer>>())
            {
               const auto parameterType = item.second->get("type");

               if (parameterType == "section"
                  || parameterType == "multiSelectSection"
                  || parameterType == "radioSection"
                  || parameterType == "comboSection"
                  || parameterType == "checkboxSection")
               {
                  if (instanceConfiguration.exists(item.first + ".content"))
                     out->set(item.first + ".content", mergeConfigurationAndSchema(
                                 item.second->get<shared::CDataContainer>("content"),
                                 instanceConfiguration.get<shared::CDataContainer>(item.first + ".content")));
                  if (instanceConfiguration.exists(item.first + ".activeSection"))
                     out->set(item.first + ".activeSection", instanceConfiguration.get<std::string>(item.first + ".activeSection"));
                  if (instanceConfiguration.exists(item.first + ".checkbox"))
                     out->set(item.first + ".checkbox", instanceConfiguration.get<bool>(item.first + ".checkbox"));
               }
               else
               {
                  if (!instanceConfiguration.exists(item.first))
                     continue;

                  if (parameterType == "string"
                     || parameterType == "enum"
                     || parameterType == "time")
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
                  else
                  {
                     YADOMS_LOG(error) << "CPluginConfigurationMerger::mergeConfigurationAndSchema : Invalid item type " << parameterType <<
                        ", ignored";
                  }
               }
            }

            return out;
         }

         boost::shared_ptr<shared::CDataContainer> CPluginConfigurationMerger::extractConfiguration(
            const shared::CDataContainer& instanceConfigurationAndSchema)
         {
            auto out = shared::CDataContainer::make();

            for (const auto& item : instanceConfigurationAndSchema.getAsMap<boost::shared_ptr<shared::CDataContainer>>())
            {
               const auto parameterType = item.second->get("type");

               if (parameterType == "section"
                  || parameterType == "multiSelectSection"
                  || parameterType == "radioSection"
                  || parameterType == "comboSection"
                  || parameterType == "checkboxSection")
               {
                  out->set(item.first + ".content",
                           extractConfiguration(instanceConfigurationAndSchema.get<shared::CDataContainer>(item.first + ".content")));
                  if (instanceConfigurationAndSchema.exists(item.first + ".activeSection"))
                     out->set(item.first + ".activeSection", instanceConfigurationAndSchema.get<std::string>(item.first + ".activeSection"));
                  if (instanceConfigurationAndSchema.exists(item.first + ".checkbox"))
                     out->set(item.first + ".checkbox", instanceConfigurationAndSchema.get<bool>(item.first + ".checkbox"));
               }
               else if (parameterType == "string"
                  || parameterType == "enum"
                  || parameterType == "time")
               {
                  out->set(item.first, instanceConfigurationAndSchema.get<std::string>(item.first + ".value"));
               }
               else if (parameterType == "bool")
               {
                  out->set(item.first, instanceConfigurationAndSchema.get<bool>(item.first + ".value"));
               }
               else if (parameterType == "int")
               {
                  out->set(item.first, instanceConfigurationAndSchema.get<int>(item.first + ".value"));
               }
               else if (parameterType == "decimal")
               {
                  out->set(item.first, instanceConfigurationAndSchema.get<double>(item.first + ".value"));
               }
               else
               {
                  YADOMS_LOG(error) << "CPluginConfigurationMerger::extractConfiguration : Invalid item type " << parameterType << ", ignored";
               }
            }

            return out;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
