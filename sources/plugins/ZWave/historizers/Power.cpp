#include "stdafx.h"
#include "Power.h"
#include <shared/plugin/yPluginApi/historization/Power.h>

namespace historizers
{
   CPower::CPower(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CPower>(name, accessMode, shared::plugin::yPluginApi::EMeasureType::kAbsolute, ti))
   {
   }

   CPower::CPower(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::EMeasureType measureType, CDecimalTypeInfo &ti)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CPower>(name, accessMode, measureType, ti))
   {

   }

   CPower::~CPower()
   {
   }

   // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
   void CPower::setWithUnits(double value, const std::string& unit)
   {
      if (unit == "BTU/h")
         set(value * 0.29307107);
      else
         set(value);
   }

   double CPower::getWithUnits(const std::string& unit) const
   {
      if (unit == "BTU/h")
         return get() / 0.29307107;
      return get();
   }

   // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------
} //namespace historizers 


