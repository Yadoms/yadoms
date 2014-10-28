#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>
#include "SingleHistorizableData.hpp"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A humidity historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CHumidity : public CSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CHumidity(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet, const EMeasureType& measureType = EMeasureType::kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CHumidity();

   protected:
      //-----------------------------------------------------
      ///\brief                     Normalize the humidity [0; 100]
      ///\param[in] value           Raw value
      ///\return                    The normalized value [0; 100]
      //-----------------------------------------------------
      virtual double Normalize(double value);
   };



} } } } // namespace shared::plugin::yadomsApi::historization

