#include "stdafx.h"
#include "ExportData.h"
#include <Poco/Zip/Compress.h>
#include <Poco/Zip/ZipException.h>
#include <Poco/Delegate.h>

#include <utility>
#include "i18n/ClientStrings.h"
#include "shared/Log.h"
#include "shared/currentTime/Provider.h"

namespace task
{
   namespace exportData
   {
      CExportData::CExportData(boost::shared_ptr<const IPathProvider> pathProvider,
                               std::unique_ptr<IExportDataHandler> exportDataHandler)
         : m_pathProvider(std::move(pathProvider)),
           m_exportDataHandler(std::move(exportDataHandler)),
           m_fileCountToZip(0),
           m_currentFileCount(0),
           m_lastProgressSent(-1.0f)
      {
      }

      std::string CExportData::getName()
      {
         return m_exportDataHandler->taskName();
      }

      void CExportData::onSetTaskId(const std::string& taskId)
      {
      }

      void CExportData::onProgressionUpdatedInternal(int remaining,
                                                     int total,
                                                     float currentPart,
                                                     float totalPart,
                                                     const std::string& message)
      {
         if (!m_reportProgressFct)
            return;

         const auto progression = currentPart + (total != 0
                                                    ? static_cast<float>(total - remaining) * (totalPart - currentPart) / static_cast<float>(total)
                                                    : 0);

         if (static_cast<int>(std::floor(progression)) == static_cast<int>(std::floor(m_lastProgressSent + 1.0f)))
            return;

         m_lastProgressSent = progression;
         m_reportProgressFct(true, progression, message, std::string(), shared::CDataContainer::make());
      }

      void CExportData::doWork(TaskProgressFunc reportProgressFct)
      {
         m_reportProgressFct = reportProgressFct;
         doWork(0);
      }

      bool CExportData::isCancellable() const
      {
         return true;
      }

      void CExportData::doWork(int currentTry)
      {
         boost::filesystem::path tempFolder;
         try
         {
            tempFolder = prepare();
            // 0% ==> 1%
            onProgressionUpdatedInternal(0,
                                         100,
                                         0.0f,
                                         1.0f,
                                         i18n::CClientStrings::ExportDataPrepare);

            m_exportDataHandler->collectDataTo(tempFolder,
                                               [this](int percent)
                                               {
                                                  // 1% ==> 59%
                                                  onProgressionUpdatedInternal(100 - percent,
                                                                               100,
                                                                               1.0f,
                                                                               59.0f,
                                                                               i18n::CClientStrings::ExportDataCopyFiles);
                                               });

            const auto zipFile = makeZipArchive(tempFolder);
            // 60% ==> 98%
            onProgressionUpdatedInternal(0,
                                         100,
                                         60.0f,
                                         98.0f,
                                         i18n::CClientStrings::ExportDataCompress);

            cleanup(tempFolder);
            // 99% ==> 100%
            onProgressionUpdatedInternal(0,
                                         100,
                                         99.0f,
                                         100.0f,
                                         i18n::CClientStrings::ExportDataSuccess);

            return;
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Fail to export data. Abort...";

            // 99% ==> 100%
            onProgressionUpdatedInternal(0,
                                         100,
                                         99.0f,
                                         100.0f,
                                         i18n::CClientStrings::ExportDataClean);

            if (!tempFolder.empty())
               cleanup(tempFolder);

            ++currentTry;
            if (currentTry >= m_exportDataHandler->maxTries())
               throw shared::exception::CException(ex.what());
         }

         doWork(currentTry);
      }

