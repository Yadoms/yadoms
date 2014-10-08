#pragma once
#include "../StandardCapacities.h"
#include "../../../StringExtension.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace commands//TODO renommer commands
{
   //-----------------------------------------------------
   ///\brief The API used for the commands on device
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief               Measure Type
      ///\note    Used to say Yadoms if data need post-processing before inserting in database
      //-----------------------------------------------------
      enum EMeasureType
      {
         kAbsolute = 0,    ///< Absolute data, like temperature, humidity
         kIncrement,       ///< Say Yadoms that value must be added to current value. Ex : pulse counters (water, energy counters...)
         kCumulative       ///< the current value is a total value (like some energy power)
      };

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
      virtual EMeasureType getMeasureType() const = 0;
   };

} } } } // namespace shared::plugin::yadomsApi::commands

