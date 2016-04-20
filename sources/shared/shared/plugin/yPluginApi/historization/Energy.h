#pragma once
#include <shared/Export.h>
#include <Poco/Types.h>
#include "SingleHistorizableData.hpp"
#include "typeInfo/DoubleTypeInfo.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A energy historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CEnergy : public CSingleHistorizableData<Poco::Int64>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
      //-----------------------------------------------------
      CEnergy(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet, const EMeasureType& measureType = EMeasureType::kCumulative, typeInfo::CDoubleTypeInfo & additionalInfo = typeInfo::CDoubleTypeInfo::Empty);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CEnergy();
   };

   //-----------------------------------------------------
   ///\brief A energy historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CEnergyDouble : public CSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
      //-----------------------------------------------------
      CEnergyDouble(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet, const EMeasureType& measureType = EMeasureType::kCumulative, typeInfo::CDoubleTypeInfo & additionalInfo = typeInfo::CDoubleTypeInfo::Empty);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CEnergyDouble();
   };

} } } } // namespace shared::plugin::yPluginApi::historization