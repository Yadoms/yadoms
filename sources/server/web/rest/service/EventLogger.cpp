#include "stdafx.h"
#include "EventLogger.h"

#include "web/poco/RestDispatcherHelpers.hpp"
#include "web/poco/RestResult.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         CEventLogger::CEventLogger(boost::shared_ptr<dataAccessLayer::IEventLogger> dataProvider)
            : m_dataProvider(dataProvider),
              m_restKeyword("EventLogger")
         {
         }

         void CEventLogger::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CEventLogger::getEvents);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("last"), CEventLogger::getLastEvent);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("from")("*"), CEventLogger::getEventsFrom);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("limit")("*")("*"), CEventLogger::getEventsRange);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword), CEventLogger::addEvent);
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CEventLogger::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            //TODO (lorsque l'EventLogger sera refactoré)

            return m_endPoints;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CEventLogger::getEvents(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               auto dvList = m_dataProvider->getEvents();
               shared::CDataContainer collection;
               collection.set(m_restKeyword, dvList);
               return poco::CRestResult::GenerateSuccess(collection);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in reading events");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CEventLogger::getLastEvent(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const
         {
            try
            {
               auto lastEvent = m_dataProvider->getLastEvent();
               return poco::CRestResult::GenerateSuccess(lastEvent);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in reading the last events");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CEventLogger::getEventsFrom(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  auto eventIdFrom = boost::lexical_cast<int>(parameters[2]);
                  auto eventList = m_dataProvider->getEventsFrom(eventIdFrom);
                  shared::CDataContainer collection;
                  collection.set(m_restKeyword, eventList);
                  return poco::CRestResult::GenerateSuccess(collection);
               }
               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve Id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in reading events");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CEventLogger::getEventsRange(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 3)
               {
                  auto offset = boost::lexical_cast<int>(parameters[2]);
                  auto count = boost::lexical_cast<int>(parameters[3]);
                  auto eventList = m_dataProvider->getEventsRange(offset, count);
                  shared::CDataContainer collection;
                  collection.set(m_restKeyword, eventList);
                  return poco::CRestResult::GenerateSuccess(collection);
               }
               return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve Id in url");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in reading events");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CEventLogger::addEvent(const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const
         {
            try
            {
               database::entities::CEventLogger entityToAdd;
               entityToAdd.fillFromSerializedString(requestContent);
               m_dataProvider->addEvent(entityToAdd);
               return poco::CRestResult::GenerateSuccess();
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new event entry");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 


