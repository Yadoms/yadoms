#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   class IFanSubtype
   {
   public:
      virtual ~IFanSubtype() {}

      virtual std::string getModel() const = 0;
      virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const = 0;
      virtual void set(const std::string& keyword, const std::string& yadomsCommand) = 0;
      virtual void reset() = 0;
      virtual unsigned char toProtocolCmnd() const = 0;
      virtual unsigned long idFromConfiguration(const shared::CDataContainerSharedPtr& configuration) const = 0;
   };

} // namespace rfxcomMessages
