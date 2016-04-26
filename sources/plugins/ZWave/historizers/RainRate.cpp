#include "stdafx.h"
#include "RainRate.h"
#include <shared/plugin/yPluginApi/historization/RainRate.h>

namespace historizers {

   CRainRate::CRainRate(const std::string & name)
      : COpenZWaveSingleHistorizableData<double>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> >(new shared::plugin::yPluginApi::historization::CRainRate(name)))
   {

   }

   CRainRate::CRainRate(const std::string & name, shared::plugin::yPluginApi::historization::EMeasureType measureType)
      : COpenZWaveSingleHistorizableData<double>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> >(new shared::plugin::yPluginApi::historization::CRainRate(name, measureType)))
   {

   }

   CRainRate::~CRainRate()
   {
   }

   // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
   void CRainRate::setWithUnits(double value, const std::string & unit)
   {
      if (unit == "in/h")
         set(value / 0.03937008);
      else
         set(value);
   }

   double CRainRate::getWithUnits(const std::string & unit) const
   {
      if (unit == "in/h")
         return get() * 0.03937008;
      return get();
   }
   // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------


} //namespace historizers 