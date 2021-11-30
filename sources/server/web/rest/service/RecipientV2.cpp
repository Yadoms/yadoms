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
               // ID
               const auto userId = request->pathVariableExists("id")
                                      ? boost::make_optional(static_cast<int>(std::stol(request->pathVariable("id"))))
                                      : boost::optional<int>();

               // Filtering
               const auto fromFirstName = request->queryParamExists("from-firstname")
                                             ? boost::make_optional(request->queryParam("from-firstname"))
                                             : boost::optional<std::string>();
               const auto fromLastName = request->queryParamExists("from-lastname")
                                            ? boost::make_optional(request->queryParam("from-lastname"))
                                            : boost::optional<std::string>();

               // Process the request
               const auto users = m_dataProvider->getRecipientRequester()->getUsers(userId,
                                                                                    fromFirstName,
                                                                                    fromLastName);

               if (users.empty())
                  return boost::make_shared<CNoContentAnswer>();

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> userEntries;
               for (const auto& user : users)
               {
                  auto keywordEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("id") != props->end())
                     keywordEntry->set("id", user->Id());
                  if (props->empty() || props->find("first-name") != props->end())
                     keywordEntry->set("first-name", user->FirstName());
                  if (props->empty() || props->find("last-name") != props->end())
                     keywordEntry->set("last-name", user->LastName());

                  userEntries.push_back(keywordEntry);
               }

               shared::CDataContainer container;
               container.set("keywords", userEntries);
               return boost::make_shared<CSuccessAnswer>(container);
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
            //TODO ajouter les fields (ainsi que dans les autres requêtes)
            //TODO si field : faire une transaction ?
            try
            {
               database::entities::CRecipient user;
               user.fillFromSerializedString(request->body());

               if (!user.FirstName.isDefined()
                  || !user.LastName.isDefined())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "invalid user creation request. Need at least firstname and lastname");

               const auto newUserId = m_dataProvider->getRecipientRequester()->createUser(user);
               return boost::make_shared<CCreatedAnswer>("users/" + std::to_string(newUserId));
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create user");
            }
         }

         boost::shared_ptr<IAnswer> CRecipient::updateUserV2(const boost::shared_ptr<IRequest>& request) const
         {
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
      } //namespace service
   } //namespace rest
} //namespace web 
