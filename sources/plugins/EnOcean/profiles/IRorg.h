#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IFunc.h"

namespace yApi = shared::plugin::yPluginApi;

class IRorg
{
public:
   virtual ~IRorg()
   {
   }

public:
   virtual unsigned int id() const = 0;
   virtual const std::string& title() const = 0;
   virtual const std::string& fullname() const = 0;
   virtual bool isTeachIn(const boost::dynamic_bitset<>& erp1Data) const = 0;
   virtual bool isEepProvided(const boost::dynamic_bitset<>& erp1Data) const = 0;
   virtual boost::shared_ptr<IFunc> createFunc(unsigned int funcId) const = 0;
};

