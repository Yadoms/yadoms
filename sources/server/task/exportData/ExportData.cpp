#include "stdafx.h"
#include "task/ITask.h"
#include "ExportData.h"
#include <Poco/Zip/Compress.h>
#include <Poco/Zip/ZipException.h>
#include <Poco/Delegate.h>
#include "i18n/ClientStrings.h"
#include "shared/Log.h"
#include "database/IAcquisitionRequester.h"

namespace task
{
   namespace exportData
   {
      std::string CExportData::m_taskName = "exportData";

      CExportData::CExportData(boost::shared_ptr<const IPathProvider> pathProvider,
                               boost::shared_ptr<database::IKeywordRequester> keywordRequester,
                               boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                               int keywordId)
         : m_pathProvider(pathProvider),
           m_keywordRequester(keywordRequester),
           m_acquisitionRequester(acquisitionRequester),
           m_keywordId(keywordId)
      {
      }

      const std::string& CExportData::getName() const
      {
         return m_taskName;
      }

      void CExportData::onProgressionUpdatedInternal(float progression,
                                                     const std::string& message) const
      {
         if (m_reportRealProgress)
            m_reportRealProgress(true, progression, message, std::string(), shared::CDataContainer::make());
      }

      void CExportData::doWork(TaskProgressFunc functor)
      {
         m_reportRealProgress = functor;

         boost::filesystem::path exportDataTempFolder;

         try
         {
            exportDataTempFolder = prepare();
            collectDataTo(exportDataTempFolder);
            auto zipFile = makeZipArchive(exportDataTempFolder);
            cleanup(exportDataTempFolder);
            onProgressionUpdatedInternal(100.0f, i18n::CClientStrings::ExportDataSuccess);
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Fail to export data. Abort...";
            if (!exportDataTempFolder.empty())
               cleanup(exportDataTempFolder);
            throw shared::exception::CException(ex.what());
         }
      }

      boost::filesystem::path CExportData::prepare() const
      {
         //create "export data temp" folder
         auto exportDataTempFolder = boost::filesystem::temp_directory_path() / "exportData";

         //if folder exist, cleanup, else create if
         if (boost::filesystem::exists(exportDataTempFolder))
         {
            YADOMS_LOG(debug) << "Deleting " << exportDataTempFolder << " and its content";
            boost::filesystem::remove_all(exportDataTempFolder);
            boost::filesystem::remove(exportDataTempFolder);
         }
         else
         {
            YADOMS_LOG(debug) << "Folder " << exportDataTempFolder << " does not exist";
         }

         YADOMS_LOG(debug) << "Create " << exportDataTempFolder;
         if (!boost::filesystem::create_directory(exportDataTempFolder))
         {
            YADOMS_LOG(error) << "Fail to create folder " << exportDataTempFolder;
            throw shared::exception::CException("fail to create export data folder");
         }
         onProgressionUpdatedInternal(1.0f, i18n::CClientStrings::ExportDataPrepare);
         return exportDataTempFolder;
      }

      void CExportData::collectDataTo(const boost::filesystem::path& exportDataTempFolder) const
      {
         std::ofstream outfile((exportDataTempFolder / "exportData.csv").string());
         auto nbLinesDone = 0;

         // Header line
         outfile << "keyword,date,value";

         // Data lines
         static const auto TotalPercentToExportAcquisition = 70.0;
         const auto keywords = m_keywordRequester->getKeyword(m_keywordId);
         if (keywords->HistoryDepth() == shared::plugin::yPluginApi::EHistoryDepth::kNoHistory)
         {
            outfile << std::endl << m_keywordId << "," << keywords->LastAcquisitionDate() << "," << keywords->LastAcquisitionValue();

            onProgressionUpdatedInternal(TotalPercentToExportAcquisition,
                                         i18n::CClientStrings::ExportDataCreateFile);
         }
         else
         {
            m_acquisitionRequester->exportAcquisitions(m_keywordId,
                                                       [this, &outfile, &nbLinesDone](const boost::posix_time::ptime& date,
                                                                                      const std::string& value,
                                                                                      const int nbTotalLines)
                                                       {
                                                          outfile << std::endl << m_keywordId << "," << date << "," << value;

                                                          // Progress from 0 to 70 for this step
                                                          ++nbLinesDone;
                                                          const auto progress = nbLinesDone * TotalPercentToExportAcquisition / nbTotalLines;
                                                          static auto lastProgressSent = -1.0;
                                                          if (progress != lastProgressSent)
                                                          {
                                                             lastProgressSent = progress;
                                                             onProgressionUpdatedInternal(static_cast<float>(progress),
                                                                                          i18n::CClientStrings::ExportDataCreateFile);
                                                          }
                                                       });
         }
      }

      boost::filesystem::path CExportData::makeZipArchive(boost::filesystem::path& exportDataTempFolder)
      {
         // Create if needed the backup folder
         if (!boost::filesystem::exists(m_pathProvider->backupPath()))
         {
            boost::filesystem::create_directory(m_pathProvider->backupPath());
         }
         else
         {
            YADOMS_LOG(debug) << "Folder " << m_pathProvider->backupPath().string() << " already exists. Do not create it";
         }

         //zip folder content (51 -> 98)
         auto zipFilenameFinal = m_pathProvider->backupPath() / "exportData.zip";
         const auto zipFilename = m_pathProvider->backupPath() / "exportData.zip.inprogress";

         boost::filesystem::remove(zipFilenameFinal);
         boost::filesystem::remove(zipFilename);

         try
         {
            std::ofstream out(zipFilename.string(), std::ios::binary);
            Poco::Zip::Compress c(out, true);
            c.EDone += Poco::Delegate<CExportData, const Poco::Zip::ZipLocalFileHeader>(this, &CExportData::onZipEDone);
            c.addRecursive(exportDataTempFolder.string());
            c.EDone -= Poco::Delegate<CExportData, const Poco::Zip::ZipLocalFileHeader>(this, &CExportData::onZipEDone);
            c.close(); // MUST be done to finalize the Zip file
            onProgressionUpdatedInternal(98.0f, i18n::CClientStrings::ExportDataCompress);
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
      void CExportData::onZipEDone(const void* pSender,
                                   const Poco::Zip::ZipLocalFileHeader& hdr)
      {
         if (hdr.isFile())
         {
            onProgressionUpdatedInternal(80.0f, i18n::CClientStrings::ExportDataCompress);
            YADOMS_LOG(debug) << " ZIP : " << hdr.getFileName();
         }
      }

      void CExportData::cleanup(boost::filesystem::path& exportDataTempFolder) const
      {
         onProgressionUpdatedInternal(98.0f, i18n::CClientStrings::ExportDataClean);
         boost::filesystem::remove_all(exportDataTempFolder);
      }
   } //namespace exportData
} //namespace task
