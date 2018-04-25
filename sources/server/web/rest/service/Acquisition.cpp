#include "stdafx.h"
#include "Acquisition.h"
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "web/rest/StringContainer.h"
#include <Poco/Stopwatch.h>
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

         CAcquisition::~CAcquisition()
         {
         }

         void CAcquisition::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("keyword")("lastdata"), CAcquisition::getKeywordListLastData); //get the last data of a list of keywords
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("lastdata"), CAcquisition::getKeywordLastData); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*"), CAcquisition::getKeywordData); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("*"), CAcquisition::getKeywordData); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("*")("*"), CAcquisition::getKeywordData); //get keyword data between two dates

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour"), CAcquisition::getKeywordDataByHour); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour")("*"), CAcquisition::getKeywordDataByHour); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour")("*")("*"), CAcquisition::getKeywordDataByHour); //get keyword data between two dates

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day"), CAcquisition::getKeywordDataByDay); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day")("*"), CAcquisition::getKeywordDataByDay); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day")("*")("*"), CAcquisition::getKeywordDataByDay); //get keyword data between two dates

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("month"), CAcquisition::getKeywordDataByMonth); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("month")("*"), CAcquisition::getKeywordDataByMonth); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("month")("*")("*"), CAcquisition::getKeywordDataByMonth); //get keyword data between two dates

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("year"), CAcquisition::getKeywordDataByYear); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("year")("*"), CAcquisition::getKeywordDataByYear); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("year")("*")("*"), CAcquisition::getKeywordDataByYear); //get keyword data between two dates
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
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);
                  auto acq = m_dataProvider->getKeywordRequester()->getKeywordLastAcquisition(keywordId);
                  return CResult::GenerateSuccess(acq);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive acquisitionId in url");
            }
            catch (shared::exception::CEmptyResult& /*noData*/)
            {
               try {
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);
                  auto keyword = m_dataProvider->getKeywordRequester()->getKeyword(keywordId);

                  if (keyword)
                  {
                     shared::CDataContainer emptyResult;
                     emptyResult.set("keywordId", keywordId);
                     return CResult::GenerateSuccess(emptyResult);
                  }
               }
               catch (shared::exception::CEmptyResult& ex)
               {
                  return CResult::GenerateError(ex);
               }
               //if no data just return success
               return CResult::GenerateSuccess();
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving one acquisition");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordListLastData(const std::vector<std::string>& parameters,
                                                                                                          const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  shared::CDataContainer content(requestContent);
                  if (content.containsChild("keywords"))
                  {
                     auto list = content.get<std::vector<int> >("keywords");

                     // erase all duplicates, if any
                     sort(list.begin(), list.end());
                     list.erase(unique(list.begin(), list.end()), list.end());

                     shared::CDataContainer result;
                     for (auto i = list.begin(); i != list.end(); ++i)
                     {
                        try
                        {
                           auto lastData = m_dataProvider->getKeywordRequester()->getKeywordLastAcquisition(*i, false);
                           if (lastData)
                           {
                              result.set(boost::lexical_cast<std::string>(*i),
                                         lastData);
                           }
                           else
                           {
                              try {
                                 auto keyword = m_dataProvider->getKeywordRequester()->getKeyword(*i);

                                 if (keyword)
                                 {
                                    shared::CDataContainer emptyResult;
                                    emptyResult.set("keywordId", *i);
                                    result.set(boost::lexical_cast<std::string>(*i),
                                               emptyResult);
                                 }
                              }
                              catch(std::exception& /*nokeyword*/)
                              {
                                 shared::CDataContainer noKeyword;
                                 noKeyword.set("keywordId", *i);
                                 noKeyword.set("error", "keyword id doesn't exist");
                                 result.set(boost::lexical_cast<std::string>(*i),
                                            noKeyword);
                              }
                           }
                        }
                        catch (std::exception& /*noData*/)
                        {
                           //ensure returning entity object, because we need to indicate that a keyword have no data, not others (in case of multiple keyword without data)
                           shared::CDataContainer emptyResult;
                           emptyResult.set("keywordId", *i);
                           result.set(boost::lexical_cast<std::string>(*i),
                                      emptyResult);
                        }
                     }
                     return CResult::GenerateSuccess(result);
                  }
                  return CResult::GenerateError("invalid parameter. Can not retreive keywords in request content");
               }
               return CResult::GenerateError("invalid parameter.");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving one acquisition");
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
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);

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

                  for (auto i = allData.begin(); i != allData.end(); ++i)
                  {
                     shared::CDataContainer result;
                     result.set("date", boost::posix_time::to_iso_string(i->get<0>()));
                     result.set("key", i->get<1>());
                     objectList.push_back(result);
                  }

                  shared::CDataContainer result;
                  result.set<std::vector<shared::CDataContainer> >("data", objectList);
                  return CResult::GenerateSuccess(result);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CAcquisition::getKeywordDataByHour(const std::vector<std::string>& parameters,
                                                                                                        const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 2)
               {
                  //get device id from URL
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 4)
                     timeFrom = boost::posix_time::from_iso_string(parameters[4]);

                  if (parameters.size() > 5)
                     timeTo = boost::posix_time::from_iso_string(parameters[5]);

                  Poco::Stopwatch sw;
                  sw.start();
                  auto allData = m_dataProvider->getAcquisitionRequester()->getHugeVectorKeywordDataByHour(keywordId,
                                                                                                     timeFrom,
                                                                                                     timeTo);
                  Poco::UInt64 load = sw.elapsed();

                  YADOMS_LOG(information) << "============================== " << load << " ms";

                  //shared::CDataContainer d;
                  //d.set("data", allData);
                  boost::shared_ptr<CStringContainer> result=boost::make_shared<CStringContainer>(allData);
                  //return CResult::GenerateSuccess(d);
                  return result;
               }
               return CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
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
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);

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
               return CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
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
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);

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
               return CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
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
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);

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
               return CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
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