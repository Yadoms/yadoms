#pragma once
#include "Instance.h"
#include "FinishedInstance.h"
#include "ITask.h"
#include "dataAccessLayer/IEventLogger.h"

namespace task
{
   //------------------------------
   ///\brief Class which handle tasks (create, run,...)
   //------------------------------
   class CScheduler : public shared::CThreadBase
   {
   public:
      //------------------------------
      ///\brief Constructor
      //------------------------------
      explicit CScheduler(boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger);
      
      //------------------------------
      ///\brief public destructor
      //------------------------------
      virtual ~CScheduler();
      
      //--------------------------------------------------------------
      ///\brief	   Run a task
      ///\param [in] taskToRun : the task to run
      ///\param [in] uniqueId : the task id 
      ///\return     true if the task has been created and false if the task has not be created. The uniqueId has the id of the previous task if the expected task was unique
      //--------------------------------------------------------------
      bool runTask(boost::shared_ptr<ITask> taskToRun, std::string & uniqueId);

      //--------------------------------------------------------------
      /// \brief			Ask a task by its guid
      ///\param [in] uniqueId : the task id 
      /// \return    	A Shared ptr to the task or null of the task does not exist
      //--------------------------------------------------------------
      boost::shared_ptr<IInstance> getTask(std::string uniqueId);

      //--------------------------------------------------------------
      /// \brief			Ask for the list of all tasks
      /// \return    	A List of all tasks
      //--------------------------------------------------------------
      std::vector< boost::shared_ptr< IInstance > > getAllTasks();

   protected:
      //--------------------------------------------------------------
      /// \brief			The main scheduler work
      //--------------------------------------------------------------
      void doWork() override;

   private:
      //--------------------------------------------------------------
      /// \brief	Event IDs
      //--------------------------------------------------------------
      enum
      {
         kTaskEvent = shared::event::kUserFirstId,  
         kRunTask,
      };
      
      //--------------------------------------------------------------
      /// \brief			Map of all running tasks (key are task uuid as string)
      //--------------------------------------------------------------
      typedef std::map< std::string, boost::shared_ptr<CInstance> > RunningTaskInstanceMap;
      RunningTaskInstanceMap m_runningTasks;

      //--------------------------------------------------------------
      /// \brief			Map of all finished tasks (key are task uuid as string)
      //--------------------------------------------------------------
      typedef std::map< std::string, boost::shared_ptr<CFinishedInstance> > FinishedTaskInstanceMap;
      FinishedTaskInstanceMap m_finishedTasks;

      //------------------------------------------
      ///\brief   A reference to the main event logger (to report start and stop status)
      //------------------------------------------
      boost::shared_ptr<dataAccessLayer::IEventLogger> m_eventLogger;
      
      //------------------------------------------
      ///\brief   A reference to the local task event handler (to report start and stop status of a task)
      //------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_taskEventHandler;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the maps
      //--------------------------------------------------------------
      mutable boost::mutex m_mapsMutex;
   };

} //namespace task
