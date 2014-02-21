#include "stdafx.h"
#include "HardwareRestService.h"
#include <shared/exceptions/NotImplemented.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "shared/Log.h"
#include "json/JsonResult.h"

std::string CHardwareRestService::m_restKeyword= std::string("hardware");

CHardwareRestService::CHardwareRestService(boost::shared_ptr<server::database::IDataProvider> dataProvider)
   :m_dataProvider(dataProvider)
{
}


CHardwareRestService::~CHardwareRestService()
{
}

const std::string & CHardwareRestService::getRestKeyword()
{
   return m_restKeyword;
}

void CHardwareRestService::configureDispatcher(CRestDispatcher & dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CHardwareRestService::getAllHardwares);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CHardwareRestService::getOneHardware);

   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CHardwareRestService::createHardware, CHardwareRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CHardwareRestService::updateHardware, CHardwareRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CHardwareRestService::deleteAllHardwares, CHardwareRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CHardwareRestService::deleteHardware, CHardwareRestService::transactionalMethod);
}


CJson CHardwareRestService::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const CJson & requestContent)
{
   boost::shared_ptr<server::database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
   CJson result;
   try
   {
      if(pTransactionalEngine)
         pTransactionalEngine->transactionBegin();
      result = realMethod(parameters, requestContent);
   }
   catch(std::exception &ex)
   {
      result = CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      result = CJsonResult::GenerateError("unknown exception hardware rest method");
   }

   if(pTransactionalEngine)
   {
      if(CJsonResult::isSuccess(result))
         pTransactionalEngine->transactionCommit();
      else
         pTransactionalEngine->transactionRollback();
   }
   return result;
}


 CJson CHardwareRestService::getOneHardware(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];

   CHardwareEntitySerializer hes;
   boost::shared_ptr<server::database::entities::CHardware> hardwareFound =  m_dataProvider->getHardwareRequester()->getHardware(boost::lexical_cast<int>(objectId));
   return CJsonResult::GenerateSuccess(hes.serialize(*hardwareFound.get()));
}

CJson CHardwareRestService::getAllHardwares(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CHardwareEntitySerializer hes;
   std::vector< boost::shared_ptr<server::database::entities::CHardware> > hwList = m_dataProvider->getHardwareRequester()->getHardwares();
   return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<server::database::entities::CHardware>::SerializeCollection(hwList, hes, getRestKeyword()));
}

CJson CHardwareRestService::createHardware(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   return CJsonResult::GenerateError("Not yet implemented");
}

CJson CHardwareRestService::updateHardware(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   return CJsonResult::GenerateError("Not yet implemented");
}


CJson CHardwareRestService::deleteHardware(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   return CJsonResult::GenerateError("Not yet implemented");
}

CJson CHardwareRestService::deleteAllHardwares(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   return CJsonResult::GenerateError("Not yet implemented");
}
