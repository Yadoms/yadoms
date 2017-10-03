#include "stdafx.h"
#include "Acquisition.h"
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>

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
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*"), CAcquisition::getHighchartKeywordData); //get all keyword data (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("*"), CAcquisition::getHighchartKeywordData); //get keyword data from date (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("*")("*"), CAcquisition::getHighchartKeywordData); //get keyword data between two dates (fast queries, optimized for highcharts js)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour"), CAcquisition::getKeywordDataByHour); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour")("*"), CAcquisition::getKeywordDataByHour); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour")("*")("*"), CAcquisition::getKeywordDataByHour); //get keyword data between two dates
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("hour"), CAcquisition::getHighchartKeywordDataByHour); //get all keyword data (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("hour")("*"), CAcquisition::getHighchartKeywordDataByHour); //get keyword data from date (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("hour")("*")("*"), CAcquisition::getHighchartKeywordDataByHour); //get keyword data between two dates (fast queries, optimized for highcharts js)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day"), CAcquisition::getKeywordDataByDay); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day")("*"), CAcquisition::getKeywordDataByDay); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day")("*")("*"), CAcquisition::getKeywordDataByDay); //get keyword data between two dates
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("day"), CAcquisition::getHighchartKeywordDataByDay); //get all keyword data (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("day")("*"), CAcquisition::getHighchartKeywordDataByDay); //get keyword data from date (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("day")("*")("*"), CAcquisition::getHighchartKeywordDataByDay); //get keyword data between two dates (fast queries, optimized for highcharts js)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("month"), CAcquisition::getKeywordDataByMonth); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("month")("*"), CAcquisition::getKeywordDataByMonth); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("month")("*")("*"), CAcquisition::getKeywordDataByMonth); //get keyword data between two dates
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("month"), CAcquisition::getHighchartKeywordDataByMonth); //get all keyword data (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("month")("*"), CAcquisition::getHighchartKeywordDataByMonth); //get keyword data from date (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("month")("*")("*"), CAcquisition::getHighchartKeywordDataByMonth); //get keyword data between two dates (fast queries, optimized for highcharts js)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("year"), CAcquisition::getKeywordDataByYear); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("year")("*"), CAcquisition::getKeywordDataByYear); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("year")("*")("*"), CAcquisition::getKeywordDataByYear); //get keyword data between two dates
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("year"), CAcquisition::getHighchartKeywordDataByYear); //get all keyword data (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("year")("*"), CAcquisition::getHighchartKeywordDataByYear); //get keyword data from date (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("year")("*")("*"), CAcquisition::getHighchartKeywordDataByYear); //get keyword data between two dates (fast queries, optimized for highcharts js)
         }

         const std::string& CAcquisition::getRestKeyword()
         {
            return m_restKeyword;
         }

         shared::CDataContainer CAcquisition::getKeywordLastData(const std::vector<std::string>& parameters,
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

         shared::CDataContainer CAcquisition::getKeywordListLastData(const std::vector<std::string>& parameters,
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
                              shared::CDataContainer emptyResult;
                              emptyResult.set("keywordId", *i);
                              result.set(boost::lexical_cast<std::string>(*i),
                                         emptyResult);
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

         shared::CDataContainer CAcquisition::getKeywordData(const std::vector<std::string>& parameters,
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

         shared::CDataContainer CAcquisition::getKeywordDataByHour(const std::vector<std::string>& parameters,
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

                  auto allData = m_dataProvider->getAcquisitionRequester()->getKeywordDataByHour(keywordId,
                                                                                                 timeFrom,
                                                                                                 timeTo);
                  shared::CDataContainer result;
                  result.set("data", allData);
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
               return CResult::GenerateError("unknown exception in reading device data by day");
            }
         }


         shared::CDataContainer CAcquisition::getKeywordDataByDay(const std::vector<std::string>& parameters,
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

                  auto allData = m_dataProvider->getAcquisitionRequester()->getKeywordDataByDay(keywordId,
                                                                                                timeFrom,
                                                                                                timeTo);
                  shared::CDataContainer result;
                  result.set("data", allData);
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
               return CResult::GenerateError("unknown exception in reading device data by day");
            }
         }


         shared::CDataContainer CAcquisition::getKeywordDataByMonth(const std::vector<std::string>& parameters,
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

                  auto allData = m_dataProvider->getAcquisitionRequester()->getKeywordDataByMonth(keywordId,
                                                                                                  timeFrom,
                                                                                                  timeTo);
                  shared::CDataContainer result;
                  result.set("data", allData);
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
               return CResult::GenerateError("unknown exception in reading device data by month");
            }
         }


         shared::CDataContainer CAcquisition::getKeywordDataByYear(const std::vector<std::string>& parameters,
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

                  auto allData = m_dataProvider->getAcquisitionRequester()->getKeywordDataByYear(keywordId,
                                                                                                 timeFrom,
                                                                                                 timeTo);
                  shared::CDataContainer result;
                  result.set("data", allData);
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
               return CResult::GenerateError("unknown exception in reading device data by year");
            }
         }

         shared::CDataContainer CAcquisition::getHighchartKeywordData(const std::vector<std::string>& parameters,
                                                                      const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 3)
               {
                  //get device id from URL
                  auto keywordId = boost::lexical_cast<int>(parameters[3]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 4)
                     timeFrom = boost::posix_time::from_iso_string(parameters[4]);

                  if (parameters.size() > 5)
                     timeTo = boost::posix_time::from_iso_string(parameters[5]);

                  //using the raw data format, to optimize treatment
                  //output is "[[dateiso,data],[dateiso,data],....]"
                  YADOMS_LOG(debug) << "Reading highchart data...";
                  auto raw = m_dataProvider->getAcquisitionRequester()->getKeywordHighchartData(keywordId,
                                                                                                timeFrom,
                                                                                                timeTo);
                  YADOMS_LOG(debug) << "Reading highchart data... OK";
                  return CResult::GenerateSuccess(raw);
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

         shared::CDataContainer CAcquisition::getHighchartKeywordDataByHour(const std::vector<std::string>& parameters,
                                                                            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 3)
               {
                  //get device id from URL
                  auto keywordId = boost::lexical_cast<int>(parameters[3]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 5)
                     timeFrom = boost::posix_time::from_iso_string(parameters[5]);

                  if (parameters.size() > 6)
                     timeTo = boost::posix_time::from_iso_string(parameters[6]);

                  //using the raw data format, to optimize treatment
                  //output is "[[dateiso,data],[dateiso,data],....]"
                  YADOMS_LOG(debug) << "Reading highchart data...";
                  auto raw = m_dataProvider->getAcquisitionRequester()->getKeywordHighchartData(keywordId,
                                                                                                timeFrom,
                                                                                                timeTo);
                  YADOMS_LOG(debug) << "Reading highchart data... OK";
                  return CResult::GenerateSuccess(raw);
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


         shared::CDataContainer CAcquisition::getHighchartKeywordDataByDay(const std::vector<std::string>& parameters,
                                                                           const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 3)
               {
                  //get device id from URL
                  auto keywordId = boost::lexical_cast<int>(parameters[3]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 5)
                     timeFrom = boost::posix_time::from_iso_string(parameters[5]);

                  if (parameters.size() > 6)
                     timeTo = boost::posix_time::from_iso_string(parameters[6]);

                  //using the raw data format, to optimize treatment
                  //output is "[[dateiso,data],[dateiso,data],....]"
                  YADOMS_LOG(debug) << "Reading highchart data...";
                  auto raw = m_dataProvider->getAcquisitionRequester()->getKeywordHighchartData(keywordId,
                                                                                                timeFrom,
                                                                                                timeTo);
                  YADOMS_LOG(debug) << "Reading highchart data... OK";
                  return CResult::GenerateSuccess(raw);
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


         shared::CDataContainer CAcquisition::getHighchartKeywordDataByMonth(const std::vector<std::string>& parameters,
                                                                             const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 3)
               {
                  //get device id from URL
                  auto keywordId = boost::lexical_cast<int>(parameters[3]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 5)
                     timeFrom = boost::posix_time::from_iso_string(parameters[5]);

                  if (parameters.size() > 6)
                     timeTo = boost::posix_time::from_iso_string(parameters[6]);

                  //using the raw data format, to optimize treatment
                  //output is "[[dateiso,data],[dateiso,data],....]"
                  YADOMS_LOG(debug) << "Reading highchart data...";
                  auto raw = m_dataProvider->getAcquisitionRequester()->getKeywordHighchartDataByMonth(keywordId,
                                                                                                       timeFrom,
                                                                                                       timeTo);
                  YADOMS_LOG(debug) << "Reading highchart data... OK";
                  return CResult::GenerateSuccess(raw);
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

         shared::CDataContainer CAcquisition::getHighchartKeywordDataByYear(const std::vector<std::string>& parameters,
                                                                            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 3)
               {
                  //get device id from URL
                  auto keywordId = boost::lexical_cast<int>(parameters[3]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 5)
                     timeFrom = boost::posix_time::from_iso_string(parameters[5]);

                  if (parameters.size() > 6)
                     timeTo = boost::posix_time::from_iso_string(parameters[6]);

                  //using the raw data format, to optimize treatment
                  //output is "[[dateiso,data],[dateiso,data],....]"
                  YADOMS_LOG(debug) << "Reading highchart data...";
                  auto raw = m_dataProvider->getAcquisitionRequester()->getKeywordHighchartDataByYear(keywordId,
                                                                                                      timeFrom,
                                                                                                      timeTo);
                  YADOMS_LOG(debug) << "Reading highchart data... OK";
                  return CResult::GenerateSuccess(raw);
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
      } //namespace service
   } //namespace rest
} //namespace web 


