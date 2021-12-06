#include <boost/date_time/c_local_time_adjustor.hpp>

#include "stdafx.h"
#include "Maintenance.h"
#include "RestEndPoint.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/NoContentAnswer.h"
#include "web/rest/SuccessAnswer.h"


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

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "maintenance/backups", getBackupsV2));
            //TODO

            
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("backup"), CMaintenance::startBackup)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup")("*"), CMaintenance::deleteBackup)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("restore")("*"), CMaintenance::restoreBackup)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup"), CMaintenance::deleteAllBackups)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("uploadBackup"), CMaintenance::uploadBackup)
            //
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("packlogs"), CMaintenance::startPackLogs)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("logs"), CMaintenance::getLogs)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("logs"), CMaintenance::deleteAllLogs)
            //
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("startExportData")("*"), CMaintenance::startExportData)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("exportData"), CMaintenance::getExportData)

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CMaintenance::getBackupsV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               if (!m_databaseRequester->backupSupported())
                  return boost::make_shared<CNoContentAnswer>();

               const auto backup = m_pathProvider->backupPath();

               if (backup.empty() || !exists(backup) || !is_directory(backup))
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer result;
               result.createArray("backups");
               // Check all subdirectories in plugin path
               for (boost::filesystem::directory_iterator i(backup); i != boost::filesystem::directory_iterator(); ++i)
               {
                  if (!is_regular_file(i->path()))
                     continue;

                  if (!boost::starts_with(i->path().filename().string(), "backup_"))
                     continue;

                  auto fileSize = file_size(i->path());

                  const auto lastWriteTimeT = last_write_time(i->path());
                  const auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(
                     boost::posix_time::from_time_t(lastWriteTimeT));

                  shared::CDataContainer file;
                  file.set("size", fileSize);
                  file.set("modificationDate", lastWriteTimePosix);
                  file.set("path", i->path().string());
                  file.set("url", i->path().filename().string());
                  file.set("inprogress", boost::iends_with(i->path().filename().string(), ".inprogress"));
                  result.appendArray("backups", file);
               }

               return boost::make_shared<CSuccessAnswer>(result);
            }
            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getKeywords request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            } catch (const std::exception& exception
            )
            {
               YADOMS_LOG(error) << "Error processing getKeywords request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get keywords");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
