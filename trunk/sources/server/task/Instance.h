#pragma once

#include <shared/ThreadBase.h>
#include "ITask.h"

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
      CInstance(boost::shared_ptr<ITask> task);
      
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


   };

} //namespace task
