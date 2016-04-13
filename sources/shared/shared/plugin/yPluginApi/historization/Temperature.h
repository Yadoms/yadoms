#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include "typeInfo/DoubleTypeInfo.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A temperature historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CTemperature : public CSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CTemperature(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet, const EMeasureType& measureType = EMeasureType::kAbsolute, typeInfo::CDoubleTypeInfo & additionalInfo = typeInfo::CDoubleTypeInfo::Empty);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CTemperature();


      // CSingleHistorizableData<double> override ------------------------
      virtual void setWithUnits(double value, const std::string & unit);
      virtual double getWithUnits(const std::string & unit) const;
      // [END] - CSingleHistorizableData<double> override ------------------------

   private:
      //-----------------------------------------------------
      ///\brief  Check if given unit match fahrenheit
      ///\param[in] unit  The unit 
      ///\return  true if the unit is recognized as Fahrenheit 
      //-----------------------------------------------------
      bool isUnitFahrenheit(const std::string & unit) const;

      //-----------------------------------------------------
      ///\brief  Set temperature from a Fahrenheit  value
      ///\param[in] value  The temperature value in Fahrenheit 
      //-----------------------------------------------------
      void setFahrenheit(double value);

      //-----------------------------------------------------
      ///\brief  Get the temperature as Fahrenheit value
      ///\return The temeprature in Fahrenheit
      //-----------------------------------------------------
      double getFahrenheit() const;

   };



} } } } // namespace shared::plugin::yPluginApi::historization

