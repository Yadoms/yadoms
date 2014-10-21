#pragma once
#include "../StandardCapacities.h"
#include "MeasureType.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief The historizable object interface
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IHistorizable() {}

      //-----------------------------------------------------
      ///\brief                     Get the keyword name
      ///\return                    The keyword name
      //-----------------------------------------------------
      virtual const std::string& getKeyword() const = 0;

      //-----------------------------------------------------
      ///\brief                     Get the capacity of the historizable object
      ///\return                    The capacity
      //-----------------------------------------------------
      virtual const CStandardCapacity& getCapacity() const = 0;

      //-----------------------------------------------------
      ///\brief                     Format value to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      virtual const std::string formatValue() const = 0;

      //-----------------------------------------------------
      ///\brief                     The measure type
      ///\return                    Measure type
      //-----------------------------------------------------
      virtual const EMeasureType& getMeasureType() const = 0;
   };

} } } } // namespace shared::plugin::yadomsApi::historization

