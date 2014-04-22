#pragma once
#include "Instance.h"
#include "ITask.h"
#include <shared/event/EventHandler.hpp>

namespace task {

   //------------------------------
   ///\brief Class which handle tasks (create, run,...)
   //------------------------------
   class CScheduler
   {
   public:
      //------------------------------
      ///\brief Constructor
      //------------------------------
      CScheduler(shared::event::CEventHandler & eventHandler, const int systemEventCode);
      
      //------------------------------
      ///\brief public destructor
      //------------------------------
      virtual ~CScheduler();
      
      //--------------------------------------------------------------
      ///\brief	   Run a task
      ///\param [in] taskToRun : the task to run
      ///\return     the task id
      //--------------------------------------------------------------
      std::string RunTask(boost::shared_ptr<ITask> taskToRun);
      
   private:
      //--------------------------------------------------------------
      /// \brief			Map of all running tasks (key are task uuid as string)
      //--------------------------------------------------------------
      typedef std::map< std::string, boost::shared_ptr<CInstance> > TaskInstanceMap;
      TaskInstanceMap m_runningTasks;

      //------------------------------------------
      ///\brief   A reference to the main event handler (to report start and stop status)
      //------------------------------------------
      shared::event::CEventHandler & m_eventHandler;
      //------------------------------------------
      ///\brief   The event code used to notify
      //------------------------------------------
      int m_systemEventCode;
   };

} //namespace task
