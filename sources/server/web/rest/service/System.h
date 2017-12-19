#pragma once
#include "IRestService.h"
#include "web/rest/RestDispatcher.h"
#include "IRunningInformation.h"
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include "dateTime/TimeZoneDatabase.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CSystem : public IRestService
         {
         public:
            explicit CSystem(const boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase);
            virtual ~CSystem();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();


            shared::CDataContainer getBinding(const std::vector<std::string>& parameters,
                                              const std::string& requestContent) const;
            shared::CDataContainer getSystemInformation(const std::vector<std::string>& parameters,
                                                        const std::string& requestContent) const;
            shared::CDataContainer getCurrentTime(const std::vector<std::string>& parameters,
                                                  const std::string& requestContent) const;
            shared::CDataContainer getVirtualDevicesSupportedCapacities(const std::vector<std::string>& parameters,
                                                                        const std::string& requestContent) const;

         private:
            shared::CDataContainer getSerialPorts() const;
            shared::CDataContainer getNetworkInterfaces(const bool includeLoopback) const;
            shared::CDataContainer platformIs(const std::string& refPlatform) const;
            static void addVirtualDevicesSupportedCapacity(const shared::plugin::yPluginApi::CStandardCapacity& capacity);
            static const shared::CDataContainer& getVirtualDevicesSupportedCapacities();
            shared::CDataContainer getSupportedTimezones() const;

            static std::string m_restKeyword;

            boost::shared_ptr<IRunningInformation> m_runningInformation;

            static shared::CDataContainer m_virtualDevicesSupportedCapacities;

            boost::shared_ptr<dateTime::CTimeZoneDatabase> m_timezoneDatabase;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


