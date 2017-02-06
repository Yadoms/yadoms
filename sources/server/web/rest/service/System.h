#pragma once

#include "IRestService.h"
#include "web/rest/RestDispatcher.h"
#include "IRunningInformation.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CSystem : public IRestService
         {
         public:
            explicit CSystem();
            virtual ~CSystem();

         public:
            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();


         public:
            shared::CDataContainer getBinding(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getSystemInformation(const std::vector<std::string>& parameters, const std::string& requestContent) const;

         private:
            shared::CDataContainer getSerialPorts() const;
            shared::CDataContainer getNetworkInterfaces(bool includeLoopback) const;
            shared::CDataContainer platformIs(const std::string& refPlatform) const;

            static std::string m_restKeyword;

            boost::shared_ptr<IRunningInformation> m_runningInformation;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


