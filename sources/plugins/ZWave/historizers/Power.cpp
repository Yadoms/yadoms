#include "stdafx.h"
#include "Power.h"
#include <shared/plugin/yPluginApi/historization/Power.h>

namespace historizers {

   CPower::CPower(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<double>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> >(new shared::plugin::yPluginApi::historization::CPower(name, accessMode)))
   {

   }

   CPower::CPower(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<double>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> >(new shared::plugin::yPluginApi::historization::CPower(name, accessMode, measureType)))
   {

   }

   CPower::~CPower()
   {
   }

   // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
   void CPower::setWithUnits(double value, const std::string & unit)
   {
      if (unit == "BTU/h")
         set(value * 0.29307107);
      else
         set(value);
   }

   double CPower::getWithUnits(const std::string & unit) const
   {
      if (unit == "BTU/h")
         return get() / 0.29307107;
      return get();
   }
   // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------


} //namespace historizers 