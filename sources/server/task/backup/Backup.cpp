#include "stdafx.h"
#include <shared/exception/InvalidParameter.hpp>
#include "database/IDataBackup.h"
#include "task/ITask.h"
#include "Backup.h"
#include "tools/FileSystem.h"
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

      CBackup::~CBackup()
      {
      }

      const std::string& CBackup::getName() const
      {
         return m_taskName;
      }

      void CBackup::OnProgressionUpdatedInternal(int remaining, int total, float currentPart, float totalPart, const std::string& message) const
      {
         const auto progression = currentPart + (total != 0 ? static_cast<float>(total - remaining) * static_cast<float>(totalPart - currentPart) / static_cast<float>(total) : 0);

         if (m_reportRealProgress)
            m_reportRealProgress(true, progression, message, std::string(), shared::CDataContainer::EmptyContainer);
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
            boost::filesystem::path backupTempFolder = prepareBackup();
            if (backupFiles(backupTempFolder))
            {
               boost::filesystem::path zipFile = makeZipArchive(backupTempFolder);
               cleanup(backupTempFolder);
               OnProgressionUpdatedInternal(0, 100, 99.0f, 100.0f, i18n::CClientStrings::BackupSuccess);
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
            else
            {
               YADOMS_LOG(warning) << "Fail to realize backup. Retry...";
               doWork(currentTry + 1);
            }
         }
      }

      bool CBackup::checkEnoughSpace(const boost::filesystem::path& where) const
      {
         uintmax_t neededSpace = 0;

         YADOMS_LOG(info) << "Check backup space needed";

         // Database
         if (m_dataBackupInterface->backupSupported())
         {
            const auto needed = m_dataBackupInterface->backupNeededSpace();
            YADOMS_LOG(info) << "  - Database : " << needed;
            neededSpace += needed;
         }

         // Scripts
         {
            const auto needed = boost::filesystem::space(m_pathProvider->scriptsPath()).available;
            YADOMS_LOG(info) << "  - Scripts : " << needed;
            neededSpace += needed;
         }

         // Plugins data
         {
            const auto needed = boost::filesystem::space(m_pathProvider->pluginsDataPath()).available;
            YADOMS_LOG(info) << "  - Plugins data : " << needed;
            neededSpace += needed;
         }
         
         YADOMS_LOG(info) << "  Total : " << neededSpace;

         // Apply 5% marging
         return boost::filesystem::space(where).available > (neededSpace * 105 / 100);
      }

      boost::filesystem::path CBackup::prepareBackup() const
      {
         boost::filesystem::path backupTempFolder = boost::filesystem::temp_directory_path() / "yadomsBackup";

         if (!checkEnoughSpace(backupTempFolder))
            YADOMS_LOG(warning) << "No enough space to backup into " << backupTempFolder;

         backupTempFolder = m_pathProvider->backupPath() / "yadomsBackup"; //TODO vérifier que le sous-répertoire yadomsBackup n'apparait pas dans la liste des backups
         YADOMS_LOG(warning) << "Retry in " << backupTempFolder << "...";
         if (!checkEnoughSpace(backupTempFolder))
            throw shared::exception::CException("No enough space in " + backupTempFolder + " to process backup");

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
         OnProgressionUpdatedInternal(0, 100, 0.0f, 1.0f, i18n::CClientStrings::BackupPrepare);
         return backupTempFolder;
      }

      bool CBackup::backupFiles(boost::filesystem::path & backupTempFolder)
      {
         //backup database (1 -> 40%)
         if (m_dataBackupInterface->backupSupported())
         {
            m_dataBackupInterface->backupData(backupTempFolder.string(), [&](int remaining, int total, std::string i18nMessage, std::string error)
            {
               OnProgressionUpdatedInternal(remaining, total, 1.0f, 40.0f, i18n::CClientStrings::BackupCopyFile);
            });
         }
         else
         {
            //database backup not supported
         }

         //backup scripts (40 -> 50%)
         if (tools::CFileSystem::copyDirectoryRecursivelyTo(m_pathProvider->scriptsPath(), backupTempFolder / "scripts"))
         {
            OnProgressionUpdatedInternal(0, 100, 40.0f, 50.0f, i18n::CClientStrings::BackupCopyFile);

            //backup plugins data  (50 -> 60%)
            if (tools::CFileSystem::copyDirectoryRecursivelyTo(m_pathProvider->pluginsDataPath(), backupTempFolder / "data"))
            {
               boost::system::error_code ec;
               boost::filesystem::copy_file("yadoms.ini", backupTempFolder / "yadoms.ini", ec);
               OnProgressionUpdatedInternal(0, 100, 50.0f, 60.0f, i18n::CClientStrings::BackupCopyFile);


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
         std::string dateAsIsoString = boost::posix_time::to_iso_string(shared::currentTime::Provider().now());
         boost::replace_all(dateAsIsoString, ",", "_");

         boost::filesystem::path zipFilenameFinal = m_pathProvider->backupPath() / (boost::format("backup_%1%.zip") % dateAsIsoString).str();
         boost::filesystem::path zipFilename = m_pathProvider->backupPath() / (boost::format("backup_%1%.zip.inprogress") % dateAsIsoString).str();
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
            OnProgressionUpdatedInternal(0, 100, 60.0f, 98.0f, i18n::CClientStrings::BackupCompress);
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
            OnProgressionUpdatedInternal(m_fileCountToZip - m_currentFileCount, m_fileCountToZip, 60.0f, 98.0f, i18n::CClientStrings::BackupCompress);
            YADOMS_LOG(debug) << " ZIP : " << hdr.getFileName();
         }
      }


      void CBackup::cleanup(boost::filesystem::path & backupTempFolder) const
      {
         OnProgressionUpdatedInternal(0, 100, 98.0f, 99.0f, i18n::CClientStrings::BackupClean);
         //remove "backup temp" folder
         boost::filesystem::remove_all(backupTempFolder);
         boost::filesystem::remove(backupTempFolder);
      }

   } //namespace backup
} //namespace task


