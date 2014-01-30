#include "stdafx.h"
#include "AcquisitionRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/AcquisitionEntitySerializer.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"

CAcquisitionRestService::CAcquisitionRestService(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("acquisition")
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



CJson CAcquisitionRestService::getOneAcquisition(const std::vector<std::string> & parameters)
{
   CAcquisitionEntitySerializer hes;
   boost::shared_ptr<CAcquisition> acquisitionFound =  m_dataProvider->getAcquisitionRequester()->getAcquisition(boost::lexical_cast<int>(parameters[0]));
   return hes.serialize(*acquisitionFound.get());
}

CJson CAcquisitionRestService::getAllAcquisitions(const std::vector<std::string> & parameters)
{
   CAcquisitionEntitySerializer hes;
   //TODO : voir les parametres
   std::vector< boost::shared_ptr<CAcquisition> > dvList = m_dataProvider->getAcquisitionRequester()->getAcquisitions("", "");
   return CJonCollectionSerializer<CAcquisition>::SerializeCollection(dvList, hes, getRestKeyword());
}
