#include "stdafx.h"
#include "ExportData.h"
#include <Poco/Zip/Compress.h>
#include <Poco/Zip/ZipException.h>
#include <Poco/Delegate.h>

#include <utility>
#include "i18n/ClientStrings.h"
#include "shared/Log.h"
#include "database/IAcquisitionRequester.h"

namespace task
{
   namespace exportData
   {
      const std::string CExportData::TaskName = "exportData";

      CExportData::CExportData(boost::shared_ptr<const IPathProvider> pathProvider,
                               boost::shared_ptr<database::IKeywordRequester> keywordRequester,
                               boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                               int keywordId)
         : m_pathProvider(std::move(pathProvider)),
           m_keywordRequester(std::move(keywordRequester)),
           m_acquisitionRequester(std::move(acquisitionRequester)),
           m_keywordId(keywordId),
           m_fileCountToZip(0),
           m_currentFileCount(0),
           m_lastProgressSent(-1.0f)
      {
      }

      const std::string& CExportData::getName() const
      {
         return TaskName;
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
         if (!m_reportRealProgress)
            return;

         const auto progression = currentPart + (total != 0
                                                    ? static_cast<float>(total - remaining) * (totalPart - currentPart) / static_cast<float>(total)
                                                    : 0);

         if (static_cast<int>(std::floor(progression)) == static_cast<int>(std::floor(m_lastProgressSent + 1.0f)))
            return;

         m_lastProgressSent = progression;
         m_reportRealProgress(true, progression, message, std::string(), shared::CDataContainer::make());
      }

      void CExportData::doWork(TaskProgressFunc functor)
      {
         m_reportRealProgress = functor;
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
            collectDataTo(tempFolder);
            const auto zipFile = makeZipArchive(tempFolder);
            cleanup(tempFolder);
            onProgressionUpdatedInternal(0, 100, 0.0f, 100.0f, i18n::CClientStrings::ExportDataSuccess);
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Fail to export data. Abort...";
            if (!tempFolder.empty())
               cleanup(tempFolder);
            throw shared::exception::CException(ex.what());
         }
      }

      bool CExportData::checkEnoughSpace(const boost::filesystem::path& where) const
      {
         return true;
      }

      boost::filesystem::path CExportData::prepare()
      {
         auto tempFolder = boost::filesystem::temp_directory_path() / "exportData";

         if (!checkEnoughSpace(tempFolder.parent_path()))
         {
            YADOMS_LOG(warning) << "No enough space to extract data into " << tempFolder;

            tempFolder = m_pathProvider->backupPath() / "exportData";
            YADOMS_LOG(warning) << "Retry in " << tempFolder << "...";
            if (!checkEnoughSpace(tempFolder.parent_path()))
               throw shared::exception::CException("No enough space in " + tempFolder.string() + " to process extract data");
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
            throw shared::exception::CException("fail to create export data folder");
         }
         onProgressionUpdatedInternal(0, 100, 0.0f, 1.0f, i18n::CClientStrings::ExportDataPrepare);
         return tempFolder;
      }

      void CExportData::collectDataTo(const boost::filesystem::path& tempFolder)
      {
         std::ofstream outfile((tempFolder / "exportData.csv").string());

         // Header line
         outfile << "keyword,date,value";

         // Data lines
         static constexpr auto TotalPercentToExportAcquisition = 70.0f;
         const auto keywords = m_keywordRequester->getKeyword(m_keywordId);
         if (keywords->HistoryDepth() == shared::plugin::yPluginApi::EHistoryDepth::kNoHistory)
         {
            outfile << std::endl << m_keywordId << "," << keywords->LastAcquisitionDate() << "," << keywords->LastAcquisitionValue();

            onProgressionUpdatedInternal(0, 100, 0.0f, TotalPercentToExportAcquisition,
                                         i18n::CClientStrings::ExportDataCreateFile);
         }
         else
         {
            auto nbLinesDone = 0;
            m_acquisitionRequester->exportAcquisitions(m_keywordId,
                                                       [this, &outfile, &nbLinesDone](const boost::posix_time::ptime& date,
                                                                                      const std::string& value,
                                                                                      const int nbTotalLines)
                                                       {
                                                          outfile << std::endl << m_keywordId << "," << date << "," << value;

                                                          // Progress from 0 to 70 for this step
                                                          ++nbLinesDone;
                                                          onProgressionUpdatedInternal(nbTotalLines - nbLinesDone,
                                                                                       nbTotalLines,
                                                                                       0.0f,
                                                                                       TotalPercentToExportAcquisition,
                                                                                       i18n::CClientStrings::ExportDataCreateFile);
                                                       });
         }
      }

      boost::filesystem::path CExportData::makeZipArchive(const boost::filesystem::path& tempFolder)
      {
         // Create if needed the backup folder
         if (!boost::filesystem::exists(m_pathProvider->backupPath()))
            boost::filesystem::create_directory(m_pathProvider->backupPath());
         else
            YADOMS_LOG(debug) << "Folder " << m_pathProvider->backupPath().string() << " already exists. Do not create it";

         //zip folder content (51 -> 98)
         auto zipFilenameFinal = m_pathProvider->backupPath() / "exportData.zip";
         const auto zipFilename = m_pathProvider->backupPath() / "exportData.zip.inprogress";

         boost::filesystem::remove(zipFilenameFinal);
         boost::filesystem::remove(zipFilename);

         try
         {
            //count files
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
            onProgressionUpdatedInternal(0, 100, 60.0f, 98.0f, i18n::CClientStrings::ExportDataCompress);
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

      void CExportData::onZipEDone(const void* pSender,
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
                                         i18n::CClientStrings::ExportDataCompress);
            YADOMS_LOG(debug) << " ZIP : " << hdr.getFileName();
         }
      }

      void CExportData::cleanup(const boost::filesystem::path& tempFolder)
      {
         onProgressionUpdatedInternal(0,
                                      100,
                                      98.0f,
                                      99.0f,
                                      i18n::CClientStrings::ExportDataClean);

         boost::filesystem::remove_all(tempFolder);
         boost::filesystem::remove(tempFolder);
      }
   } //namespace exportData
} //namespace task