      boost::filesystem::path CExportData::prepare() const
      {
         auto tempFolder = boost::filesystem::temp_directory_path() / "exportData";

         if (!m_exportDataHandler->checkEnoughSpace(tempFolder.parent_path()))
         {
            YADOMS_LOG(error) << "No enough space to extract data into " << tempFolder;
            throw shared::exception::CException("No enough space in " + tempFolder.string() + " to extract data");
         }

         if (boost::filesystem::exists(tempFolder))
         {
            YADOMS_LOG(debug) << "Deleting " << tempFolder << " and its content";
            boost::filesystem::remove_all(tempFolder);
            boost::filesystem::remove(tempFolder);
         }

         YADOMS_LOG(debug) << "Create " << tempFolder;
         if (!boost::filesystem::create_directory(tempFolder))
         {
            YADOMS_LOG(error) << "Fail to create folder " << tempFolder;
            throw shared::exception::CException("fail to create export data folder");
         }

         return tempFolder;
      }

      boost::filesystem::path CExportData::makeZipArchive(const boost::filesystem::path& tempFolder)
      {
         // Create if needed the backup folder
         if (!boost::filesystem::exists(m_pathProvider->backupPath()))
            boost::filesystem::create_directory(m_pathProvider->backupPath());

         // Zip folder content (51 -> 98)
         auto dateAsIsoString = boost::posix_time::to_iso_string(shared::currentTime::Provider().now());
         boost::replace_all(dateAsIsoString, ",", "_");

         auto zipFilenameFinal = m_pathProvider->backupPath() /
            (boost::format("%1%_%2%.zip") % m_exportDataHandler->taskName() % dateAsIsoString).str();
         const auto zipFilename = m_pathProvider->backupPath() /
         (boost::format("%1%_%2%.zip.inprogress") % m_exportDataHandler->taskName() %
            dateAsIsoString).str();

         boost::filesystem::remove(zipFilenameFinal);
         boost::filesystem::remove(zipFilename);

         try
         {
            m_fileCountToZip = std::count_if(boost::filesystem::recursive_directory_iterator(tempFolder),
                                             boost::filesystem::recursive_directory_iterator(),
                                             static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file));
            m_currentFileCount = 0;

            std::ofstream out(zipFilename.string(), std::ios::binary);

            Poco::Zip::Compress c(out, true);
            c.EDone += Poco::Delegate<CExportData, const Poco::Zip::ZipLocalFileHeader>(this, &CExportData::onZipEDone);
            c.addRecursive(tempFolder.string());
            c.EDone -= Poco::Delegate<CExportData, const Poco::Zip::ZipLocalFileHeader>(this, &CExportData::onZipEDone);
            c.close(); // MUST be done to finalize the Zip file

            out.close();

            // Rename file from "{taskName}_{date}.zip.inprogress" to "{taskName}_{date}.zip"
            boost::filesystem::rename(zipFilename,
                                      zipFilenameFinal);

            return zipFilenameFinal;
         }
         catch (Poco::Exception& zex)
         {
            YADOMS_LOG(error) << "Fail to create zip archive (Poco::Exception)";
            YADOMS_LOG(error) << " class : " << zex.className();
            YADOMS_LOG(error) << " message : " << zex.message();
            YADOMS_LOG(error) << " text : " << zex.displayText();

            boost::filesystem::remove(zipFilename);
            throw;
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Fail to create zip archive (unknown exception)";
            YADOMS_LOG(error) << " message : " << ex.what();

            boost::filesystem::remove(zipFilename);
            throw;
         }
      }

      void CExportData::onZipEDone(const void* pSender,
                                   const Poco::Zip::ZipLocalFileHeader& hdr)
      {
         if (hdr.isFile())
         {
            m_currentFileCount++;

            // 60% ==> 98%
            onProgressionUpdatedInternal(m_fileCountToZip - m_currentFileCount,
                                         m_fileCountToZip,
                                         60.0f,
                                         98.0f,
                                         i18n::CClientStrings::ExportDataCompress);
            YADOMS_LOG(debug) << " ZIP : " << hdr.getFileName();
         }
      }

      void CExportData::cleanup(const boost::filesystem::path& tempFolder) const
      {
         boost::filesystem::remove_all(tempFolder);
         boost::filesystem::remove(tempFolder);
      }
   } //namespace exportData
} //namespace task
