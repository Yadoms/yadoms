#pragma once

#include "IRestService.h"
#include "task/Scheduler.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CTask final : public IRestService
         {
         public:
            explicit CTask(boost::shared_ptr<task::CScheduler> taskManager);
            ~CTask() override = default;

            // IRestService implementation
            void configurePocoDispatcher(CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();
            boost::shared_ptr<shared::serialization::IDataSerializable> getOneTask(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllTasks(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent) const;

         private:
            boost::shared_ptr<task::CScheduler> m_taskManager;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
            static std::string m_restKeyword;

            boost::shared_ptr<shared::CDataContainer> serialize(const boost::shared_ptr<const task::IInstance>& instance) const;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
