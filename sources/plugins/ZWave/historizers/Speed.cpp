#include "stdafx.h"
#include "Speed.h"
#include <shared/plugin/yPluginApi/historization/Speed.h>

namespace historizers
{
   CSpeed::CSpeed(const std::string& name,
                  shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CSpeed>(name,
                                                                                                                       accessMode))
   {
   }

   CSpeed::CSpeed(const std::string& name,
                  shared::plugin::yPluginApi::EKeywordAccessMode accessMode,
                  shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<double>(boost::make_shared<shared::plugin::yPluginApi::historization::CSpeed>(name,
                                                                                                                       accessMode,
                                                                                                                       measureType))
   {
   }

   CSpeed::~CSpeed()
   {
   }

   // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
   void CSpeed::setWithUnits(double value,
                             const std::string& unit)
   {
      if (unit == "mph")
         set(value * 0.44704);
      else
         set(value);
   }

   double CSpeed::getWithUnits(const std::string& unit) const
   {
      if (unit == "mph")
         return get() / 0.44704;
      return get();
   }

   // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------
} //namespace historizers 


