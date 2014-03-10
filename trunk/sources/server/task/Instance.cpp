#include "stdafx.h"
#include "ITask.h"
#include "Instance.h"
#include <shared/Log.h>

namespace task {

   //------------------------------
   ///\brief Constructor
   //------------------------------
   CInstance::CInstance(boost::shared_ptr<ITask> task)
      :CThreadBase("Task " + task->getName()), m_pTask(task)
   {
      BOOST_ASSERT(m_pTask);
      start();
   }

   //------------------------------
   ///\brief public destructor
   //------------------------------
   CInstance::~CInstance()
   {
      stop();
   }
   
      
   void CInstance::OnTaskProgressUpdated(float progression)
   {
      m_currentProgression = progression;
      YADOMS_LOG(info) << m_pTask->getName() << " report progression " << m_currentProgression;
   }

   void CInstance::doWork()
   {
      BOOST_ASSERT(m_pTask);
      YADOMS_LOG_CONFIGURE(m_pTask->getName());

      try
      {
         // Execute plugin code
         m_pTask->doWork(boost::bind(&CInstance::OnTaskProgressUpdated, this, _1));
      }
      catch (boost::thread_interrupted&)
      {
         // End-of-thread exception was not catch by plugin,
         // it's a developer's error, we have to report it
         YADOMS_LOG(error) << m_pTask->getName() << " didn't catch boost::thread_interrupted.";
      }
      catch (std::exception& e)
      {
         // Plugin crashed
         YADOMS_LOG(error) << m_pTask->getName() << " crashed in doWork with exception : " << e.what();
      }
      catch (...)
      {
         // Plugin crashed
         YADOMS_LOG(error) << m_pTask->getName() << " crashed in doWork with unknown exception.";
      }
   }
   
} //namespace task