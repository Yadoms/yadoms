#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IMessageHandler.h"

namespace yApi = shared::plugin::yPluginApi;

class IType
{
public:
   virtual ~IType()
   {
   }

   virtual const std::string& profile() const = 0;
   virtual const std::string& title() const = 0;
   virtual std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> allHistorizers() const = 0;
   virtual std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> states(const boost::dynamic_bitset<>& data,
                                                                                           const boost::dynamic_bitset<>& status) const = 0;
   virtual void sendCommand(const std::string& keyword,
                            const std::string& commandBody,
                            boost::shared_ptr<IMessageHandler> messageHandler) const = 0;
   virtual void sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                  boost::shared_ptr<IMessageHandler> messageHandler) const = 0;
};

