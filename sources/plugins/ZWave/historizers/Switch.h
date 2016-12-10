#pragma once
#include "../OpenZWaveSingleHistorizableData.h"
#include "../typeinfo/BoolTypeInfo.h"

namespace historizers {

   class CSwitch : public COpenZWaveSingleHistorizableData<bool>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CSwitch(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CBoolTypeInfo & ti);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CSwitch();
   };

} //namespace historizers 