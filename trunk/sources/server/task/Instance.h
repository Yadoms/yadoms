#pragma once

#include <shared/ThreadBase.h>
#include "ITask.h"
#include <shared/event/EventHandler.hpp>
#include "IInstance.h"

namespace task {

   //------------------------------
   ///\brief Class which handle one task
   //------------------------------
   class CInstance : public shared::CThreadBase, public IInstance
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
      ///\brief Get the unique identifier of the instance
      //---------------------------------
      virtual std::string getGuid() const;

      //---------------------------------
      ///\brief Get the current task
      //---------------------------------
      boost::shared_ptr<ITask> getTask();

      //---------------------------------
      ///\brief Get the current progression
      //---------------------------------
      virtual boost::optional<float> getProgression() const;

      //---------------------------------
      ///\brief Get the current message
      //---------------------------------
      virtual std::string getMessage() const;

      //------------------------------------------
      ///\brief   Obtain Status
      //------------------------------------------
      virtual ITask::EStatus getStatus() const;

      //---------------------------------
      ///\brief Get the current name of the task
      //---------------------------------
      virtual std::string getName() const;

      //---------------------------------
      ///\brief Get the creation date of the task
      //---------------------------------
      virtual boost::posix_time::ptime getCreationDate() const;

   private:
      //---------------------------------
      ///\brief Static method which reports progress
      ///\param [in] progression : the actual progression or NULL if not supported by the task
      ///\param [in] message : the actual message provided by the task
      //---------------------------------
      void OnTaskProgressUpdated(boost::optional<float> progression, std::string message);

      //--------------------------------------------------------------
      /// \brief			The current task instance progression
      //--------------------------------------------------------------
      boost::optional<float> m_currentProgression;

      //--------------------------------------------------------------
      /// \brief			The current message given by the task
      //--------------------------------------------------------------
      std::string m_currentMessage;

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
      ITask::EStatus m_currentStatus;

      //------------------------------------------
      ///\brief   The creation date
      //------------------------------------------
      boost::posix_time::ptime m_creationDate;

   };

} //namespace task
