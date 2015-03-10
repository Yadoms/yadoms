#pragma once
#include "task/ITask.h"
#include "task/IUnique.h"
#include <Poco/Zip/ZipLocalFileHeader.h>

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update a plugin
   //-----------------------------------------
   class CYadoms : public ITask, public IUnique
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      //------------------------------------------
      CYadoms();

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CYadoms();

   public:
      // ITask implementation
      virtual const std::string & getName();
      bool doWork(TaskProgressFunc pFunctor);
      // ITask implementation

      void onDownloadReportProgress(const std::string & filename, float progression);

   private:
      //------------------------------------------
      ///\brief   The task name
      //------------------------------------------
      static std::string m_taskName;

      //------------------------------------------
      ///\brief   A method for receivving unzip errors
      //------------------------------------------
      void onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);
      
      //------------------------------------------
      ///\brief   Indicate if an error occured during package extraction
      //------------------------------------------
      bool m_unzipError;

      //------------------------------------------
      ///\brief   Indicate the error message if ab error occured during package extraction
      //------------------------------------------
      std::string m_unzipErrorMessage;
   };

} //namespace update
} //namespace task
