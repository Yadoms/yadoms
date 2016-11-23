#pragma once
#include "../OpenZWaveSingleHistorizableData.h"
#include "../typeinfo/DecimalTypeInfo.h"

namespace historizers
{
   class CTemperature : public COpenZWaveSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CTemperature(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CTemperature();

      // COpenZWaveSingleHistorizableData<double> override ------------------------
      void setWithUnits(double value, const std::string& unit) override;
      double getWithUnits(const std::string& unit) const override;
      // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------

   private:
      //-----------------------------------------------------
      ///\brief  Check if given unit match fahrenheit
      ///\param[in] unit  The unit 
      ///\return  true if the unit is recognized as Fahrenheit 
      //-----------------------------------------------------
      bool isUnitFahrenheit(const std::string& unit) const;

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
} //namespace historizers 


