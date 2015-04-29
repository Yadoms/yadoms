#pragma once
#include "task/ITask.h"
#include "task/IUnique.h"
#include <Poco/Zip/ZipLocalFileHeader.h>
#include "IRunningInformation.h"
#include <shared/DataContainer.h>
#include <Poco/URI.h>
#include "update/source/Yadoms.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update a plugin
   //-----------------------------------------
   class CYadoms : public ITask, public IUnique
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] updateSource   The update source
      ///\param [in] onlyCheckForUpdate   If true will perform a check for update; if false will perform a complete update
      //------------------------------------------
      CYadoms(boost::shared_ptr<::update::source::CYadoms> updateSource, bool onlyCheckForUpdate); // "::update" is needed to avoid confusion with task::update namespace

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CYadoms();

   public:
      // ITask implementation
      virtual const std::string & getName();
      bool doWork(TaskProgressFunc pFunctor);
      // ITask implementation

     
   private:
      //------------------------------------------
      ///\brief   The task name
      //------------------------------------------
      static std::string m_taskName;

      //------------------------------------------
      ///\brief   The update source
      /// "::update" is needed to avoid confusion with task::update namespace
      //------------------------------------------
      boost::shared_ptr<::update::source::CYadoms> m_updateSource;

      //------------------------------------------
      ///\brief   Indicate if check for update is required (true) or a full update (false)
      //------------------------------------------
      bool m_onlyCheckForUpdate;
   };

} //namespace update
} //namespace task
