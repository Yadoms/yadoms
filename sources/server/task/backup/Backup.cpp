#include "stdafx.h"
#include <shared/exception/InvalidParameter.hpp>
#include "database/IDataBackup.h"
#include "task/ITask.h"
#include "Backup.h"
#include "tools/FileSystem.h"
#include <shared/tools/Filesystem.h>
#include <Poco/Zip/Compress.h>
#include <shared/currentTime/Provider.h>
#include <Poco/Zip/ZipException.h>
#include <Poco/Delegate.h>
#include "i18n/ClientStrings.h"

namespace task
{
   namespace backup
   {
      std::string CBackup::m_taskName = "backup";

      CBackup::CBackup(boost::shared_ptr<const IPathProvider> pathProvider, boost::shared_ptr<database::IDataBackup> dataBackupInterface)
         : m_pathProvider(pathProvider), m_dataBackupInterface(dataBackupInterface), m_fileCountToZip(0), m_currentFileCount(0)
      {
         if (!m_dataBackupInterface)
            throw shared::exception::CInvalidParameter("dataBackupInterface");
      }

      const std::string& CBackup::getName() const
      {
         return m_taskName;
      }

      void CBackup::onProgressionUpdatedInternal(int remaining, int total, float currentPart, float totalPart, const std::string& message) const
      {
		  if (m_reportRealProgress)
		  {
			  const auto progression = currentPart + (total != 0 ? static_cast<float>(total - remaining) * static_cast<float>(totalPart - currentPart) / static_cast<float>(total) : 0);
			  m_reportRealProgress(true, progression, message, std::string(), new_CDataContainerSharedPtr());
		  }
      }

      void CBackup::doWork(TaskProgressFunc pFunctor)
      {
         m_reportRealProgress = pFunctor;
         doWork(0);
      }

      void CBackup::doWork(int currentTry)
      {
         try
         {
            auto backupTempFolder = prepareBackup();
            if (backupFiles(backupTempFolder))
            {
               const auto zipFile = makeZipArchive(backupTempFolder);
               cleanup(backupTempFolder);
               onProgressionUpdatedInternal(0, 100, 99.0f, 100.0f, i18n::CClientStrings::BackupSuccess);
            }
            else
            {
               //fail to backup files, just cleanup and ends with error
               cleanup(backupTempFolder);
               std::string errorMessage = "fail to copy files to temp folder : ";
               errorMessage += backupTempFolder.string();
               throw shared::exception::CException(errorMessage);
            }
         }
         catch (std::exception &ex)
         {
            std::string innerMessage = "Fail to backup after 3 tries : ";
            innerMessage += ex.what();
            

            if (currentTry == 3)
            {
               YADOMS_LOG(error) << "Fail to realize backup 3 times. Abort...";
               throw shared::exception::CException(innerMessage);
            }
            
            YADOMS_LOG(warning) << "Fail to realize backup. Retry...";
            doWork(currentTry + 1);
         }
      }

      bool CBackup::checkEnoughSpace(const boost::filesystem::path& where) const
      {
         uintmax_t neededSpace = 0;

         YADOMS_LOG(information) << "Check backup space needed";

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
         return boost::filesystem::space(where).available > (neededSpace * 105 / 100);
      }

      boost::filesystem::path CBackup::prepareBackup() const
      {
         auto backupTempFolder = boost::filesystem::temp_directory_path() / "yadomsBackup";

         if (!checkEnoughSpace(backupTempFolder.parent_path()))
         {
            YADOMS_LOG(warning) << "No enough space to backup into " << backupTempFolder;

            backupTempFolder = m_pathProvider->backupPath() / "yadomsBackup";
            YADOMS_LOG(warning) << "Retry in " << backupTempFolder << "...";
            if (!checkEnoughSpace(backupTempFolder.parent_path()))
               throw shared::exception::CException("No enough space in " + backupTempFolder.string() + " to process backup");
         }

         //if folder exist, cleanup, else create if
         if (boost::filesystem::exists(backupTempFolder))
         {
            YADOMS_LOG(debug) << "Deleting " << backupTempFolder << " and its content";
            boost::filesystem::remove_all(backupTempFolder);
            boost::filesystem::remove(backupTempFolder);
         }
         else
         {
            YADOMS_LOG(debug) << "Folder " << backupTempFolder << " does not exist";
         }

         YADOMS_LOG(debug) << "Create " << backupTempFolder;
         if (!boost::filesystem::create_directory(backupTempFolder))
         {
            YADOMS_LOG(error) << "Fail to create folder " << backupTempFolder;
            throw shared::exception::CException("fail to create temp backup folder");
         }
         onProgressionUpdatedInternal(0, 100, 0.0f, 1.0f, i18n::CClientStrings::BackupPrepare);
         return backupTempFolder;
      }

