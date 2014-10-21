#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A speed historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CSpeed : public CSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CSpeed(const std::string& keywordName, const EMeasureType& measureType = EMeasureType::kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CSpeed();
   };



} } } } // namespace shared::plugin::yadomsApi::historization

