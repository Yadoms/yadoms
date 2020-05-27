#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>


template <class T>
class COpenZWaveNodeKeywordGenericByType : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<T>
{
public:
   //-----------------------------------------------------
   ///\brief                        Constructor
   ///\param[in] keywordName        the keyword name
   ///\param[in] capacity           the capacity
   ///\param[in] accessMode         The access mode
   ///\param[in] measureType        The measure type
   ///\param[in] typeInfo           keyword type information
   //-----------------------------------------------------
   COpenZWaveNodeKeywordGenericByType(const std::string& keywordName,
                                      const shared::plugin::yPluginApi::CStandardCapacity& capacity,
                                      const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode,
                                      const shared::plugin::yPluginApi::EMeasureType& measureType = shared::plugin::yPluginApi::EMeasureType::kAbsolute,
                                      const shared::plugin::yPluginApi::typeInfo::ITypeInfo& typeInfo = shared::plugin::yPluginApi::typeInfo::CEmptyTypeInfo::Empty)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<T>(keywordName, capacity, accessMode, measureType, typeInfo)
   {
   }

   //-----------------------------------------------------
   ///\brief                        Constructor
   ///\param[in] keywordName        the keyword name
   ///\param[in] capacity           the capacity
   ///\param[in] accessMode         The access mode
   ///\param[in] initialValue       the initial value
   ///\param[in] measureType        The measure type
   ///\param[in] typeInfo           keyword type information
   //-----------------------------------------------------
   COpenZWaveNodeKeywordGenericByType(const std::string& keywordName,
                                      const shared::plugin::yPluginApi::CStandardCapacity& capacity,
                                      const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode,
                                      const T& initialValue,
                                      const shared::plugin::yPluginApi::EMeasureType& measureType = shared::plugin::yPluginApi::EMeasureType::kAbsolute,
                                      const shared::plugin::yPluginApi::typeInfo::ITypeInfo& typeInfo = shared::plugin::yPluginApi::typeInfo::CEmptyTypeInfo::Empty)
      : shared::plugin::yPluginApi::historization::CSingleHistorizableData<T>(keywordName, capacity, accessMode, initialValue, measureType, typeInfo)
   {
   }

   virtual ~COpenZWaveNodeKeywordGenericByType()
   {
   }
};

