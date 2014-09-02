#include "stdafx.h"
#include "Acquisition.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/Result.h"
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

   shared::CDataContainer CAcquisition::getAcquisitionById(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int acqId = boost::lexical_cast<int>(parameters[1]);
            boost::shared_ptr<database::entities::CAcquisition> acq =  m_dataProvider->getAcquisitionRequester()->getAcquisitionById(acqId);
            return web::rest::CResult::GenerateSuccess(acq);
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive acquisitionId in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving one acquisition");
      }      
   }



   shared::CDataContainer CAcquisition::getKeywordLastData(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>2)
         {
            int keywordId = boost::lexical_cast<int>(parameters[2]);
            boost::shared_ptr<database::entities::CAcquisition> acq =  m_dataProvider->getAcquisitionRequester()->getKeywordLastData(keywordId);
            return web::rest::CResult::GenerateSuccess(acq);
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive acquisitionId in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving one acquisition");
      }      
   }


   shared::CDataContainer CAcquisition::getKeywordData(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
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
               timeFrom = boost::posix_time::from_iso_string(parameters[3]);

            if(parameters.size()>4)
               timeTo = boost::posix_time::from_iso_string(parameters[4]);

            //using the raw data format, to optimize treatment
            //output is "[[dateiso,data],[dateiso,data],....]"
            std::string raw = m_dataProvider->getAcquisitionRequester()->getKeywordRawData(keywordId, timeFrom, timeTo);
            return web::rest::CResult::GenerateSuccess(raw);
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive parameters in url");
         }

   }
   catch(std::exception &ex)
   {
      return web::rest::CResult::GenerateError(ex);
   }
   catch(...)
   {
      return web::rest::CResult::GenerateError("unknown exception in reading device data");
   }
}


} //namespace service
} //namespace rest
} //namespace web 
