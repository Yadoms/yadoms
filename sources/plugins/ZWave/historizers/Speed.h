#pragma once
#include "../OpenZWaveSingleHistorizableData.h"
#include "../typeinfo/DecimalTypeInfo.h"

namespace historizers
{
   class CSpeed : public COpenZWaveSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CSpeed(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CDecimalTypeInfo &ti);

      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     The measure type (normally kAbsolute)
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CSpeed(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::EMeasureType measureType, CDecimalTypeInfo &ti);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CSpeed();

      // COpenZWaveSingleHistorizableData<Poco::Int64> override ------------------------
      void setWithUnits(double value, const std::string& unit) override;
      double getWithUnits(const std::string& unit) const override;
      // [END] - COpenZWaveSingleHistorizableData<double> override ------------------------
   };
} //namespace historizers 


