#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IType.h"

namespace yApi = shared::plugin::yPluginApi;

class IFunc
{
public:
   virtual ~IFunc()
   {
   }

   virtual unsigned int id() const = 0;
   virtual const std::string& title() const = 0;
   virtual boost::shared_ptr<IType> createType(unsigned int typeId,
                                               const std::string& deviceId,
                                               boost::shared_ptr<yApi::IYPluginApi> api) const = 0;
};
