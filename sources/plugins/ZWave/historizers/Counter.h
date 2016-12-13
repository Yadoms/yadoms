#pragma once
#include "../OpenZWaveSingleHistorizableData.h"
#include "../typeinfo/IntegerTypeInfo.h"

namespace historizers
{
   class CCounter : public COpenZWaveSingleHistorizableData<Poco::Int64>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CCounter(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CIntegerTypeInfo & ti);

      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] ti              The measure type information
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CCounter(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::historization::EMeasureType measureType, CIntegerTypeInfo & ti);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CCounter();
   };
} //namespace historizers 


