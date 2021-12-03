#include <regex>

#include "stdafx.h"
#include "Recipient.h"
#include "RestEndPoint.h"
#include "web/rest/CreatedAnswer.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/Helpers.h"
#include "web/rest/NoContentAnswer.h"
#include "web/rest/SuccessAnswer.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CRecipient::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "recipients", getRecipientsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "recipients/fields", getFieldsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "recipients/{id}", getRecipientsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "recipients", createRecipientV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "recipients/{id}", updateRecipientV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "recipients/{id}", deleteRecipientV2));

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CRecipient::getRecipientsV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req)-> boost::shared_ptr<IAnswer>
                  {
                     // ID
                     const auto recipientId = req->pathVariableExists("id")
                                                 ? boost::make_optional(static_cast<int>(std::stol(req->pathVariable("id"))))
                                                 : boost::optional<int>();

                     // Filtering
                     const auto fromFirstName = req->queryParamExists("from-firstname")
                                                   ? boost::make_optional(req->queryParam("from-firstname"))
                                                   : boost::optional<std::string>();
                     const auto fromLastName = req->queryParamExists("from-lastname")
                                                  ? boost::make_optional(req->queryParam("from-lastname"))
                                                  : boost::optional<std::string>();

                     // Process the request
                     const auto recipients = m_dataProvider->getRecipientRequester()->getRecipients(recipientId,
                        fromFirstName,
                        fromLastName);

                     if (recipients.empty())
                        return boost::make_shared<CNoContentAnswer>();

                     // Get requested props
                     const auto props = req->queryParamAsList("prop");
                     std::vector<boost::shared_ptr<shared::CDataContainer>> recipientEntries;
                     for (const auto& recipient : recipients)
                     {
                        auto keywordEntry = boost::make_shared<shared::CDataContainer>();
                        if (props->empty() || props->find("id") != props->end())
                           keywordEntry->set("id", recipient->Id());
                        if (props->empty() || props->find("firstname") != props->end())
                           keywordEntry->set("firstname", recipient->FirstName());
                        if (props->empty() || props->find("lastname") != props->end())
                           keywordEntry->set("lastname", recipient->LastName());
                        if (props->empty() || props->find("fields") != props->end())
                        {
                           shared::CDataContainer fields;
                           for (const auto& availableField : this->getAvailableFields()->getAsMap<boost::shared_ptr<shared::CDataContainer>>())
                           {
                              const auto fieldNamePair = this->fromFieldName(availableField.first);

                              const auto recipientFields = m_dataProvider->getRecipientRequester()->getFields(recipient->Id(),
                                 fieldNamePair.first,
                                 fieldNamePair.second);

                              if (recipientFields.empty())
                                 continue;

                              const auto recipientField = recipientFields[0];
                              shared::CDataContainer value;
                              value.set("value", recipientField->Value());
                              fields.set(availableField.first, value);
                           }
                           keywordEntry->set("fields", fields);
                        }

                        recipientEntries.push_back(keywordEntry);
                     }

                     shared::CDataContainer container;
                     container.set("recipients", recipientEntries);
                     return boost::make_shared<CSuccessAnswer>(container);
                  });
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getRecipients request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getRecipients request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get recipients");
            }
         }

         boost::shared_ptr<IAnswer> CRecipient::createRecipientV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req)-> boost::shared_ptr<IAnswer>
                  {
                     if (req->body().empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "body was not provided");

                     const shared::CDataContainer body(req->body());

                     if (!body.exists("firstName")
                        || !body.exists("lastName"))
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "invalid recipient creation request. Need at least firstname and lastname");

                     // Create recipient
                     database::entities::CRecipient recipient;
                     recipient.FirstName = body.get<std::string>("firstName");
                     recipient.LastName = body.get<std::string>("lastName");
                     const auto newRecipientId = m_dataProvider->getRecipientRequester()->createRecipient(recipient);

                     // Add fields
                     if (body.exists("fields"))
                     {
                        for (const auto& field : body.getAsMap<boost::shared_ptr<shared::CDataContainer>>("fields"))
                        {
                           database::entities::CRecipientField fieldEntry;
                           fieldEntry.IdRecipient = newRecipientId;
                           const auto fieldNamePair = this->fromFieldName(field.first);
                           fieldEntry.PluginType = fieldNamePair.first;
                           fieldEntry.FieldName = fieldNamePair.second;

                           fieldEntry.Value = field.second->get<std::string>("value");
                           m_dataProvider->getRecipientRequester()->createField(fieldEntry);
                        }
                     }

                     return boost::make_shared<CCreatedAnswer>("recipients/" + std::to_string(newRecipientId));
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create recipient");
            }
         }

         boost::shared_ptr<IAnswer> CRecipient::updateRecipientV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req)-> boost::shared_ptr<IAnswer>
                  {
                     // ID
                     const auto id = req->pathVariable("id", std::string());
                     if (id.empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "recipient id was not provided");
                     const auto recipientId = static_cast<int>(std::stol(id));

                     if (req->body().empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "body was not provided");

                     const shared::CDataContainer body(req->body());

                     if (body.empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "body was not provided");

                     // Update recipient
                     database::entities::CRecipient recipient;
                     recipient.Id = recipientId;
                     if (body.exists("firstName"))
                        recipient.FirstName = body.get<std::string>("firstName");
                     if (body.exists("lastName"))
                        recipient.LastName = body.get<std::string>("lastName");
                     m_dataProvider->getRecipientRequester()->updateRecipientV2(recipient);

                     // Update fields
                     if (body.exists("fields"))
                     {
                        for (const auto& field : body.getAsMap<boost::shared_ptr<shared::CDataContainer>>("fields"))
                        {
                           database::entities::CRecipientField fieldEntry;
                           fieldEntry.IdRecipient = recipientId;
                           const auto fieldNamePair = this->fromFieldName(field.first);
                           fieldEntry.PluginType = fieldNamePair.first;
                           fieldEntry.FieldName = fieldNamePair.second;

                           fieldEntry.Value = field.second->get<std::string>("value");
                           m_dataProvider->getRecipientRequester()->updateField(fieldEntry);
                        }
                     }

                     return boost::make_shared<CNoContentAnswer>();
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update recipient");
            }
         }

         boost::shared_ptr<IAnswer> CRecipient::deleteRecipientV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     // ID
                     const auto id = req->pathVariable("id", std::string());
                     if (id.empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "recipient id was not provided");
                     const auto recipientId = static_cast<int>(std::stol(id));

                     m_dataProvider->getRecipientRequester()->removeRecipient(recipientId);

                     return boost::make_shared<CNoContentAnswer>();
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete recipient");
            }
         }

         std::unique_ptr<shared::CDataContainer> CRecipient::getAvailableFields() const
         {
            // List concerned plugins (plugins which have instances)
            const auto allPlugins = m_pluginManager->getPluginList();
            std::map<std::string, boost::shared_ptr<const shared::plugin::information::IInformation>> plugins;
            for (const auto& pluginInstance : m_pluginManager->getInstanceList())
            {
               if (pluginInstance->Category() == database::entities::EPluginCategory::kSystem)
               {
                  plugins[pluginInstance->Type()] = m_pluginManager->getRunningInstance(pluginInstance->Id())->aboutPlugin();
               }
               else
               {
                  if (plugins.find(pluginInstance->Type()) != plugins.end())
                     continue;
                  if (allPlugins.find(pluginInstance->Type()) == allPlugins.end())
                     continue;

                  plugins[pluginInstance->Type()] = allPlugins.at(pluginInstance->Type());
               }
            }

            // Extract recipient fields
            auto fields = shared::CDataContainer::makeUnique();
            for (const auto& plugin : plugins)
            {
               const auto package = plugin.second->getPackage();

               if (!package->containsChild("recipientFields"))
                  continue;

               const auto pluginRecipientFields = package->getAsMap<boost::shared_ptr<shared::CDataContainer>>("recipientFields");
               for (const auto& recipientField : pluginRecipientFields)
               {
                  auto fieldContent = boost::make_shared<shared::CDataContainer>();
                  fieldContent = recipientField.second;
                  fieldContent->set("i18nPath", (plugin.second->getPath() / "locales").string());
                  fields->set(plugin.second->getType() + "|" + recipientField.first, fieldContent);
               }
            }

            return fields;
         }

         std::pair<std::string, std::string> CRecipient::fromFieldName(const std::string& fieldName) const
         {
            std::smatch result;
            if (!std::regex_search(fieldName, result, std::regex("^([[:alnum:]]*)\\|(.*)$"))
               || result.size() != 3)
               throw std::runtime_error("Invalid field name");
            return std::make_pair(result[1], result[2]);
         }

         boost::shared_ptr<IAnswer> CRecipient::getFieldsV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               shared::CDataContainer container;
               container.set("fields", *getAvailableFields());
               return boost::make_shared<CSuccessAnswer>(container);
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getFields request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getFields request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get recipients fields");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
