#pragma once
#include "IRestService.h"
#include "update/IUpdateManager.h"
#include "task/Scheduler.h"
#include "ITaskInProgressHandler.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         //-----------------------------------------------------------------------------
         /// \brief  REST handler class for update and check for update
         //-----------------------------------------------------------------------------      
         class CUpdate final : public IRestService
         {
         public:
            //-----------------------------------------------------------------------------
            /// \brief  Constructor
            /// \param [in] updateManager   The update manager
            /// \param [in] taskScheduler   The task scheduler
            //-----------------------------------------------------------------------------      
            explicit CUpdate(boost::shared_ptr<update::IUpdateManager> updateManager,
                             const boost::shared_ptr<task::CScheduler>& taskScheduler);
            ~CUpdate() override = default;


            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
            static const std::string& getRestKeyword();

            boost::shared_ptr<shared::serialization::IDataSerializable> scanForUpdatesV1(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> availableUpdatesV1(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateYadomsV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateWidgetV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> installWidgetV1(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> removeWidgetV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updatePluginV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> installPluginV1(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> removePluginV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateScriptInterpreterV1(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> installScriptInterpreterV1(const std::vector<std::string>& parameters,
                                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> removeScriptInterpreterV1(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const;

            enum class UpdatePackage
            {
               kYadoms,
               kPlugin,

               //TODO à virer ?
               kWidget,

               kScriptInterpreter
            };

            std::string findPackageUrl(UpdatePackage updatePackage,
                                       const std::string& version) const;

            static void extractVersions(const boost::shared_ptr<shared::CDataContainer>& updates,
                                        const std::string& path);
            static void extractComponentVersions(const boost::shared_ptr<shared::CDataContainer>& updates,
                                                 const std::string& componentTag);
            static boost::shared_ptr<shared::CDataContainer> formatUpdates(const boost::shared_ptr<shared::CDataContainer>& availableUpdates);

            boost::shared_ptr<IAnswer> getAvailableUpdatesV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> scanForUpdatesV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> updateYadomsV2(const boost::shared_ptr<IRequest>& request) const;

            boost::shared_ptr<update::IUpdateManager> m_updateManager;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;

            static std::string m_restKeyword;
            boost::shared_ptr<ITaskInProgressHandler> m_scanForUpdatesInProgressTaskUidHandler;
            boost::shared_ptr<ITaskInProgressHandler> m_updateYadomsInProgressTaskUidHandler;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
