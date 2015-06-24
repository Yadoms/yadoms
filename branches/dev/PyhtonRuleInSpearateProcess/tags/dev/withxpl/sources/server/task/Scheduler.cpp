#include "stdafx.h"
#include "ITask.h"
#include "Scheduler.h"
#include "Instance.h"



namespace task {

   CScheduler::CScheduler(shared::event::CEventHandler & eventHandler, const int systemEventCode)
      :m_eventHandler(eventHandler), m_systemEventCode(systemEventCode)
   {
   }


   CScheduler::~CScheduler()
   {
   }
   
   std::string CScheduler::RunTask(boost::shared_ptr<ITask> taskToRun)
   {
      BOOST_ASSERT(taskToRun.get() != NULL);

      boost::uuids::uuid u =boost::uuids::random_generator()();
      std::string uniqueId = boost::uuids::to_string(u);
      m_runningTasks.insert(std::make_pair(uniqueId, boost::shared_ptr<CInstance>(new CInstance(taskToRun, m_eventHandler, m_systemEventCode))));
      return uniqueId;
   }
   
} //namespace task
