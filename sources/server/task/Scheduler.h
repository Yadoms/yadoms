#pragma once
#include "Instance.h"
#include "ITask.h"

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
      CScheduler();
      
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
   };

} //namespace task
