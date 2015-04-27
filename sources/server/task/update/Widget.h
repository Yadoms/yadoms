#pragma once
#include "task/ITask.h"
#include "update/source/Widget.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update a plugin
   //-----------------------------------------
   class CWidget : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] updateSource   The update source
      ///\param [in] onlyCheckForUpdate   If true will perform a check for update; if false will perform a complete update
      //------------------------------------------
      CWidget(boost::shared_ptr<::update::source::CWidget> updateSource, bool onlyCheckForUpdate); // "::update" is needed to avoid confusion with task::update namespace

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CWidget();

   public:
      // ITask implementation
      virtual const std::string & getName();
      virtual bool doWork(TaskProgressFunc pFunctor);
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
      boost::shared_ptr<::update::source::CWidget> m_updateSource;

      //------------------------------------------
      ///\brief   Indicate if check for update is required (true) or a full update (false)
      //------------------------------------------
      bool m_onlyCheckForUpdate;
   };

} //namespace update
} //namespace task
