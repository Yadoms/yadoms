#pragma once
#include "IGeneralInfo.h"

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The script properties
      //-----------------------------------------------------
      class CGeneralInfo : public IGeneralInfo
      {
      public:
         //-----------------------------------------------------
         ///\brief               Constructor
         //-----------------------------------------------------
         CGeneralInfo();

         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~CGeneralInfo();

      protected:
         // IGeneralInfo Implementation
         std::string get(shared::script::yScriptApi::IYScriptApi::EInfoKeys key) const override;
         // [END] IGeneralInfo Implementation
      };
   }
} // namespace automation::script


