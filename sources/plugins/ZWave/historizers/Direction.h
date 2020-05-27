#pragma once
#include "../OpenZWaveSingleHistorizableData.h"
#include "../typeinfo/IntegerTypeInfo.h"

namespace historizers
{
   class CDirection : public COpenZWaveSingleHistorizableData<int>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CDirection(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CIntegerTypeInfo &ti);

      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     The measure type (normally kAbsolute)
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CDirection(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::EMeasureType measureType, CIntegerTypeInfo &ti);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CDirection();
   };
} //namespace historizers 


