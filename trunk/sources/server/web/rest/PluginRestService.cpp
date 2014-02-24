#include "stdafx.h"
#include "PluginRestService.h"
#include <shared/exception/NotImplemented.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "shared/Log.h"
#include "json/JsonResult.h"

CPluginRestService::CPluginRestService(boost::shared_ptr<database::IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("plugin")
{
}


CPluginRestService::~CPluginRestService()
{
}

const std::string & CPluginRestService::getRestKeyword()
{
   return m_restKeyword;
}

void CPluginRestService::configureDispatcher(CRestDispatcher & dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CPluginRestService::getAllPlugins);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CPluginRestService::getOnePlugin);

   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPluginRestService::createPlugin, CPluginRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPluginRestService::updatePlugin, CPluginRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPluginRestService::deleteAllPlugins, CPluginRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPluginRestService::deletePlugin, CPluginRestService::transactionalMethod);
}


CJson CPluginRestService::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const CJson & requestContent)
{
   boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
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
      result = CJsonResult::GenerateError("unknown exception plugin rest method");
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


 CJson CPluginRestService::getOnePlugin(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];

   CPluginEntitySerializer hes;
   boost::shared_ptr<database::entities::CPlugin> pluginFound =  m_dataProvider->getPluginRequester()->getPlugin(boost::lexical_cast<int>(objectId));
   return CJsonResult::GenerateSuccess(hes.serialize(*pluginFound.get()));
}

CJson CPluginRestService::getAllPlugins(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CPluginEntitySerializer hes;
   std::vector< boost::shared_ptr<database::entities::CPlugin> > hwList = m_dataProvider->getPluginRequester()->getPlugins();
   return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<database::entities::CPlugin>::SerializeCollection(hwList, hes, getRestKeyword()));
}

CJson CPluginRestService::createPlugin(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   return CJsonResult::GenerateError("Not yet implemented");
}

CJson CPluginRestService::updatePlugin(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   return CJsonResult::GenerateError("Not yet implemented");
}


CJson CPluginRestService::deletePlugin(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   return CJsonResult::GenerateError("Not yet implemented");
}

CJson CPluginRestService::deleteAllPlugins(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   return CJsonResult::GenerateError("Not yet implemented");
}
