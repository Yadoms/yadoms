#include "stdafx.h"
#include "ExportBackupHandler.h"
#include <utility>
#include <Poco/Zip/Compress.h>
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/tools/Filesystem.h>
#include "database/IDataBackup.h"
#include "task/ITask.h"

namespace task
{
   namespace backup
   {
      CExportBackupHandler::CExportBackupHandler(boost::shared_ptr<const IPathProvider> pathProvider,
                                                 boost::shared_ptr<database::IDataBackup> dataBackupInterface)
         : m_pathProvider(std::move(pathProvider)),
           m_dataBackupInterface(std::move(dataBackupInterface))
      {
         if (!m_dataBackupInterface)
            throw shared::exception::CInvalidParameter("dataBackupInterface");
      }

      std::string CExportBackupHandler::taskName()
      {
         return "backup";
      }

      int CExportBackupHandler::maxTries()
      {
         return 3;
      }

      bool CExportBackupHandler::checkEnoughSpace(const boost::filesystem::path& tempFolder)
      {
         uintmax_t neededSpace = 0;

         YADOMS_LOG(information) << "Check space needed...";

         // Database
         if (m_dataBackupInterface->backupSupported())
         {
            const auto needed = m_dataBackupInterface->backupNeededSpace();
            YADOMS_LOG(information) << "  - Database : " << needed;
            neededSpace += needed;
         }

         // Scripts
         {
            const auto needed = shared::tools::CFilesystem::directorySize(m_pathProvider->scriptsPath());
            YADOMS_LOG(information) << "  - Scripts : " << needed;
            neededSpace += needed;
         }

         // Plugins data
         {
            const auto needed = shared::tools::CFilesystem::directorySize(m_pathProvider->pluginsDataPath());
            YADOMS_LOG(information) << "  - Plugins data : " << needed;
            neededSpace += needed;
         }

         YADOMS_LOG(information) << "  Total : " << neededSpace;

         // Apply 5% margin
         return boost::filesystem::space(tempFolder).available > (neededSpace * 105 / 100);
      }

      void CExportBackupHandler::collectDataTo(const boost::filesystem::path& tempFolder,
                                               std::function<void(int)> onProgressionPercentFct)
      {
         // Backup database (1 -> 70%)
         if (m_dataBackupInterface->backupSupported())
         {
            m_dataBackupInterface->backupData(tempFolder.string(),
                                              [&](int remaining, int total, std::string i18nMessage, std::string error)
                                              {
                                                 onProgressionPercentFct(remaining * 70 / total);
                                              });
         }

         // Backup scripts (71 -> 85%)
         if (!shared::tools::CFilesystem::copyDirectoryRecursivelyTo(m_pathProvider->scriptsPath(), tempFolder / "scripts"))
            throw std::runtime_error("Fail to copy scripts to temp folder");
         onProgressionPercentFct(85);

         // Backup plugins data (86 -> 95%)
         if (!shared::tools::CFilesystem::copyDirectoryRecursivelyTo(m_pathProvider->pluginsDataPath(), tempFolder / "data"))
            throw std::runtime_error("Fail to copy plugins data to temp folder");
         onProgressionPercentFct(95);

         // Backup yadoms.ini file (96 -> 100%)
         boost::system::error_code ec;
         boost::filesystem::copy_file("yadoms.ini", tempFolder / "yadoms.ini", ec);
         onProgressionPercentFct(100);
      }
   } //namespace backup
} //namespace task
