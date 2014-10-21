#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A frequency historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CFrequency : public CSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CFrequency(const std::string& keywordName, const EMeasureType& measureType = EMeasureType::kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CFrequency();
   };



} } } } // namespace shared::plugin::yadomsApi::historization

