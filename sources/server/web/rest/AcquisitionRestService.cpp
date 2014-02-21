#include "stdafx.h"
#include "AcquisitionRestService.h"
#include <shared/exceptions/NotImplemented.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "json/JsonResult.h"
#include "RestDispatcherHelpers.hpp"

std::string CAcquisitionRestService::m_restKeyword= std::string("acquisition");


CAcquisitionRestService::CAcquisitionRestService(boost::shared_ptr<server::database::IDataProvider> dataProvider)
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



CJson CAcquisitionRestService::getOneAcquisition(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CAcquisitionEntitySerializer hes;
   boost::shared_ptr<server::database::entities::CAcquisition> acquisitionFound =  m_dataProvider->getAcquisitionRequester()->getAcquisition(boost::lexical_cast<int>(parameters[0]));
   return CJsonResult::GenerateSuccess(hes.serialize(*acquisitionFound.get()));
}

CJson CAcquisitionRestService::getAllAcquisitions(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CAcquisitionEntitySerializer hes;
   //TODO : voir les parametres
   std::vector< boost::shared_ptr<server::database::entities::CAcquisition> > dvList = m_dataProvider->getAcquisitionRequester()->getAcquisitions("", "");
   return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<server::database::entities::CAcquisition>::SerializeCollection(dvList, hes, getRestKeyword()));
}
