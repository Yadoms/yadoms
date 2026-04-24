#pragma once
#include <Poco/Zip/ZipLocalFileHeader.h>
#include "IExportDataHandler.h"
#include "IPathProvider.h"
#include "database/IKeywordRequester.h"
#include "task/ITask.h"

namespace task
{
   namespace exportData
   {
      //------------------------------------------
      ///\brief   Export data task
      //-----------------------------------------
      class CExportData final : public ITask
      {
      public:
         explicit CExportData(boost::shared_ptr<const IPathProvider> pathProvider,
                              std::unique_ptr<IExportDataHandler> exportDataHandler);
         ~CExportData() override = default;

         // ITask implementation
         std::string getName() override;
         void onSetTaskId(const std::string& taskId) override;
         void doWork(TaskProgressFunc reportProgressFct) override;
         bool isCancellable() const override;
         // [END] ITask implementation

      private:
         void doWork(int currentTry = 0);
         boost::filesystem::path prepare() const;
         boost::filesystem::path makeZipArchive(const boost::filesystem::path& tempFolder);
         void cleanup(const boost::filesystem::path& tempFolder) const;

         //------------------------------------------
         ///\brief   Internal progress handler 
         ///\param [in] remaining   The remaining count (between 0 and total) for current operation
         ///\param [in] total       The total count for current operation
         ///\param [in] currentPart The current total progression
         ///\param [in] totalPart   The final progression when current operation ends
         ///\param [in] message     Optional message to display
         //------------------------------------------
         void onProgressionUpdatedInternal(int remaining,
                                           int total,
                                           float currentPart,
                                           float totalPart,
                                           const std::string& message = std::string());

         //------------------------------------------
         ///\brief   Report event
         ///\param [in] pSender     The sender
         ///\param [in] hdr         The event (Zip local file)
         //------------------------------------------
         void onZipEDone(const void* pSender,
                         const Poco::Zip::ZipLocalFileHeader& hdr);
         
         boost::shared_ptr<const IPathProvider> m_pathProvider;
         std::unique_ptr<IExportDataHandler> m_exportDataHandler;

         //------------------------------------------
         ///\brief   Manage progress report
         //------------------------------------------
         TaskProgressFunc m_reportProgressFct;
         int m_fileCountToZip;
         int m_currentFileCount;
         float m_lastProgressSent;
      };
   } //namespace exportData
} //namespace task
