#include "stdafx.h"
#include "Acquisition.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include <shared/Log.h>

namespace web {
   namespace rest {
      namespace service {

         std::string CAcquisition::m_restKeyword = std::string("acquisition");


         CAcquisition::CAcquisition(boost::shared_ptr<database::IDataProvider> dataProvider)
            :m_dataProvider(dataProvider)
         {

         }


         CAcquisition::~CAcquisition()
         {
         }


         void CAcquisition::configureDispatcher(CRestDispatcher & dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("lastdata"), CAcquisition::getKeywordLastData); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*"), CAcquisition::getKeywordData); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("*"), CAcquisition::getKeywordData); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("*")("*"), CAcquisition::getKeywordData); //get keyword data between two dates
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*"), CAcquisition::getHighchartKeywordData); //get all keyword data (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("*"), CAcquisition::getHighchartKeywordData); //get keyword data from date (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("*")("*"), CAcquisition::getHighchartKeywordData); //get keyword data between two dates (fast queries, optimized for highcharts js)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day"), CAcquisition::getKeywordDataByDay); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day")("*"), CAcquisition::getKeywordDataByDay); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("day")("*")("*"), CAcquisition::getKeywordDataByDay); //get keyword data between two dates
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("day"), CAcquisition::getHighchartKeywordDataByDay); //get all keyword data (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("day")("*"), CAcquisition::getHighchartKeywordDataByDay); //get keyword data from date (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("day")("*")("*"), CAcquisition::getHighchartKeywordDataByDay); //get keyword data between two dates (fast queries, optimized for highcharts js)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour"), CAcquisition::getKeywordDataByHour); //get all keyword data
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour")("*"), CAcquisition::getKeywordDataByHour); //get keyword data from date
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*")("hour")("*")("*"), CAcquisition::getKeywordDataByHour); //get keyword data between two dates
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("hour"), CAcquisition::getHighchartKeywordDataByHour); //get all keyword data (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("hour")("*"), CAcquisition::getHighchartKeywordDataByHour); //get keyword data from date (fast queries, optimized for highcharts js)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("highcharts")("keyword")("*")("hour")("*")("*"), CAcquisition::getHighchartKeywordDataByHour); //get keyword data between two dates (fast queries, optimized for highcharts js)
         }

         const std::string & CAcquisition::getRestKeyword()
         {
            return m_restKeyword;
         }

         shared::CDataContainer CAcquisition::getKeywordLastData(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
         {
            try
            {
               if (parameters.size() > 2)
               {
                  int keywordId = boost::lexical_cast<int>(parameters[2]);
                  boost::shared_ptr<database::entities::CAcquisition> acq = m_dataProvider->getAcquisitionRequester()->getKeywordLastData(keywordId);
                  return web::rest::CResult::GenerateSuccess(acq);
               }
               else
               {
                  return web::rest::CResult::GenerateError("invalid parameter. Can not retreive acquisitionId in url");
               }
            }
            catch (std::exception &ex)
            {
               return web::rest::CResult::GenerateError(ex);
            }
            catch (...)
            {
               return web::rest::CResult::GenerateError("unknown exception in retreiving one acquisition");
            }
         }


         shared::CDataContainer CAcquisition::getKeywordData(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
         {
            try
            {

               if (parameters.size() >= 2)
               {
                  //get device id from URL
                  int keywordId = boost::lexical_cast<int>(parameters[2]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 3)
                     timeFrom = boost::posix_time::from_iso_string(parameters[3]);

                  if (parameters.size() > 4)
                     timeTo = boost::posix_time::from_iso_string(parameters[4]);

                  std::vector< boost::tuple<boost::posix_time::ptime, std::string> > allData = m_dataProvider->getAcquisitionRequester()->getKeywordData(keywordId, timeFrom, timeTo);
                  std::vector<shared::CDataContainer> objectList;
                  std::vector< boost::tuple<boost::posix_time::ptime, std::string> >::const_iterator i;

                  for (i = allData.begin(); i != allData.end(); ++i)
                  {
                     shared::CDataContainer result;
                     result.set("date", boost::posix_time::to_iso_string(i->get<0>()));
                     result.set("key", i->get<1>());
                     objectList.push_back(result);
                  }

                  shared::CDataContainer result;
                  result.set< std::vector<shared::CDataContainer> >("data", objectList);
                  return web::rest::CResult::GenerateSuccess(result);
               }
               else
               {
                  return web::rest::CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
               }

            }
            catch (std::exception &ex)
            {
               return web::rest::CResult::GenerateError(ex);
            }
            catch (...)
            {
               return web::rest::CResult::GenerateError("unknown exception in reading device data");
            }
         }


         shared::CDataContainer CAcquisition::getKeywordDataByDay(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
         {
            try
            {

               if (parameters.size() >= 2)
               {
                  //get device id from URL
                  int keywordId = boost::lexical_cast<int>(parameters[2]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 4)
                     timeFrom = boost::posix_time::from_iso_string(parameters[4]);

                  if (parameters.size() > 5)
                     timeTo = boost::posix_time::from_iso_string(parameters[5]);

                  std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > allData = m_dataProvider->getAcquisitionRequester()->getKeywordDataByDay(keywordId, timeFrom, timeTo);
                  shared::CDataContainer result;
                  result.set("data", allData);
                  return web::rest::CResult::GenerateSuccess(result);
               }
               else
               {
                  return web::rest::CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
               }

            }
            catch (std::exception &ex)
            {
               return web::rest::CResult::GenerateError(ex);
            }
            catch (...)
            {
               return web::rest::CResult::GenerateError("unknown exception in reading device data by day");
            }
         }

         shared::CDataContainer CAcquisition::getKeywordDataByHour(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
         {
            try
            {

               if (parameters.size() >= 2)
               {
                  //get device id from URL
                  int keywordId = boost::lexical_cast<int>(parameters[2]);

                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if (parameters.size() > 4)
                     timeFrom = boost::posix_time::from_iso_string(parameters[4]);

                  if (parameters.size() > 5)
                     timeTo = boost::posix_time::from_iso_string(parameters[5]);

                  std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary> > allData = m_dataProvider->getAcquisitionRequester()->getKeywordDataByHour(keywordId, timeFrom, timeTo);
                  shared::CDataContainer result;
                  result.set("data", allData);
                  return web::rest::CResult::GenerateSuccess(result);
               }
               else
               {
                  return web::rest::CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
               }

            }
            catch (std::exception &ex)
            {
               return web::rest::CResult::GenerateError(ex);
            }
            catch (...)
            {
               return web::rest::CResult::GenerateError("unknown exception in reading device data by day");
            }
         }


         shared::CDataContainer CAcquisition::getHighchartKeywordData(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
         {
            try
            {

               if (parameters.size() >= 3)
               {
                  //get device id from URL
                  int keywordId = boost::lexical_cast<int>(parameters[3]);

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
                  std::string raw = m_dataProvider->getAcquisitionRequester()->getKeywordHighchartData(keywordId, timeFrom, timeTo);
                  YADOMS_LOG(debug) << "Reading highchart data... OK";
                  return web::rest::CResult::GenerateSuccess(raw);
               }
               else
               {
                  return web::rest::CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
               }

            }
            catch (std::exception &ex)
            {
               return web::rest::CResult::GenerateError(ex);
            }
            catch (...)
            {
               return web::rest::CResult::GenerateError("unknown exception in reading device data");
            }
         }

         shared::CDataContainer CAcquisition::getHighchartKeywordDataByDay(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
         {
            try
            {

               if (parameters.size() >= 3)
               {
                  //get device id from URL
                  int keywordId = boost::lexical_cast<int>(parameters[3]);

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
                  std::string raw = m_dataProvider->getAcquisitionRequester()->getKeywordHighchartData(keywordId, timeFrom, timeTo);
                  YADOMS_LOG(debug) << "Reading highchart data... OK";
                  return web::rest::CResult::GenerateSuccess(raw);
               }
               else
               {
                  return web::rest::CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
               }

            }
            catch (std::exception &ex)
            {
               return web::rest::CResult::GenerateError(ex);
            }
            catch (...)
            {
               return web::rest::CResult::GenerateError("unknown exception in reading device data");
            }
         }

         shared::CDataContainer CAcquisition::getHighchartKeywordDataByHour(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
         {
            try
            {

               if (parameters.size() >= 3)
               {
                  //get device id from URL
                  int keywordId = boost::lexical_cast<int>(parameters[3]);

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
                  std::string raw = m_dataProvider->getAcquisitionRequester()->getKeywordHighchartData(keywordId, timeFrom, timeTo);
                  YADOMS_LOG(debug) << "Reading highchart data... OK";
                  return web::rest::CResult::GenerateSuccess(raw);
               }
               else
               {
                  return web::rest::CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
               }

            }
            catch (std::exception &ex)
            {
               return web::rest::CResult::GenerateError(ex);
            }
            catch (...)
            {
               return web::rest::CResult::GenerateError("unknown exception in reading device data");
            }
         }


      } //namespace service
   } //namespace rest
} //namespace web 
