#pragma once

#include <shared/ThreadBase.h>
#include "ITask.h"
#include <shared/event/EventHandler.hpp>
#include "IInstance.h"
#include "TaskStatus.h"

namespace task {

   //------------------------------
   ///\brief Class which handle one task
   //------------------------------
   class CInstance : public shared::CThreadBase, public IInstance, public boost::enable_shared_from_this<CInstance>
   {
   public:
      //------------------------------
      ///\brief Constructor
      //------------------------------
      CInstance(boost::shared_ptr<ITask> task, boost::shared_ptr<shared::event::CEventHandler> eventHandler, const int eventCode, const std::string & guid);
      
      //------------------------------
      ///\brief public destructor
      //------------------------------
      virtual ~CInstance();

      //---------------------------------
      ///\brief Get the current task
      //---------------------------------
      boost::shared_ptr<ITask> getTask();

      // IInstance implementation
      virtual std::string getGuid() const;
      virtual boost::optional<float> getProgression() const;
      virtual std::string getMessage() const;
      virtual std::string getExceptionMessage() const;
      virtual ETaskStatus getStatus() const;
      virtual shared::CDataContainer getTaskData() const;
      virtual std::string getName() const;
      virtual boost::posix_time::ptime getCreationDate() const;
      // [END] - IInstance implementation

   private:
      //---------------------------------
      ///\brief Static method which reports progress
      ///\param [in] isRunning : the actual task is still running (false in case of task error)
      ///\param [in] progression : the actual progression or NULL(boost::optional) if not supported by the task 
      ///\param [in] message : the actual message provided by the task (i18n string)
      ///\param [in] exception : the internal error message (not i18n)
      ///\param [in] taskData : some free data provided by task implementation
      //---------------------------------
      void OnTaskProgressUpdated(bool isRunning, boost::optional<float> progression, std::string message, std::string exception, shared::CDataContainer taskData);



      //--------------------------------------------------------------
      /// \brief			The current exception given by the task
      //--------------------------------------------------------------
      bool m_currentIsRunning;

      //--------------------------------------------------------------
      /// \brief			The current task instance progression
      //--------------------------------------------------------------
      boost::optional<float> m_currentProgression;

      //--------------------------------------------------------------
      /// \brief			The current message given by the task
      //--------------------------------------------------------------
      std::string m_currentMessage;

      //--------------------------------------------------------------
      /// \brief			The current exception given by the task
      //--------------------------------------------------------------
      std::string m_currentException;

      //--------------------------------------------------------------
      /// \brief			The current task custom data
      //--------------------------------------------------------------
      shared::CDataContainer m_taskData;
   protected:
      //--------------------------------------------------------------
      /// \brief			The main instance work
      //--------------------------------------------------------------
      virtual void doWork();
      
   private:
      //--------------------------------------------------------------
      /// \brief			The plugin used for this instance
      //--------------------------------------------------------------
      boost::shared_ptr<ITask> m_task;

      //------------------------------------------
      ///\brief   A reference to the main event handler (to report start and stop status)
      //------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_eventHandler;

      //------------------------------------------
      ///\brief   The event identifier for event handler
      //------------------------------------------
      int m_eventCode;

      //------------------------------------------
      ///\brief   The unique identifier of the task
      //------------------------------------------
      std::string m_guid;

      //------------------------------------------
      ///\brief   The status of the task
      //------------------------------------------
      ETaskStatus m_currentStatus;

      //------------------------------------------
      ///\brief   The creation date
      //------------------------------------------
      boost::posix_time::ptime m_creationDate;
   };

} //namespace task
