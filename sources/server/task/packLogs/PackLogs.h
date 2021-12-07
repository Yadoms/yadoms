#pragma once
#include "task/ITask.h"
#include "IPathProvider.h"
#include <Poco/Zip/ZipLocalFileHeader.h>

namespace task
{
   namespace packLogs
   {
      //------------------------------------------
      ///\brief   Logs packing task
      //-----------------------------------------
      class CPackLogs final : public ITask
      {
      public:
         explicit CPackLogs(boost::shared_ptr<const IPathProvider> pathProvider);
         ~CPackLogs() override = default;

         // ITask implementation
         const std::string& getName() const override;
         void doWork(TaskProgressFunc functor) override;
         void onSetTaskId(const std::string& taskId) override;
         bool isCancellable() const override;
         // [END] ITask implementation

      private:
         void doWork(int currentTry = 0);
         boost::filesystem::path prepare() const;
         bool copyLogsFiles(const boost::filesystem::path& logsTempFolder) const;
         boost::filesystem::path makeZipArchive(const boost::filesystem::path& logsTempFolder);
         void cleanup(const boost::filesystem::path& logsTempFolder) const;

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
         ///\brief   The path provider instance
         //------------------------------------------
         boost::shared_ptr<const IPathProvider> m_pathProvider;

         //------------------------------------------
         ///\brief   The task name
         //------------------------------------------
         static const std::string TaskName;

         //------------------------------------------
         ///\brief   The function pointer for reporting progression
         //------------------------------------------
         TaskProgressFunc m_reportRealProgress;
      };
   } //namespace packLogs
} //namespace task
