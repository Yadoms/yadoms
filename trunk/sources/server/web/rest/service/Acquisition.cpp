#include "stdafx.h"
#include "Acquisition.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonResult.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include <shared/Log.h>

namespace web { namespace rest { namespace service {

   std::string CAcquisition::m_restKeyword= std::string("acquisition");


   CAcquisition::CAcquisition(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider)
   {

   }


   CAcquisition::~CAcquisition()
   {
   }


   void CAcquisition::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("keyword")("*")("lastdata"), CAcquisition::getKeywordLastData); //get all keyword data
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("keyword")("*"), CAcquisition::getKeywordData); //get all keyword data
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("keyword")("*")("*"), CAcquisition::getKeywordData); //get keyword data from date
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("keyword")("*")("*")("*"), CAcquisition::getKeywordData); //get keyword data between two dates
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CAcquisition::getAcquisitionById); 
   }

   const std::string & CAcquisition::getRestKeyword()
   {
      return m_restKeyword;
   }

   web::rest::json::CJson CAcquisition::getAcquisitionById(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int acqId = boost::lexical_cast<int>(parameters[1]);
            web::rest::json::CAcquisitionEntitySerializer serialiser;
            boost::shared_ptr<database::entities::CAcquisition> acq =  m_dataProvider->getAcquisitionRequester()->getAcquisitionById(acqId);
            return web::rest::json::CJsonResult::GenerateSuccess(serialiser.serialize(*acq.get()));
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive acquisitionId in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in retreiving one acquisition");
      }      
   }



   web::rest::json::CJson CAcquisition::getKeywordLastData(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>2)
         {
            int keywordId = boost::lexical_cast<int>(parameters[2]);
            web::rest::json::CAcquisitionEntitySerializer serialiser;
            boost::shared_ptr<database::entities::CAcquisition> acq =  m_dataProvider->getAcquisitionRequester()->getKeywordLastData(keywordId);
            return web::rest::json::CJsonResult::GenerateSuccess(serialiser.serialize(*acq.get()));
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive acquisitionId in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in retreiving one acquisition");
      }      
   }


   web::rest::json::CJson CAcquisition::getKeywordData(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {

         if(parameters.size()>=2)
         {
            //get device id from URL
            int keywordId = boost::lexical_cast<int>(parameters[2]);

            //try to get from and to limits.
            //as this method is common for three rest url, those variable may keep unfilled
            boost::posix_time::ptime timeFrom, timeTo;

            if(parameters.size()>3)
               timeFrom = web::rest::json::CJsonDate::fromString(parameters[3]);

            if(parameters.size()>4)
               timeTo = web::rest::json::CJsonDate::fromString(parameters[4]);


            std::vector< boost::tuple<boost::posix_time::ptime, std::string> > allData = m_dataProvider->getAcquisitionRequester()->getKeywordData(keywordId, timeFrom, timeTo);
            std::vector<shared::CDataContainer> objectList;
            std::vector< boost::tuple<boost::posix_time::ptime, std::string> >::const_iterator i;

            for(i=allData.begin(); i!=allData.end(); ++i)
            {
               shared::CDataContainer result;
               result.set("date", web::rest::json::CJsonDate::toString(i->get<0>()));
               result.set("key", i->get<1>());
               objectList.push_back(result);
            }

            web::rest::json::CJson result;
            result.setValues("data", objectList);
            return web::rest::json::CJsonResult::GenerateSuccess(result);

         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive parameters in url");
         }

   }
   catch(std::exception &ex)
   {
      return web::rest::json::CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      return web::rest::json::CJsonResult::GenerateError("unknown exception in reading device data");
   }
}


} //namespace service
} //namespace rest
} //namespace web 
