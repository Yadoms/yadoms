#pragma once

#include <shared/ThreadBase.h>
#include "ITask.h"
#include <shared/event/EventHandler.hpp>

namespace task {

   //------------------------------
   ///\brief Class which handle one task
   //------------------------------
   class CInstance : public shared::CThreadBase
   {
   public:
      //------------------------------
      ///\brief Constructor
      //------------------------------
      CInstance(boost::shared_ptr<ITask> task, shared::event::CEventHandler & eventHandler, const int systemEventCode);
      
      //------------------------------
      ///\brief public destructor
      //------------------------------
      virtual ~CInstance();

   private:
      //---------------------------------
      ///\brief Static method which reports progress
      ///\param [in] progression : the actual progression
      //---------------------------------
      void OnTaskProgressUpdated(float progression);

      //--------------------------------------------------------------
      /// \brief			The current task instance progression
      //--------------------------------------------------------------
      float m_currentProgression;

   protected:
      //--------------------------------------------------------------
      /// \brief			The main plugin work
      //--------------------------------------------------------------
      virtual void doWork();
      
   private:
      //--------------------------------------------------------------
      /// \brief			The plugin used for this instance
      //--------------------------------------------------------------
      boost::shared_ptr<ITask> m_pTask;

      //------------------------------------------
      ///\brief   A reference to the main event handler (to report start and stop status)
      //------------------------------------------
      shared::event::CEventHandler & m_eventHandler;

      //------------------------------------------
      ///\brief   The event identifier for event handler
      //------------------------------------------
      int m_systemEventCode;

   };

} //namespace task
