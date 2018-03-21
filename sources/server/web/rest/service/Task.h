#pragma once

#include "IRestService.h"
#include "task/Scheduler.h"

namespace web { namespace rest { namespace service {

   class CTask : public IRestService
   {
   public:
      explicit CTask(boost::shared_ptr<task::CScheduler> taskManager);
      virtual ~CTask();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      static const std::string & getRestKeyword();
      boost::shared_ptr<shared::serialization::IDataSerializable> getOneTask(const std::vector<std::string> & parameters, const std::string & requestContent);
      boost::shared_ptr<shared::serialization::IDataSerializable> getAllTasks(const std::vector<std::string> & parameters, const std::string & requestContent);
      
      boost::shared_ptr<shared::serialization::IDataSerializable> addTask(const std::vector<std::string> & parameters, const std::string & requestContent);

   private:
      boost::shared_ptr<task::CScheduler> m_taskManager;
      static std::string m_restKeyword;

      shared::CDataContainer serialize(boost::shared_ptr<task::IInstance> instance) const;
   };


} //namespace service
} //namespace rest
} //namespace web 
