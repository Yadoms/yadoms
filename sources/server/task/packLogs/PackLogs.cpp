#include "stdafx.h"
#include "task/ITask.h"
#include "PackLogs.h"
#include "tools/FileSystem.h"
#include <Poco/Zip/Compress.h>
#include <shared/currentTime/Provider.h>
#include <Poco/Zip/ZipException.h>
#include <Poco/Delegate.h>
#include "i18n/ClientStrings.h"

namespace task
{
   namespace packLogs
   {
      std::string CPackLogs::m_taskName = "packLogs";

      CPackLogs::CPackLogs(boost::shared_ptr<const IPathProvider> pathProvider)
         : m_pathProvider(pathProvider)
      {
      }

      const std::string& CPackLogs::getName() const
      {
         return m_taskName;
      }

      void CPackLogs::onProgressionUpdatedInternal(float progression,
                                                   const std::string& message) const
      {
         if (m_reportRealProgress)
            m_reportRealProgress(true, progression, message, std::string(), shared::CDataContainer::EmptyContainer);
      }

      void CPackLogs::doWork(TaskProgressFunc functor)
      {
         m_reportRealProgress = functor;
         doWork(0);
      }

      void CPackLogs::doWork(int currentTry)
      {
         try
         {
            auto logsTempFolder = prepare();
            if (copyLogsFiles(logsTempFolder))
            {
               auto zipFile = makeZipArchive(logsTempFolder);
               cleanup(logsTempFolder);
               onProgressionUpdatedInternal(99.0f, i18n::CClientStrings::PackLogsSuccess);
            }
            else
            {
               // Fail to pack files, just cleanup and ends with error
               cleanup(logsTempFolder);
               std::string errorMessage = "fail to copy files to temp folder : ";
               errorMessage += logsTempFolder.string();
               throw shared::exception::CException(errorMessage);
            }
         }
         catch (std::exception& ex)
         {
            std::string innerMessage = "Fail to packing logs after 3 tries : ";
            innerMessage += ex.what();

            if (currentTry == 3)
            {
               YADOMS_LOG(error) << "Fail to packing logs after 3 times. Abort...";
               throw shared::exception::CException(innerMessage);
            }
            else
            {
               YADOMS_LOG(warning) << "Fail to packing logs. Retry...";
               doWork(currentTry + 1);
            }
         }
      }

      boost::filesystem::path CPackLogs::prepare() const
      {
         //create "logs temp" folder
         auto logsTempFolder = boost::filesystem::temp_directory_path() / "yadomsLogs";

         //if folder exist, cleanup, else create if
         if (boost::filesystem::exists(logsTempFolder))
         {
            YADOMS_LOG(debug) << "Deleting " << logsTempFolder << " and its content";
            boost::filesystem::remove_all(logsTempFolder);
            boost::filesystem::remove(logsTempFolder);
         }
         else
         {
            YADOMS_LOG(debug) << "Folder " << logsTempFolder << " does not exist";
         }

         YADOMS_LOG(debug) << "Create " << logsTempFolder;
         if (!boost::filesystem::create_directory(logsTempFolder))
         {
            YADOMS_LOG(error) << "Fail to create folder " << logsTempFolder;
            throw shared::exception::CException("fail to create temp logs folder");
         }
         onProgressionUpdatedInternal(1.0f, i18n::CClientStrings::PackLogsPrepare);
         return logsTempFolder;
      }

      bool CPackLogs::copyLogsFiles(boost::filesystem::path& logsTempFolder) const
      {
         if (tools::CFileSystem::copyDirectoryRecursivelyTo(m_pathProvider->logsPath(), logsTempFolder / "logs"))
         {
            onProgressionUpdatedInternal(50.0f, i18n::CClientStrings::PackLogsCopyFile);

            // Create if needed the backup folder
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
         
         YADOMS_LOG(error) << "Fail to copy logs to temp folder";
         return false;
      }

      boost::filesystem::path CPackLogs::makeZipArchive(boost::filesystem::path& logsTempFolder)
      {
         //zip folder content (51 -> 98)
         auto zipFilenameFinal = m_pathProvider->backupPath() / "logs.zip";
         auto zipFilename = m_pathProvider->backupPath() / "logs.zip.inprogress";

         boost::filesystem::remove(zipFilenameFinal);
         boost::filesystem::remove(zipFilename);

         try
         {
            std::ofstream out(zipFilename.string(), std::ios::binary);
            Poco::Zip::Compress c(out, true);
            c.EDone += Poco::Delegate<CPackLogs, const Poco::Zip::ZipLocalFileHeader>(this, &CPackLogs::onZipEDone);
            c.addRecursive(logsTempFolder.string());
            c.EDone -= Poco::Delegate<CPackLogs, const Poco::Zip::ZipLocalFileHeader>(this, &CPackLogs::onZipEDone);
            c.close(); // MUST be done to finalize the Zip file
            onProgressionUpdatedInternal(98.0f, i18n::CClientStrings::PackLogsCompress);
            out.close();

            //rename file from "logs_date.zip.inprogress" to "logs_date.zip"
            boost::filesystem::rename(zipFilename, zipFilenameFinal);

            //validate the file
            return zipFilenameFinal;
         }
         catch (Poco::Exception& zex)
         {
            YADOMS_LOG(error) << "Fail to create zip archive (Poco::Exception)";
            YADOMS_LOG(error) << " class : " << zex.className();
            YADOMS_LOG(error) << " message : " << zex.message();
            YADOMS_LOG(error) << " text : " << zex.displayText();

            //remove falsy zip file
            boost::filesystem::remove(zipFilename);
            throw;
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Fail to create zip archive (unknown exception)";
            YADOMS_LOG(error) << " message : " << ex.what();

            //remove falsy zip file
            boost::filesystem::remove(zipFilename);
            throw;
         }
      }

      // ReSharper disable once CppMemberFunctionMayBeConst
      void CPackLogs::onZipEDone(const void* pSender,
                                 const Poco::Zip::ZipLocalFileHeader& hdr)
      {
         if (hdr.isFile())
         {
            onProgressionUpdatedInternal(80.0f, i18n::CClientStrings::PackLogsCompress);
            YADOMS_LOG(debug) << " ZIP : " << hdr.getFileName();
         }
      }

      void CPackLogs::cleanup(boost::filesystem::path& logsTempFolder) const
      {
         onProgressionUpdatedInternal(98.0f, i18n::CClientStrings::PackLogsClean);
         boost::filesystem::remove_all(logsTempFolder);
      }
   } //namespace packLogs
} //namespace task
