﻿#include "stdafx.h"

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
               if (!item.second->getWithDefault<bool>("show", true))
               {
                  out->remove(item.first);
                  continue;
               }

               const auto parameterType = item.second->get("type");

               if (parameterType == "section"
                  || parameterType == "multiSelectSection"
                  || parameterType == "radioSection"
                  || parameterType == "comboSection")
               {
                  if (item.second->exists("content"))
                     out->set(item.first + ".content",
                              mergeConfigurationAndSchema(item.second->get<shared::CDataContainer>("content"),
                                                          instanceConfiguration.getWithDefault<shared::CDataContainer>(
                                                             item.first + ".content", {})));

                  if (parameterType == "radioSection"
                     || parameterType == "comboSection")
                  {
                     out->set(item.first + ".activeSection",
                              instanceConfiguration.getWithDefault<std::string>(item.first + ".activeSection", ""));
                  }
               }
               else if (parameterType == "checkboxSection")
               {
                  if (item.second->exists("content"))
                     out->set(item.first + ".content",
                              mergeConfigurationAndSchema(item.second->get<shared::CDataContainer>("content"),
                                                          instanceConfiguration.getWithDefault<shared::CDataContainer>(
                                                             item.first + ".content", {})));

                  out->set(item.first + ".checkbox",
                           instanceConfiguration.getWithDefault<bool>(
                              item.first + ".checkbox",
                              out->getWithDefault<bool>(item.first + ".defaultValue", false)));
               }
               else
               {
                  if (parameterType == "string"
                     || parameterType == "time")
                  {
                     out->set(item.first + ".value",
                              instanceConfiguration.getWithDefault<std::string>(
                                 item.first,
                                 out->getWithDefault<std::string>(item.first + ".defaultValue", "")));
                  }
                  else if (parameterType == "enum")
                  {
                     out->set(item.first + ".value",
                              instanceConfiguration.getWithDefault<std::string>(
                                 item.first,
                                 out->getWithDefault<std::string>(
                                    item.first + ".defaultValue",
                                    out->exists(item.first + ".values") && !out->getKeys(item.first + ".values").empty()
                                       ? out->getKeys(item.first + ".values").at(0)
                                       : "")));
                  }
                  else if (parameterType == "bool")
                  {
                     out->set(item.first + ".value",
                              instanceConfiguration.getWithDefault<bool>(
                                 item.first,
                                 out->getWithDefault<bool>(item.first + ".defaultValue", false)));
                  }
                  else if (parameterType == "int")
                  {
                     out->set(item.first + ".value",
                              instanceConfiguration.getWithDefault<int>(
                                 item.first,
                                 out->getWithDefault<int>(item.first + ".defaultValue", 0)));
                  }
                  else if (parameterType == "decimal")
                  {
                     out->set(item.first + ".value",
                              instanceConfiguration.getWithDefault<double>(
                                 item.first,
                                 out->getWithDefault<double>(item.first + ".defaultValue", 0.0)));
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