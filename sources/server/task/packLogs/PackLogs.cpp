#include "stdafx.h"
#include "task/ITask.h"
#include "PackLogs.h"
#include <Poco/Zip/Compress.h>
#include <Poco/Zip/ZipException.h>
#include <Poco/Delegate.h>
#include "i18n/ClientStrings.h"
#include <shared/Log.h>

#include <utility>

#include "shared/currentTime/Provider.h"
#include "shared/tools/Filesystem.h"

namespace task
{
   namespace packLogs
   {
      const std::string CPackLogs::TaskName = "packLogs";

      CPackLogs::CPackLogs(boost::shared_ptr<const IPathProvider> pathProvider)
         : m_pathProvider(std::move(pathProvider)),
           m_fileCountToZip(0),
           m_currentFileCount(0)
      {
      }

      const std::string& CPackLogs::getName() const
      {
         return TaskName;
      }

      void CPackLogs::onSetTaskId(const std::string& taskId)
      {
      }

      void CPackLogs::onProgressionUpdatedInternal(int remaining,
                                                   int total,
                                                   float currentPart,
                                                   float totalPart,
                                                   const std::string& message) const
      {
         if (!m_reportRealProgress)
            return;

         const auto progression = currentPart + (total != 0
                                                    ? static_cast<float>(total - remaining) * (totalPart - currentPart) / static_cast<float>(total)
                                                    : 0);
         m_reportRealProgress(true, progression, message, std::string(), shared::CDataContainer::make());
      }

      void CPackLogs::doWork(TaskProgressFunc functor)
      {
         m_reportRealProgress = functor;
         doWork(0);
      }

      bool CPackLogs::isCancellable() const
      {
         return true;
      }

      void CPackLogs::doWork(int currentTry)
      {
         boost::filesystem::path tempFolder;
         try
         {
            tempFolder = prepare();
            if (copyLogsFiles(tempFolder))
            {
               const auto zipFile = makeZipArchive(tempFolder);
               cleanup(tempFolder);
               onProgressionUpdatedInternal(0, 100, 99.0f, 100.0f, i18n::CClientStrings::PackLogsSuccess);
            }
            else
            {
               // Fail to pack files, just cleanup and ends with error
               cleanup(tempFolder);
               std::string errorMessage = "fail to copy files to temp folder : ";
               errorMessage += tempFolder.string();
               throw shared::exception::CException(errorMessage);
            }
         }
         catch (boost::thread_interrupted&)
         {
            YADOMS_LOG(information) << "Cancel logs packaging asked...";
            if (!tempFolder.empty())
               cleanup(tempFolder);
            throw shared::exception::CException("Logs packaging cancelled");
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

            YADOMS_LOG(warning) << "Fail to packing logs. Retry...";
            doWork(currentTry + 1);
         }
      }

      bool CPackLogs::checkEnoughSpace(const boost::filesystem::path& where) const
      {
         uintmax_t neededSpace = 0;

         YADOMS_LOG(information) << "Check space needed...";

         // Plugins data
         {
            const auto needed = shared::tools::CFilesystem::directorySize(m_pathProvider->logsPath());
            YADOMS_LOG(information) << "  - Logs : " << needed;
            neededSpace += needed;
         }

         YADOMS_LOG(information) << "  Total : " << neededSpace;

         // Apply 5% margin
         return boost::filesystem::space(where).available > (neededSpace * 105 / 100);
      }

