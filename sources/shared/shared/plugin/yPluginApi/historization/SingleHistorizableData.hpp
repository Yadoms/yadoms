#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/enumeration/IExtendedEnum.h>
#include "../typeInfo/ITypeInfo.h"
#include "../typeInfo/EnumTypeInfo.hpp"
#include "../typeInfo/EmptyTypeInfo.h"
#include <shared/StringExtension.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief     Template class which can be used to declare a single data IHistorizable value
            //-----------------------------------------------------
            template <class T>
            class CSingleHistorizableData : public IHistorizable
            {
            protected:
               //-----------------------------------------------------
               ///\brief                        Constructor
               ///\param[in] keywordName        the keyword name
               ///\param[in] capacity           the capacity
               ///\param[in] accessMode         The access mode
               ///\param[in] measureType        The measure type
               ///\param[in] typeInfo           The type information
               ///\param[in] historyDepth       The history depth policy
               //-----------------------------------------------------
               CSingleHistorizableData(const std::string& keywordName,
                                       const CStandardCapacity& capacity,
                                       const EKeywordAccessMode& accessMode,
                                       const EMeasureType& measureType = EMeasureType::kAbsolute,
                                       const typeInfo::ITypeInfo& typeInfo = typeInfo::CEmptyTypeInfo::Empty,
                                       const EHistoryDepth& historyDepth = EHistoryDepth::kDefault)
                  : m_keywordName(keywordName),
                    m_capacity(capacity),
                    m_value(),
                    m_accessMode(accessMode),
                    m_measureType(measureType),
                    m_typeInfo(typeInfo.serialize()),
                    m_historyDepth(historyDepth)
               {
               }

               //-----------------------------------------------------
               ///\brief                        Constructor
               ///\param[in] keywordName        the keyword name
               ///\param[in] capacity           the capacity
               ///\param[in] accessMode         The access mode
               ///\param[in] initialValue       the initial value
               ///\param[in] measureType        The measure type
               ///\param typeInfo               The type information
               ///\param historyDepth           The history depth policy
               //-----------------------------------------------------
               CSingleHistorizableData(const std::string& keywordName,
                                       const CStandardCapacity& capacity,
                                       const EKeywordAccessMode& accessMode,
                                       const T& initialValue,
                                       const EMeasureType& measureType = EMeasureType::kAbsolute,
                                       const typeInfo::ITypeInfo& typeInfo = typeInfo::CEmptyTypeInfo::Empty,
                                       const EHistoryDepth& historyDepth = EHistoryDepth::kDefault)
                  : m_keywordName(keywordName),
                    m_capacity(capacity),
                    m_value(initialValue),
                    m_accessMode(accessMode),
                    m_measureType(measureType),
                    m_typeInfo(typeInfo.serialize()),
                    m_historyDepth(historyDepth)
               {
               }

            public:
               //-----------------------------------------------------
               ///\brief                        Constructor (copy)
               ///\param[in] rhs                the object to copy
               //-----------------------------------------------------
               CSingleHistorizableData(const CSingleHistorizableData<T>& rhs)
                  : m_keywordName(rhs.m_keywordName),
                    m_capacity(rhs.m_capacity),
                    m_value(rhs.m_value),
                    m_accessMode(rhs.m_accessMode),
                    m_measureType(rhs.m_measureType),
                    m_typeInfo(rhs.m_typeInfo),
                    m_historyDepth(rhs.m_historyDepth)
               {
               }

               virtual ~CSingleHistorizableData() = default;

               CSingleHistorizableData& operator =(const CSingleHistorizableData<T>& rhs) = delete;

               // IHistorizable implementation
               const std::string& getKeyword() const override
               {
                  return m_keywordName;
               }

               const CStandardCapacity& getCapacity() const override
               {
                  return m_capacity;
               }

               std::string formatValue() const override
               {
                  return CStringExtension::cultureInvariantToString(m_value);
               }

               const EKeywordAccessMode& getAccessMode() const override
               {
                  return m_accessMode;
               }

               const EMeasureType& getMeasureType() const override
               {
                  return m_measureType;
               }

               boost::shared_ptr<CDataContainer> getTypeInfo() const override
               {
                  //if not defined, use empty result
                  if (!m_typeInfo || m_typeInfo->empty())
                     return helper<T>::createDefaultTypeInfo();
                  return m_typeInfo;
               }

               const EHistoryDepth& getHistoryDepth() const override
               {
                  return m_historyDepth;
               }

               // [END] IHistorizable implementation   


               //-----------------------------------------------------
               ///\brief                     Set value from Yadoms command
               ///\param[in] yadomsCommand   Yadoms command
               ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
               //-----------------------------------------------------
               virtual void setCommand(const std::string& yadomsCommand)
               {
                  set(helper<T>::getInternal(yadomsCommand));
               }

               //-----------------------------------------------------
               ///\brief                     Set value
               ///\param[in] value           The value         
               //-----------------------------------------------------
               virtual void set(T value)
               {
                  m_value = Normalize(value);
               }

               //-----------------------------------------------------
               ///\brief                     Get the value
               ///\return                    The value
               //-----------------------------------------------------
               virtual T get() const
               {
                  return m_value;
               }

               //-----------------------------------------------------
               ///\brief                     Implicit operator
               ///\return                    The value
               //-----------------------------------------------------
               operator T() const
               {
                  return get();
               }

               //-----------------------------------------------------
               ///\brief                     Explicit operator
               ///\return                    The value
               //-----------------------------------------------------
               T operator()() const
               {
                  return get();
               }


            protected:
               //-----------------------------------------------------
               ///\brief                     Normalize the value
               ///\param[in] value           Raw value
               ///\return                    The normalized value
               //-----------------------------------------------------
               virtual T Normalize(T value)
               {
                  return value;
               }


            private:
               const std::string m_keywordName;
               const CStandardCapacity& m_capacity;
               T m_value;
               const EKeywordAccessMode m_accessMode;
               const EMeasureType m_measureType;
               const boost::shared_ptr<CDataContainer> m_typeInfo;
               const EHistoryDepth m_historyDepth;

               //-----------------------------------------------------
               ///\brief     Helpers to normalize access to simple value and enum values
               //-----------------------------------------------------
               template <typename TData, class Enable = void>
               struct helper
               {
                  static TData getInternal(const std::string& value)
                  {
                     return boost::lexical_cast<TData>(value);
                  }

                  static boost::shared_ptr<CDataContainer> createDefaultTypeInfo()
                  {
                     return shared::CDataContainer::make();
                  }
               };

               //-----------------------------------------------------
               ///\brief     Helpers specialization for bool
               //-----------------------------------------------------      
               template <typename TData>
               struct helper<TData, typename boost::enable_if<boost::is_same<boost::shared_ptr<CDataContainer>, TData>>::type>
               {
                  static boost::shared_ptr<CDataContainer> getInternal(const std::string& value)
                  {
                     return shared::CDataContainer::make(value);
                  }

                  static boost::shared_ptr<CDataContainer> createDefaultTypeInfo()
                  {
                     return shared::CDataContainer::make();
                  }
               };


               //-----------------------------------------------------
               ///\brief     Helpers specialization for bool
               //-----------------------------------------------------      
               template <typename TData>
               struct helper<TData, typename boost::enable_if<boost::is_same<bool, TData>>::type>
               {
                  static bool getInternal(const std::string& value)
                  {
                     return (value == "1" || boost::to_lower_copy(value) == "true");
                  }

                  static boost::shared_ptr<CDataContainer> createDefaultTypeInfo()
                  {
                     return shared::CDataContainer::make();
                  }
               };

               //-----------------------------------------------------
               ///\brief     Helpers specialization for int
               //-----------------------------------------------------      
               template <typename TData>
               struct helper<TData, typename boost::enable_if<boost::is_same<int, TData>>::type>
               {
                  static int getInternal(const std::string& value)
                  {
                     try
                     {
                        return boost::lexical_cast<int>(value);
                     }
                     catch (boost::bad_lexical_cast&)
                     {
                        return static_cast<int>(boost::lexical_cast<float>(value));
                     }
                  }

                  static boost::shared_ptr<CDataContainer> createDefaultTypeInfo()
                  {
                     return shared::CDataContainer::make();
                  }
               };

               //-----------------------------------------------------
               ///\brief     Helpers specialization for unsigned char
               //-----------------------------------------------------      
               template <typename TData>
               struct helper<TData, typename boost::enable_if<boost::is_same<unsigned char, TData>>::type>
               {
                  static unsigned char getInternal(const std::string& value)
                  {
                     try
                     {
                        //force a cast to int (without int cast, lexicalcast take the ascii value of int: "1" => 49, instead of 1)
                        const auto v = boost::lexical_cast<int>(value);
                        return static_cast<unsigned char>(std::max(0, std::min(255, v)));
                     }
                     catch (boost::bad_lexical_cast&)
                     {
                        return static_cast<unsigned char>(std::max(0, std::min(255, static_cast<int>(boost::lexical_cast<float>(value)))));
                     }
                  }

                  static boost::shared_ptr<CDataContainer> createDefaultTypeInfo()
                  {
                     return shared::CDataContainer::make();
                  }
               };

               //-----------------------------------------------------
               ///\brief     Helpers specialization for ExtendedEnum
               //-----------------------------------------------------      
               template <typename TData>
               struct helper<TData, typename boost::enable_if<boost::is_base_of<enumeration::IExtendedEnum, TData>>::type>
               {
                  static TData getInternal(const std::string& value)
                  {
                     return TData(value);
                  }

                  static boost::shared_ptr<CDataContainer> createDefaultTypeInfo()
                  {
                     typeInfo::CEnumTypeInfo<TData> ti;
                     return ti.serialize();
                  }
               };

               //-----------------------------------------------------
               ///\brief     Helpers specialization for boost::posix_time::ptime
               //-----------------------------------------------------      
               template <typename TData>
               struct helper<TData, typename boost::enable_if<boost::is_base_of<boost::posix_time::ptime, TData>>::type>
               {
                  static TData getInternal(const std::string& value)
                  {
                     return TData(boost::posix_time::from_iso_string(value));
                  }

                  static boost::shared_ptr<CDataContainer> createDefaultTypeInfo()
                  {
                     return shared::CDataContainer::make();
                  }
               };

               //-----------------------------------------------------
               ///\brief     Helpers specialization for unsigned char*
               //-----------------------------------------------------      
               template <typename TData>
               struct helper<TData, typename boost::enable_if<boost::is_base_of<unsigned char *, TData>>::type>
               {
                  static TData getInternal(const std::string& value)
                  {
                     return;
                  }

                  static boost::shared_ptr<CDataContainer> createDefaultTypeInfo()
                  {
                     return shared::CDataContainer::make();
                  }
               };
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
