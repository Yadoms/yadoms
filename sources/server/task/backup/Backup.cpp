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

namespace task
{
   namespace backup
   {
      std::string CBackup::m_taskName = "backup";

      CBackup::CBackup(boost::shared_ptr<const IPathProvider> pathProvider, boost::shared_ptr<database::IDataBackup> dataBackupInterface)
         : m_pathProvider(pathProvider), m_dataBackupInterface(dataBackupInterface)
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
         float progression = currentPart + (total != 0 ? static_cast<float>(total - remaining) * static_cast<float>(totalPart - currentPart) / static_cast<float>(total) : 0);

         if (m_reportRealProgress)
            m_reportRealProgress(true, progression, message, std::string(), shared::CDataContainer::EmptyContainer);
      }

      void CBackup::onZipEDone(const void* pSender, const Poco::Zip::ZipLocalFileHeader& hdr)
      {
         YADOMS_LOG(debug) << " ZIP : " << hdr.getFileName();
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
            }
            else
            {
               //fail to backup files, just cleanup and ends with error
               cleanup(backupTempFolder);
               throw std::exception("Fail to backup after 3 tries");
            }
         }
         catch (std::exception &ex)
         {
            if (currentTry == 3)
               throw std::exception("Fail to backup after 3 tries");
            else
               doWork(currentTry + 1);
         }
      }

      boost::filesystem::path CBackup::prepareBackup()
      {

         //create "backup temp" folder
         boost::filesystem::path backupTempFolder = boost::filesystem::temp_directory_path() / "yadomsBackup";

         //if folder exist, cleanup, else create if
         if (boost::filesystem::exists(backupTempFolder))
         {
            YADOMS_LOG(debug) << "Deleting " << backupTempFolder << " and its content";
            boost::filesystem::remove_all(backupTempFolder);
            boost::filesystem::remove(backupTempFolder);
         }
         else
         {
            YADOMS_LOG(debug) << "Folder " << backupTempFolder << " do not exist";
         }

         YADOMS_LOG(debug) << "Create " << backupTempFolder;
         if (!boost::filesystem::create_directory(backupTempFolder))
         {
            YADOMS_LOG(error) << "Fail to create folder " << backupTempFolder;
            throw new std::exception("fail to create temp backup folder");
         }
         return backupTempFolder;
      }

      bool CBackup::backupFiles(boost::filesystem::path & backupTempFolder)
      {
         //backup database (0 -> 80%)
         if (m_dataBackupInterface->backupSupported())
         {
            m_dataBackupInterface->backupData(backupTempFolder.string(), [&](int remaining, int total, std::string i18nMessage, std::string error)
            {
               OnProgressionUpdatedInternal(remaining, total, 0.0f, 80.0f, i18nMessage);
            });
         }
         else
         {
            //database backup not supported
         }

         //backup scripts (80 -> 85%)
         if (tools::CFileSystem::copyDirectoryRecursivelyTo(m_pathProvider->scriptsPath(), backupTempFolder / "scripts"))
         {
            OnProgressionUpdatedInternal(0, 100, 80.0f, 85.0f, "");

            //backup plugins data  (85 -> 90%)
            if (tools::CFileSystem::copyDirectoryRecursivelyTo(m_pathProvider->pluginsDataPath(), backupTempFolder / "data"))
            {
               boost::system::error_code ec;
               boost::filesystem::copy_file("yadoms.ini", backupTempFolder / "yadoms.ini", ec);
               OnProgressionUpdatedInternal(0, 100, 85.0f, 90.0f, "");


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

         //zip folder content (90 -> 100)
         std::string dateAsIsoString = boost::posix_time::to_iso_string(shared::currentTime::Provider().now());
         boost::replace_all(dateAsIsoString, ",", "_");

         boost::filesystem::path zipFilenameFinal = m_pathProvider->backupPath() / (boost::format("backup_%1%.zip") % dateAsIsoString).str();
         boost::filesystem::path zipFilename = m_pathProvider->backupPath() / (boost::format("backup_%1%.zip.inprogress") % dateAsIsoString).str();
         try
         {
            std::ofstream out(zipFilename.string(), std::ios::binary);
            Poco::Zip::Compress c(out, true);
            c.EDone += Poco::Delegate<CBackup, const Poco::Zip::ZipLocalFileHeader>(this, &CBackup::onZipEDone);
            c.addRecursive(backupTempFolder.string());
            c.EDone -= Poco::Delegate<CBackup, const Poco::Zip::ZipLocalFileHeader>(this, &CBackup::onZipEDone);
            c.close(); // MUST be done to finalize the Zip file
            OnProgressionUpdatedInternal(0, 100, 90, 100.0f, "");
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
            throw zex;
         }
         catch (std::exception & ex)
         {
            YADOMS_LOG(error) << "Fail to create zip archive (unknown exception)";
            YADOMS_LOG(error) << " message : " << ex.what();

            //remove falsy zip file
            boost::filesystem::remove(zipFilename);
            throw ex;
         }
        
      }

      void CBackup::cleanup(boost::filesystem::path & backupTempFolder)
      {
         //remove "backup temp" folder
         boost::filesystem::remove_all(backupTempFolder);
         boost::filesystem::remove(backupTempFolder);
      }

   } //namespace backup
} //namespace task


