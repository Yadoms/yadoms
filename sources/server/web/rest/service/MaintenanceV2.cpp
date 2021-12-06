#include "stdafx.h"
#include "Maintenance.h"
#include "RestEndPoint.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CMaintenance::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            
            //TODO

            
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("backup"), CMaintenance::getBackups)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("backup"), CMaintenance::startBackup)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup")("*"), CMaintenance::deleteBackup)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("restore")("*"), CMaintenance::restoreBackup)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup"), CMaintenance::deleteAllBackups)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("uploadBackup"), CMaintenance::uploadBackup)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("packlogs"), CMaintenance::startPackLogs)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("logs"), CMaintenance::getLogs)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("logs"), CMaintenance::deleteAllLogs)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("startExportData")("*"), CMaintenance::startExportData)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("exportData"), CMaintenance::getExportData)

            return m_endPoints;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
