#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A temperature historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CBatteryLevel : public CSingleHistorizableData<int>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CBatteryLevel(const std::string& keywordName, const EMeasureType& measureType = EMeasureType::kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CBatteryLevel();

   protected:
      //-----------------------------------------------------
      ///\brief                     Normalize the humidity [0; 100]
      ///\param[in] value           Raw value
      ///\return                    The normalized value [0; 100]
      //-----------------------------------------------------
      virtual int Normalize(int value);
   };



} } } } // namespace shared::plugin::yadomsApi::historization

