#include "stdafx.h"
#include "Acquisition.h"
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "web/rest/StringContainer.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CAcquisition::m_restKeyword = std::string("acquisition");


         CAcquisition::CAcquisition(boost::shared_ptr<database::IDataProvider> dataProvider)
            : m_dataProvider(dataProvider)
         {
         }

         void CAcquisition::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("keyword")("lastdata"), CAcquisition::getKeywordListLastData);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("keyword")("info"), CAcquisition::getKeywordListInfo);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("keyword")("*")("info"), CAcquisition::getKeywordInfo);
            //get the last data of a list of keywords
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("lastdata"), CAcquisition::getKeywordLastData);
            //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*"), CAcquisition::getKeywordData); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("*"), CAcquisition::getKeywordData);
            //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("*")("*"), CAcquisition::getKeywordData);
            //get keyword data between two dates

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour"), CAcquisition::getKeywordDataByHour);
            //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour")("*"), CAcquisition::getKeywordDataByHour);
            //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour")("*")("*"), CAcquisition::getKeywordDataByHour);
            //get keyword data between two dates

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day"), CAcquisition::getKeywordDataByDay);
            //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day")("*"), CAcquisition::getKeywordDataByDay);
            //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day")("*")("*"), CAcquisition::getKeywordDataByDay);
            //get keyword data between two dates

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("month"), CAcquisition::getKeywordDataByMonth);
            //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("month")("*"), CAcquisition::getKeywordDataByMonth);
            //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("month")("*")("*"), CAcquisition::getKeywordDataByMonth);
            //get keyword data between two dates

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("year"), CAcquisition::getKeywordDataByYear);
            //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("year")("*"), CAcquisition::getKeywordDataByYear);
            //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("year")("*")("*"), CAcquisition::getKeywordDataByYear);
            //get keyword data between two dates
         }

         const std::string& CAcquisition::getRestKeyword()
         {
            return m_restKeyword;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordLastData(const std::vector<std::string>& parameters,
                                                                                                      const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);
                  shared::CDataContainer result;
                  readKeywordInfo(keywordId, "lastValue", result);

                  return CResult::GenerateSuccess(result);
               }
               return CResult::GenerateError("invalid parameter.");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving one acquisition");
            }
         }

         void CAcquisition::readKeywordInfo(int keywordId,
                                            const std::string& info,
                                            shared::CDataContainer& keywordResult) const
         {
            try
            {
               if (info == "exist")
               {
                  try
                  {
                     // ReSharper disable once CppExpressionWithoutSideEffects
                     m_dataProvider->getKeywordRequester()->getKeyword(keywordId);
                     keywordResult.set(info, true);
                  }
                  catch (shared::exception::CEmptyResult&)
                  {
                     keywordResult.set(info, false);
                  }
               }
               else if (info == "lastValue")
               {
                  const auto lastAcq = m_dataProvider->getKeywordRequester()->getKeywordLastAcquisition(keywordId, false);
                  keywordResult.set(info, lastAcq ? lastAcq->Value : std::string());
               }
               else if (info == "lastValueDate")
               {
                  const auto lastAcq = m_dataProvider->getKeywordRequester()->getKeywordLastAcquisition(keywordId, false);
                  if (lastAcq)
                     keywordResult.set(info, lastAcq->Date);
                  else
                     keywordResult.set(info, std::string());
               }
               else if (info == "friendlyName")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->FriendlyName());
               }
               else if (info == "deviceId")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->DeviceId);
               }
               else if (info == "pluginId")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getDeviceRequester()->getDevice(
                                       m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->DeviceId)->PluginId);
               }
               else if (info == "capacity")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->CapacityName);
               }
               else if (info == "accessMode")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->AccessMode().toString());
               }
               else if (info == "dataType")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->Type().toString());
               }
               else if (info == "unit")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->Units());
               }
               else if (info == "typeInfo")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->TypeInfo());
               }
               else if (info == "measure")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->Measure().toString());
               }
               else if (info == "details")
               {
                  keywordResult.set(info,
                                    m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->Details());
               }
               else
               {
                  YADOMS_LOG(warning) << "readKeywordInfo, can not get requested keyword " << keywordId << " info \"" << info <<
                     "\", ignored : unknown info label";
               }
            }
            catch (std::exception& exception)
            {
               YADOMS_LOG(warning) << "readKeywordInfo, can not get requested keyword " << keywordId << " info \"" << info <<
                  "\", ignored : " << exception.what();
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordListLastData(const std::vector<std::string>& parameters,
                                                                                                          const std::string& requestContent) const
         {
            try
            {
               shared::CDataContainer content(requestContent);
               auto keywords = content.get<std::vector<int>>("keywords");

               // erase all duplicates, if any
               keywords.erase(unique(keywords.begin(), keywords.end()), keywords.end());

               shared::CDataContainer result;
               for (const auto& keywordId : keywords)
               {
                  shared::CDataContainer keywordResult;
                  readKeywordInfo(keywordId, "lastValue", keywordResult);
                  result.set(std::to_string(keywordId),
                             keywordResult);
               }
               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving one acquisition");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordListInfo(const std::vector<std::string>& parameters,
                                                                                                      const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  shared::CDataContainer content(requestContent);
                  content.printToLog(YADOMS_LOG(information));
                  if (content.containsChildArray("keywords"))
                  {
                     auto keywords = content.get<std::vector<int>>("keywords");
                     const auto infoRequested = content.getWithDefault<std::vector<std::string>>("info", std::vector<std::string>());

                     // erase all duplicates, if any
                     sort(keywords.begin(), keywords.end());
                     keywords.erase(unique(keywords.begin(), keywords.end()), keywords.end());

                     shared::CDataContainer result;
                     for (const auto& keywordId : keywords)
                     {
                        shared::CDataContainer keywordResult;
                        for (const auto& info : infoRequested)
                           readKeywordInfo(keywordId, info, keywordResult);

                        result.set(std::to_string(keywordId),
                                   keywordResult);
                     }
                     return CResult::GenerateSuccess(result);
                  }
                  return CResult::GenerateError("invalid parameter. Can not retrieve keywords in request content");
               }
               return CResult::GenerateError("invalid parameter.");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving one acquisition");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordData(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 2)
               {
                  //get device id from URL
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 3)
                     timeFrom = boost::posix_time::from_iso_string(parameters[3]);

                  if (parameters.size() > 4)
                     timeTo = boost::posix_time::from_iso_string(parameters[4]);

                  auto allData = m_dataProvider->getAcquisitionRequester()->getKeywordData(keywordId,
                                                                                           timeFrom,
                                                                                           timeTo);
                  std::vector<shared::CDataContainer> objectList;

                  for (auto& i : allData)
                  {
                     shared::CDataContainer result;
                     result.set("date", boost::posix_time::to_iso_string(i.get<0>()));
                     result.set("key", i.get<1>());
                     objectList.push_back(result);
                  }

                  shared::CDataContainer result;
                  result.set<std::vector<shared::CDataContainer>>("data", objectList);
                  return CResult::GenerateSuccess(result);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve parameters in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in reading device data");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordInfo(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  shared::CDataContainer content(requestContent);
                  const auto infoRequested = content.getWithDefault<std::vector<std::string>>("info",
                                                                                              std::vector<std::string>());

                  shared::CDataContainer result;

                  shared::CDataContainer keywordResult;
                  for (const auto& info : infoRequested)
                     readKeywordInfo(keywordId, info, keywordResult);

                  result.set(std::to_string(keywordId),
                             keywordResult);

                  return CResult::GenerateSuccess(result);
               }
               return CResult::GenerateError("invalid parameter.");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving one acquisition");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordDataByHour(const std::vector<std::string>& parameters,
                                                                                                        const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 2)
               {
                  //get device id from URL
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 4)
                     timeFrom = boost::posix_time::from_iso_string(parameters[4]);

                  if (parameters.size() > 5)
                     timeTo = boost::posix_time::from_iso_string(parameters[5]);

                  auto allData = m_dataProvider->getAcquisitionRequester()->getHugeVectorKeywordDataByHour(keywordId,
                                                                                                           timeFrom,
                                                                                                           timeTo);

                  boost::shared_ptr<CStringContainer> result = boost::make_shared<CStringContainer>(allData);
                  return result;
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve parameters in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in reading device data by day");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordDataByDay(const std::vector<std::string>& parameters,
                                                                                                       const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 2)
               {
                  //get device id from URL
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 4)
                     timeFrom = boost::posix_time::from_iso_string(parameters[4]);

                  if (parameters.size() > 5)
                     timeTo = boost::posix_time::from_iso_string(parameters[5]);

                  auto allData = m_dataProvider->getAcquisitionRequester()->getHugeVectorKeywordDataByDay(keywordId,
                                                                                                          timeFrom,
                                                                                                          timeTo);

                  boost::shared_ptr<CStringContainer> result = boost::make_shared<CStringContainer>(allData);
                  return result;
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve parameters in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in reading device data by day");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordDataByMonth(const std::vector<std::string>& parameters,
                                                                                                         const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 2)
               {
                  //get device id from URL
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 4)
                     timeFrom = boost::posix_time::from_iso_string(parameters[4]);

                  if (parameters.size() > 5)
                     timeTo = boost::posix_time::from_iso_string(parameters[5]);

                  auto allData = m_dataProvider->getAcquisitionRequester()->getHugeVectorKeywordDataByMonth(keywordId,
                                                                                                            timeFrom,
                                                                                                            timeTo);
                  boost::shared_ptr<CStringContainer> result = boost::make_shared<CStringContainer>(allData);
                  return result;
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve parameters in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in reading device data by month");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordDataByYear(const std::vector<std::string>& parameters,
                                                                                                        const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 2)
               {
                  //get device id from URL
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 4)
                     timeFrom = boost::posix_time::from_iso_string(parameters[4]);

                  if (parameters.size() > 5)
                     timeTo = boost::posix_time::from_iso_string(parameters[5]);

                  auto allData = m_dataProvider->getAcquisitionRequester()->getHugeVectorKeywordDataByYear(keywordId,
                                                                                                           timeFrom,
                                                                                                           timeTo);
                  boost::shared_ptr<CStringContainer> result = boost::make_shared<CStringContainer>(allData);
                  return result;
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve parameters in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in reading device data by year");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
