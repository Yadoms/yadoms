#pragma once
#include "task/ITask.h"
#include "IPathProvider.h"
#include <Poco/Zip/ZipLocalFileHeader.h>
#include "database/IAcquisitionRequester.h"
#include "database/IKeywordRequester.h"

namespace task
{
   namespace exportAcquisitions
   {
      //------------------------------------------
      ///\brief   Export acquisitions task
      //-----------------------------------------
      class CExportAcquisitions final : public ITask
      {
      public:
         explicit CExportAcquisitions(boost::shared_ptr<const IPathProvider> pathProvider,
                              boost::shared_ptr<database::IKeywordRequester> keywordRequester,
                              boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                              int keywordId);
         ~CExportAcquisitions() override = default;

         // ITask implementation
         const std::string& getName() const override;
         void onSetTaskId(const std::string& taskId) override;
         void doWork(TaskProgressFunc functor) override;
         bool isCancellable() const override;
         // [END] ITask implementation

      private:
         void doWork(int currentTry = 0);
         bool checkEnoughSpace(const boost::filesystem::path& where) const;
         boost::filesystem::path prepare();
         void collectDataTo(const boost::filesystem::path& tempFolder);
         boost::filesystem::path makeZipArchive(const boost::filesystem::path& tempFolder);
         void cleanup(const boost::filesystem::path& tempFolder);

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

         //------------------------------------------
         ///\brief   The input data for the task
         //------------------------------------------
         boost::shared_ptr<const IPathProvider> m_pathProvider;
         boost::shared_ptr<database::IKeywordRequester> m_keywordRequester;
         boost::shared_ptr<database::IAcquisitionRequester> m_acquisitionRequester;
         int m_keywordId;

         //------------------------------------------
         ///\brief   The task name
         //------------------------------------------
         static const std::string TaskName;

         //------------------------------------------
         ///\brief   The function pointer for reporting progression
         //------------------------------------------
         TaskProgressFunc m_reportRealProgress;

         //------------------------------------------
         ///\brief   The file count to zip
         //------------------------------------------
         int m_fileCountToZip;

         //------------------------------------------
         ///\brief   The current count of zipped file
         //------------------------------------------
         int m_currentFileCount;

         float m_lastProgressSent;
      };
   } //namespace exportAcquisitions
} //namespace task
