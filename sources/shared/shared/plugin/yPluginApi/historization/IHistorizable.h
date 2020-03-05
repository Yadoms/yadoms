#pragma once
#include "../StandardCapacities.h"
#include "../KeywordAccessMode.h"
#include "../MeasureType.h"
#include "../HistoryDepth.h"
#include <shared/DataContainer.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief The historizable object interface
            //-----------------------------------------------------
            class IHistorizable
            {
            public:
               //-----------------------------------------------------
               ///\brief               Destructor
               //-----------------------------------------------------
               virtual ~IHistorizable() = default;

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
               ///\brief                     Get the capacity of the historizable object
               ///\return                    The capacity
               //-----------------------------------------------------
               virtual const EKeywordAccessMode& getAccessMode() const = 0;

               //-----------------------------------------------------
               ///\brief                     Format value to Yadoms format
               ///\return                    Formatted data
               //-----------------------------------------------------
               virtual std::string formatValue() const = 0;

               //-----------------------------------------------------
               ///\brief                     The measure type
               ///\return                    Measure type
               //-----------------------------------------------------
               virtual const EMeasureType& getMeasureType() const = 0;

               //-----------------------------------------------------
               ///\brief                     The type information
               ///\return                    Type information
               //-----------------------------------------------------
               virtual CDataContainerSharedPtr getTypeInfo() const = 0;

               //-----------------------------------------------------
               ///\brief                     The history depth policy
               ///\return                    History depth policy
               //-----------------------------------------------------
               virtual const EHistoryDepth& getHistoryDepth() const = 0;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
