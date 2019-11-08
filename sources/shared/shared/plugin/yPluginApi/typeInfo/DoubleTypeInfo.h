#pragma once
#include "ITypeInfo.h"
#include <shared/Field.hpp>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace typeInfo
         {
            //-----------------------------------------------------
            ///\brief Interface for keyword type information
            //-----------------------------------------------------
            class CDoubleTypeInfo : public ITypeInfo
            {
            public:
               //-----------------------------------------------------
               ///\brief   Empty container (can be with references)
               //-----------------------------------------------------
               static const CDoubleTypeInfo Empty;

               CDoubleTypeInfo() = default;
               virtual ~CDoubleTypeInfo() = default;

               //-----------------------------------------------------
               ///\brief   Set the min value
               ///\param [in] minValue : The minimum value
               ///\return  A reference to itself to allow method chaining
               //-----------------------------------------------------
               CDoubleTypeInfo& setMin(const double minValue);

               //-----------------------------------------------------
               ///\brief   Set the max value
               ///\param [in] maxValue : The maximum value
               ///\return  A reference to itself to allow method chaining
               //-----------------------------------------------------
               CDoubleTypeInfo& setMax(const double maxValue);

               //-----------------------------------------------------
               ///\brief   Set the step value
               ///\param [in] stepValue : The step value
               ///\return  A reference to itself to allow method chaining
               //-----------------------------------------------------
               CDoubleTypeInfo& setStep(const double stepValue);

               //-----------------------------------------------------
               ///\brief   Set the precision value
               ///\param [in] precisionValue : The precision value
               ///\return  A reference to itself to allow method chaining
               //-----------------------------------------------------
               CDoubleTypeInfo& setPrecision(const double precisionValue);

               // ITypeInfo implementation 
               CDataContainer serialize() const override;
               // END ITypeInfo implementation 

            private:
               CField<double> m_min;
               CField<double> m_max;
               CField<double> m_step;
               CField<double> m_precision;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo
