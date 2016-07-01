#include "stdafx.h"
#include "Maintenance.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/Result.h"

namespace web { namespace rest { namespace service {

   std::string CMaintenance::m_restKeyword= std::string("maintenance");

   CMaintenance::CMaintenance(boost::shared_ptr<database::IDatabaseRequester> databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }


   CMaintenance::~CMaintenance()
   {
   }


   void CMaintenance::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("information"), CMaintenance::getDatabaseInformation);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("backup"), CMaintenance::startDatabaseBackup);
   }


   const std::string & CMaintenance::getRestKeyword()
   {
      return m_restKeyword;
   }

   shared::CDataContainer CMaintenance::getDatabaseInformation(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      try
      {
         shared::CDataContainer result = m_databaseRequester->getInformation();
         result.set("backupSupport", m_databaseRequester->backupSupported());
         return CResult::GenerateSuccess(result);
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in retreiving database information");
      }      
   }
   
   shared::CDataContainer CMaintenance::startDatabaseBackup(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      return CResult::GenerateError("Not implemented");
   }

} //namespace service
} //namespace rest
} //namespace web 