      boost::filesystem::path CPackLogs::prepare() const
      {
         auto tempFolder = boost::filesystem::temp_directory_path() / "yadomsLogs";

         if (!checkEnoughSpace(tempFolder.parent_path()))
         {
            YADOMS_LOG(warning) << "No enough space to package logs into " << tempFolder;

            tempFolder = m_pathProvider->backupPath() / "yadomsLogs";
            YADOMS_LOG(warning) << "Retry in " << tempFolder << "...";
            if (!checkEnoughSpace(tempFolder.parent_path()))
               throw shared::exception::CException("No enough space in " + tempFolder.string() + " to process packaging logs");
         }

         //if folder exist, cleanup, else create if
         if (boost::filesystem::exists(tempFolder))
         {
            YADOMS_LOG(debug) << "Deleting " << tempFolder << " and its content";
            boost::filesystem::remove_all(tempFolder);
            boost::filesystem::remove(tempFolder);
         }
         else
         {
            YADOMS_LOG(debug) << "Folder " << tempFolder << " does not exist";
         }

         YADOMS_LOG(debug) << "Create " << tempFolder;
         if (!boost::filesystem::create_directory(tempFolder))
         {
            YADOMS_LOG(error) << "Fail to create folder " << tempFolder;
            throw shared::exception::CException("fail to create temp logs folder");
         }
         onProgressionUpdatedInternal(0, 100, 0.0f, 1.0f, i18n::CClientStrings::PackLogsPrepare);
         return tempFolder;
      }

      bool CPackLogs::copyLogsFiles(const boost::filesystem::path& tempFolder) const
      {
         //backup database (1 -> 60%)
         if (shared::tools::CFilesystem::copyDirectoryRecursivelyTo(m_pathProvider->logsPath(), tempFolder / "logs"))
         {
            onProgressionUpdatedInternal(0, 100, 1.0f, 60, i18n::CClientStrings::PackLogsCopyFile);

            // Create if needed the backup folder
            if (!boost::filesystem::exists(m_pathProvider->backupPath()))
               boost::filesystem::create_directory(m_pathProvider->backupPath());
            else
               YADOMS_LOG(debug) << "Folder " << m_pathProvider->backupPath().string() << " already exists. Do not create it";

            return true;
         }

         YADOMS_LOG(error) << "Fail to copy logs to temp folder";
         return false;
      }

      boost::filesystem::path CPackLogs::makeZipArchive(const boost::filesystem::path& tempFolder)
      {
         //zip folder content (60 -> 99)
         auto dateAsIsoString = boost::posix_time::to_iso_string(shared::currentTime::Provider().now());
         boost::replace_all(dateAsIsoString, ",", "_");

         auto zipFilenameFinal = m_pathProvider->backupPath() / (boost::format("logs_%1%.zip") % dateAsIsoString).str();
         const auto zipFilename = m_pathProvider->backupPath() / (boost::format("logs_%1%.zip.inprogress") % dateAsIsoString).str();

         try
         {
            //count files
            m_fileCountToZip = std::count_if(boost::filesystem::recursive_directory_iterator(tempFolder),
                                             boost::filesystem::recursive_directory_iterator(),
                                             static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file));
            m_currentFileCount = 0;

            std::ofstream out(zipFilename.string(), std::ios::binary);
            Poco::Zip::Compress c(out, true);
            c.EDone += Poco::Delegate<CPackLogs, const Poco::Zip::ZipLocalFileHeader>(this, &CPackLogs::onZipEDone);
            c.addRecursive(tempFolder.string());
            c.EDone -= Poco::Delegate<CPackLogs, const Poco::Zip::ZipLocalFileHeader>(this, &CPackLogs::onZipEDone);
            c.close(); // MUST be done to finalize the Zip file
            onProgressionUpdatedInternal(0, 100, 60.0f, 98.0f, i18n::CClientStrings::PackLogsCompress);
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

      void CPackLogs::onZipEDone(const void* pSender,
                                 const Poco::Zip::ZipLocalFileHeader& hdr)
      {
         if (hdr.isFile())
         {
            //going from 60 to 98
            m_currentFileCount++;
            onProgressionUpdatedInternal(m_fileCountToZip - m_currentFileCount,
                                         m_fileCountToZip,
                                         60.0f,
                                         98.0f,
                                         i18n::CClientStrings::PackLogsCompress);
            YADOMS_LOG(debug) << " ZIP : " << hdr.getFileName();
         }
      }

      void CPackLogs::cleanup(const boost::filesystem::path& tempFolder) const
      {
         onProgressionUpdatedInternal(0,
                                      100,
                                      98.0f,
                                      99.0f,
                                      i18n::CClientStrings::PackLogsClean);

         boost::filesystem::remove_all(tempFolder);
         boost::filesystem::remove(tempFolder);
      }
   } //namespace packLogs
} //namespace task
