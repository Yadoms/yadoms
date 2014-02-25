#include "stdafx.h"
#include "AcquisitionRestService.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/json/JsonResult.h"
#include "web/rest/RestDispatcherHelpers.hpp"

namespace web { namespace rest { namespace service {

   std::string CAcquisitionRestService::m_restKeyword= std::string("acquisition");


   CAcquisitionRestService::CAcquisitionRestService(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider)
   {
   }


   CAcquisitionRestService::~CAcquisitionRestService()
   {
   }

   const std::string & CAcquisitionRestService::getRestKeyword()
   {
      return m_restKeyword;
   }


   void CAcquisitionRestService::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CAcquisitionRestService::getAllAcquisitions);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CAcquisitionRestService::getOneAcquisition);
   }



   web::rest::json::CJson CAcquisitionRestService::getOneAcquisition(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      web::rest::json::CAcquisitionEntitySerializer hes;
      boost::shared_ptr<database::entities::CAcquisition> acquisitionFound =  m_dataProvider->getAcquisitionRequester()->getAcquisition(boost::lexical_cast<int>(parameters[0]));
      return web::rest::json::CJsonResult::GenerateSuccess(hes.serialize(*acquisitionFound.get()));
   }

   web::rest::json::CJson CAcquisitionRestService::getAllAcquisitions(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      web::rest::json::CAcquisitionEntitySerializer hes;
      //TODO : voir les parametres
      std::vector< boost::shared_ptr<database::entities::CAcquisition> > dvList = m_dataProvider->getAcquisitionRequester()->getAcquisitions("", "");
      return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CAcquisition>::SerializeCollection(dvList, hes, getRestKeyword()));
   }



} //namespace service
} //namespace rest
} //namespace web 
