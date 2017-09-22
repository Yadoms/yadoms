#include "stdafx.h"
#include <shared/exception/InvalidParameter.hpp>
#include "database/IDataBackup.h"
#include "task/ITask.h"
#include "Backup.h"
#include "tools/FileSystem.h"
#include <Poco/Zip/Compress.h>
#include <shared/currentTime/Provider.h>

namespace task
{
   namespace backup
   {
      std::string CBackup::m_taskName = "backup";

      CBackup::CBackup(const IPathProvider& pathProvider, boost::shared_ptr<database::IDataBackup> dataBackupInterface)
         : m_pathProvider(pathProvider), m_dataBackupInterface(dataBackupInterface)
      {
         if (!m_dataBackupInterface)
            throw shared::exception::CInvalidParameter("dataBackupInterface");
      }

      CBackup::~CBackup()
      {
      }

      const std::string& CBackup::getName()
      {
         return m_taskName;
      }

      void CBackup::OnProgressionUpdatedInternal(int remaining, int total, float currentPart, float totalPart, const std::string& message)
      {
         float progression = currentPart + (total != 0 ? ((float)(total - remaining) * (float)totalPart / (float)total) : 0);

         if (m_reportRealProgress)
            m_reportRealProgress(true, progression, message, std::string(), shared::CDataContainer::EmptyContainer);
      }


      void CBackup::doWork(TaskProgressFunc pFunctor)
      {
         m_reportRealProgress = pFunctor;

         //create "backup temp" folder
         auto backupTempFolder = boost::filesystem::temp_directory_path() / "yadomsBackup";

         //if folder exist, cleanup, else create if
         if (boost::filesystem::exists(backupTempFolder))
         {
            boost::filesystem::remove_all(backupTempFolder);
            boost::filesystem::remove(backupTempFolder);
         }

         if (!boost::filesystem::create_directory(backupTempFolder))
         {
            OnProgressionUpdatedInternal(0, 100, 80.0f, 85.0f, "fail to create temp backup folder");
         }

         //backup database (0 -> 80%)
         m_dataBackupInterface->backupData(backupTempFolder.string(), [&](int remaining, int total, std::string i18nMessage, std::string error)
         {
            OnProgressionUpdatedInternal(remaining, total, 0.0f, 80.0f, i18nMessage);
         });

         //backup scripts (80 -> 85%)
         tools::CFileSystem::copyDirectoryRecursivelyTo(m_pathProvider.scriptsPath(), backupTempFolder / "scripts");
         OnProgressionUpdatedInternal(0, 100, 80.0f, 85.0f, "");

         //backup plugins data  (85 -> 90%)
         tools::CFileSystem::copyDirectoryRecursivelyTo(m_pathProvider.pluginsDataPath(), backupTempFolder / "data");
         boost::filesystem::copy_file("yadoms.ini", backupTempFolder / "yadoms.ini");
         OnProgressionUpdatedInternal(0, 100, 85.0f, 90.0f, "");


         //create if needed a backup folder
         if (!boost::filesystem::exists(m_pathProvider.backupPath()))
         {
            boost::filesystem::create_directory(m_pathProvider.backupPath());
         }

         

         //zip folder content (90 -> 100)
         std::string dateAsIsoString = boost::posix_time::to_iso_string(shared::currentTime::Provider().now());
         boost::replace_all(dateAsIsoString, ",", "_");

         std::ofstream out( (m_pathProvider.backupPath() / (boost::format("backup_%1%.zip") % dateAsIsoString).str() ).string(), std::ios::binary);
         Poco::Zip::Compress c(out, true);
         c.addRecursive(backupTempFolder.string());
         c.close(); // MUST be done to finalize the Zip file
         OnProgressionUpdatedInternal(0, 100, 90, 100.0f, "");

         //remove "backup temp" folder
         boost::filesystem::remove_all(backupTempFolder);
         boost::filesystem::remove(backupTempFolder);
         
      }
   } //namespace backup
} //namespace task


