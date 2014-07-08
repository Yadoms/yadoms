#include "stdafx.h"
#include "ITask.h"
#include "Instance.h"
#include <shared/Log.h>
#include "database/entities/Entities.h"

namespace task {

   //------------------------------
   ///\brief Constructor
   //------------------------------
   CInstance::CInstance(boost::shared_ptr<ITask> task, shared::event::CEventHandler & eventHandler, const int systemEventCode)
      :CThreadBase("Task " + task->getName()), m_currentProgression(0), m_pTask(task), m_eventHandler(eventHandler), m_systemEventCode(systemEventCode)
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
         //log event started
         database::entities::CEventLogger entry;
         entry.Code = database::entities::ESystemEventCode::kStarted;
         entry.Who = m_pTask->getName();
         m_eventHandler.postEvent(m_systemEventCode, entry);

         // Execute plugin code
         m_pTask->doWork(boost::bind(&CInstance::OnTaskProgressUpdated, this, _1));

         //log event stopped
         entry.Code = database::entities::ESystemEventCode::kStopped;
         m_eventHandler.postEvent(m_systemEventCode, entry);

      }
      catch (boost::thread_interrupted&)
      {
         // End-of-thread exception was not catch by plugin,
         // it's a developer's error, we have to report it
         YADOMS_LOG(error) << m_pTask->getName() << " didn't catch boost::thread_interrupted.";

         database::entities::CEventLogger entry;
         entry.Code = database::entities::ESystemEventCode::kTaskFailed;
         entry.Who = m_pTask->getName();
         entry.What = "didn't catch boost::thread_interrupted.";
         m_eventHandler.postEvent(m_systemEventCode, entry);
      }
      catch (std::exception& e)
      {
         // Plugin crashed
         YADOMS_LOG(error) << m_pTask->getName() << " crashed in doWork with exception : " << e.what();

         database::entities::CEventLogger entry;
         entry.Code = database::entities::ESystemEventCode::kTaskFailed;
         entry.Who = m_pTask->getName();
         entry.What =  (boost::format("Crashed in doWork with exception %1%") % e.what()).str();
         m_eventHandler.postEvent(m_systemEventCode, entry);

      }
      catch (...)
      {
         // Plugin crashed
         YADOMS_LOG(error) << m_pTask->getName() << " crashed in doWork with unknown exception.";

         database::entities::CEventLogger entry;
         entry.Code = database::entities::ESystemEventCode::kTaskFailed;
         entry.Who = m_pTask->getName();
         entry.What =  "Crashed in doWork with unknown exception";
         m_eventHandler.postEvent(m_systemEventCode, entry);
      }
   }
   
} //namespace task