      bool CBackup::backupFiles(boost::filesystem::path & backupTempFolder)
      {
         //backup database (1 -> 40%)
         if (m_dataBackupInterface->backupSupported())
         {
            m_dataBackupInterface->backupData(backupTempFolder.string(), [&](int remaining, int total, std::string i18nMessage, std::string error)
            {
               onProgressionUpdatedInternal(remaining, total, 1.0f, 40.0f, i18n::CClientStrings::BackupCopyFile);
            });
         }
         else
         {
            //database backup not supported
         }

         //backup scripts (40 -> 50%)
         if (tools::CFileSystem::copyDirectoryRecursivelyTo(m_pathProvider->scriptsPath(), backupTempFolder / "scripts"))
         {
            onProgressionUpdatedInternal(0, 100, 40.0f, 50.0f, i18n::CClientStrings::BackupCopyFile);

            //backup plugins data  (50 -> 60%)
            if (tools::CFileSystem::copyDirectoryRecursivelyTo(m_pathProvider->pluginsDataPath(), backupTempFolder / "data"))
            {
               boost::system::error_code ec;
               boost::filesystem::copy_file("yadoms.ini", backupTempFolder / "yadoms.ini", ec);
               onProgressionUpdatedInternal(0, 100, 50.0f, 60.0f, i18n::CClientStrings::BackupCopyFile);


               //create if needed a backup folder
               if (!boost::filesystem::exists(m_pathProvider->backupPath()))
               {
                  boost::filesystem::create_directory(m_pathProvider->backupPath());
               }
               else
               {
                  YADOMS_LOG(debug) << "Folder " << m_pathProvider->backupPath().string() << " already exists. Do not create it";
               }
               return true;
            }
            else
            {
               YADOMS_LOG(error) << "Fail to copy plugins data to temp folder";
            }
         }
         else 
         {
            YADOMS_LOG(error) << "Fail to copy scripts to temp folder";
         }
         return false;
      }

      boost::filesystem::path CBackup::makeZipArchive(boost::filesystem::path & backupTempFolder)
      {

         //zip folder content (60 -> 99)
         auto dateAsIsoString = boost::posix_time::to_iso_string(shared::currentTime::Provider().now());
         boost::replace_all(dateAsIsoString, ",", "_");

         auto zipFilenameFinal = m_pathProvider->backupPath() / (boost::format("backup_%1%.zip") % dateAsIsoString).str();
         auto zipFilename = m_pathProvider->backupPath() / (boost::format("backup_%1%.zip.inprogress") % dateAsIsoString).str();
         try
         {
            //count files
            m_fileCountToZip = std::count_if(boost::filesystem::recursive_directory_iterator(backupTempFolder), boost::filesystem::recursive_directory_iterator(), static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file));
            m_currentFileCount = 0;

            std::ofstream out(zipFilename.string(), std::ios::binary);
            Poco::Zip::Compress c(out, true);
            c.EDone += Poco::Delegate<CBackup, const Poco::Zip::ZipLocalFileHeader>(this, &CBackup::onZipEDone);
            c.addRecursive(backupTempFolder.string());
            c.EDone -= Poco::Delegate<CBackup, const Poco::Zip::ZipLocalFileHeader>(this, &CBackup::onZipEDone);
            c.close(); // MUST be done to finalize the Zip file
            onProgressionUpdatedInternal(0, 100, 60.0f, 98.0f, i18n::CClientStrings::BackupCompress);
            out.close();

            //rename file from "backup_date.zip.inprogress" to "backup_date.zip"
            boost::filesystem::rename(zipFilename, zipFilenameFinal);

            //validate the file
            return zipFilenameFinal;
         }
         catch (Poco::Exception & zex)
         {
            YADOMS_LOG(error) << "Fail to create zip archive (Poco::Exception)";
            YADOMS_LOG(error) << " class : " << zex.className();
            YADOMS_LOG(error) << " message : " << zex.message();
            YADOMS_LOG(error) << " text : " << zex.displayText();

            //remove falsy zip file
            boost::filesystem::remove(zipFilename);
            throw;
         }
         catch (std::exception & ex)
         {
            YADOMS_LOG(error) << "Fail to create zip archive (unknown exception)";
            YADOMS_LOG(error) << " message : " << ex.what();

            //remove falsy zip file
            boost::filesystem::remove(zipFilename);
            throw;
         }
        
      }

      void CBackup::onZipEDone(const void* pSender, const Poco::Zip::ZipLocalFileHeader& hdr)
      {
         if (hdr.isFile())
         {
            //going from 60 to 98
            m_currentFileCount++;
            onProgressionUpdatedInternal(m_fileCountToZip - m_currentFileCount, m_fileCountToZip, 60.0f, 98.0f, i18n::CClientStrings::BackupCompress);
            YADOMS_LOG(debug) << " ZIP : " << hdr.getFileName();
         }
      }


      void CBackup::cleanup(boost::filesystem::path & backupTempFolder) const
      {
         onProgressionUpdatedInternal(0, 100, 98.0f, 99.0f, i18n::CClientStrings::BackupClean);
         //remove "backup temp" folder
         boost::filesystem::remove_all(backupTempFolder);
         boost::filesystem::remove(backupTempFolder);
      }

   } //namespace backup
} //namespace task


