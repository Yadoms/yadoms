#pragma once
#include "task/ITask.h"
#include "IPathProvider.h"
#include <Poco/Zip/ZipLocalFileHeader.h>
#include "database/IAcquisitionRequester.h"

namespace task
{
   namespace exportData
   {
      //------------------------------------------
      ///\brief   Export data task
      //-----------------------------------------
      class CExportData : public ITask
      {
      public:
         explicit CExportData(boost::shared_ptr<const IPathProvider> pathProvider,
                              boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                              int keywordId);
         virtual ~CExportData() = default;

         // ITask implementation
         const std::string& getName() const override;
         void doWork(TaskProgressFunc functor) override;
         // [END] ITask implementation

      private:
         boost::filesystem::path prepare() const;
         void collectDataTo(const boost::filesystem::path& logsTempFolder) const;
         boost::filesystem::path makeZipArchive(boost::filesystem::path& logsTempFolder);
         void cleanup(boost::filesystem::path& logsTempFolder) const;

         //------------------------------------------
         ///\brief   Internal progress handler 
         ///\param [in] progression The operation progression percentage
         ///\param [in] message     Message to display
         //------------------------------------------
         void onProgressionUpdatedInternal(float progression,
                                           const std::string& message = std::string()) const;

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
         boost::shared_ptr<database::IAcquisitionRequester> m_acquisitionRequester;
         int m_keywordId;

         //------------------------------------------
         ///\brief   The task name
         //------------------------------------------
         static std::string m_taskName;

         //------------------------------------------
         ///\brief   The function pointer for reporting progression
         //------------------------------------------
         TaskProgressFunc m_reportRealProgress;
      };
   } //namespace exportData
} //namespace task
