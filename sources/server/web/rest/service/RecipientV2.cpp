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
         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CRecipient::endPoints() //TODO renommer la classe en CUser ?
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "users", getUsersV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "users/fields", getFieldsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "users/{id}", getUsersV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "users", createUserV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "users/{id}", updateUserV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "users/{id}", deleteUserV2));

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CRecipient::getUsersV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req)-> boost::shared_ptr<IAnswer>
                  {
                     // ID
                     const auto userId = req->pathVariableExists("id")
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
                     const auto users = m_dataProvider->getRecipientRequester()->getUsers(userId,
                                                                                          fromFirstName,
                                                                                          fromLastName);

                     if (users.empty())
                        return boost::make_shared<CNoContentAnswer>();

                     // Get requested props
                     const auto props = req->queryParamAsList("prop");
                     std::vector<boost::shared_ptr<shared::CDataContainer>> userEntries;
                     for (const auto& user : users)
                     {
                        auto keywordEntry = boost::make_shared<shared::CDataContainer>();
                        if (props->empty() || props->find("id") != props->end())
                           keywordEntry->set("id", user->Id());
                        if (props->empty() || props->find("firstname") != props->end())
                           keywordEntry->set("firstname", user->FirstName());
                        if (props->empty() || props->find("lastname") != props->end())
                           keywordEntry->set("lastname", user->LastName());
                        if (props->empty() || props->find("fields") != props->end())
                        {
                           shared::CDataContainer fields;
                           for (const auto& availableField : getAvailableFields()->getAsMap<boost::shared_ptr<shared::CDataContainer>>())
                           {
                              const auto fieldNamePair = fromFieldName(availableField.first);

                              const auto userFields = m_dataProvider->getRecipientRequester()->getFields(user->Id(),
                                 fieldNamePair.first,
                                 fieldNamePair.second);

                              if (userFields.empty())
                                 continue;

                              const auto userField = userFields[0];
                              shared::CDataContainer value;
                              value.set("value", userField->Value());
                              fields.set(availableField.first, value);
                           }
                           keywordEntry->set("fields", fields);
                        }

                        userEntries.push_back(keywordEntry);
                     }

                     shared::CDataContainer container;
                     container.set("users", userEntries);
                     return boost::make_shared<CSuccessAnswer>(container);
                  });
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getUsers request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getUsers request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get users");
            }
         }

         boost::shared_ptr<IAnswer> CRecipient::createUserV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req)-> boost::shared_ptr<IAnswer>
                  {
                     const shared::CDataContainer body(req->body());

                     if (!body.exists("firstName")
                        || !body.exists("lastName"))
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "invalid user creation request. Need at least firstname and lastname");

                     // Create user
                     database::entities::CRecipient user;
                     user.FirstName = body.get<std::string>("firstName");
                     user.LastName = body.get<std::string>("lastName");
                     const auto newUserId = m_dataProvider->getRecipientRequester()->createUser(user);

                     // Add fields
                     if (body.exists("fields"))
                     {
                        for (const auto& field : body.getAsMap<boost::shared_ptr<shared::CDataContainer>>("fields"))
                        {
                           database::entities::CRecipientField fieldEntry;
                           fieldEntry.IdRecipient = newUserId;
                           const auto fieldNamePair = fromFieldName(field.first);
                           fieldEntry.PluginType = fieldNamePair.first;
                           fieldEntry.FieldName = fieldNamePair.second;

                           fieldEntry.Value = field.second->get<std::string>("value");
                           m_dataProvider->getRecipientRequester()->createField(fieldEntry);
                        }
                     }

                     return boost::make_shared<CCreatedAnswer>("users/" + std::to_string(newUserId));
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create user");
            }
         }

         boost::shared_ptr<IAnswer> CRecipient::updateUserV2(const boost::shared_ptr<IRequest>& request) const
         {
            //TODO ajouter les fields (ainsi que dans les autres requêtes)
            //TODO si field : faire une transaction ?
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "user id was not provided");
               const auto userId = static_cast<int>(std::stol(id));

               if (request->body().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "body was not provided");

               const auto body = shared::CDataContainer::make(request->body());

               if (body->empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "body was not provided");

               // Filter only client-modifiable fields
               database::entities::CRecipient userToUpdate;
               userToUpdate.Id = userId;
               if (body->exists("firstName"))
                  userToUpdate.FirstName = body->get<std::string>("firstName");
               if (body->exists("lastName"))
                  userToUpdate.LastName = body->get<std::string>("lastName");

               m_dataProvider->getRecipientRequester()->updateUser(userToUpdate);

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update user");
            }
         }

         boost::shared_ptr<IAnswer> CRecipient::deleteUserV2(const boost::shared_ptr<IRequest>& request) const
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
                                                                "user id was not provided");
                     const auto userId = static_cast<int>(std::stol(id));

                     m_dataProvider->getRecipientRequester()->removeRecipient(userId);

                     return boost::make_shared<CNoContentAnswer>();
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete user");
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
                                                       "Fail to get users fields");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
