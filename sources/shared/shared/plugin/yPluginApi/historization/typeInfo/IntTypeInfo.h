#pragma once
#include "ITypeInfo.h"
#include <shared/Export.h>
#include <shared/Field.hpp>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            namespace typeInfo
            {
               //-----------------------------------------------------
               ///\brief Interface for keyword type information
               //-----------------------------------------------------
               class YADOMS_SHARED_EXPORT CIntTypeInfo : public ITypeInfo
               {
               public:
                  //-----------------------------------------------------
                  ///\brief   Empty container (can be with references)
                  //-----------------------------------------------------
                  static CIntTypeInfo Empty;

                  //-----------------------------------------------------
                  ///\brief   Constructor
                  //-----------------------------------------------------
                  CIntTypeInfo();

                  //-----------------------------------------------------
                  ///\brief   Destructor
                  //-----------------------------------------------------
                  virtual ~CIntTypeInfo();

                  //-----------------------------------------------------
                  ///\brief   Set the min value
                  ///\param [in] minValue : The minimum value
                  ///\return  A reference to itself to allow method chaining
                  //-----------------------------------------------------
                  CIntTypeInfo& setMin(const int minValue);

                  //-----------------------------------------------------
                  ///\brief   Set the max value
                  ///\param [in] maxValue : The maximum value
                  ///\return  A reference to itself to allow method chaining
                  //-----------------------------------------------------
                  CIntTypeInfo& setMax(const int maxValue);

                  //-----------------------------------------------------
                  ///\brief   Set the step value
                  ///\param [in] stepValue : The step value
                  ///\return  A reference to itself to allow method chaining
                  //-----------------------------------------------------
                  CIntTypeInfo& setStep(const int stepValue);

                  // ITypeInfo implementation 
                  CDataContainer serialize() const override;
                  // END ITypeInfo implementation 

               private:
                  CField<int> m_min;
                  CField<int> m_max;
                  CField<int> m_step;
               };
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo


