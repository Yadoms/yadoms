#pragma once

#include <shared/plugin/yadomsApi/commands/IHistorizable.h>
#include <shared/DataContainer.h>

namespace xplrules { namespace rfxLanXpl { namespace data {


   template<class TEnum>
   class CHistorizableEnum : public shared::plugin::yadomsApi::commands::IHistorizable
   {
   protected:
      //-----------------------------------------------------
      ///\brief                        Constructor
      ///\param[in] keywordName        the keyword name
      ///\param[in] capacity           the capacity
      ///\param[in] yadomsIdentifier   the identifier used by yadoms in DataContainer
      //-----------------------------------------------------
      CHistorizableEnum(const std::string& keywordName, const shared::plugin::yadomsApi::CStandardCapacity& capacity, const std::string & yadomsIdentifier)
      :m_keywordName(keywordName), m_capacity(capacity), m_yadomsIdentifier(yadomsIdentifier)
      {
      }      
      
      //-----------------------------------------------------
      ///\brief                        Constructor
      ///\param[in] keywordName        the keyword name
      ///\param[in] capacity           the capacity
      ///\param[in] yadomsIdentifier   the identifier used by yadoms in DataContainer
      ///\param[in] initialValue       the initial value
      //-----------------------------------------------------
      CHistorizableEnum(const std::string& keywordName, const shared::plugin::yadomsApi::CStandardCapacity& capacity, const std::string & yadomsIdentifier, const TEnum initialValue)
      :m_keywordName(keywordName), m_capacity(capacity), m_yadomsIdentifier(yadomsIdentifier), m_value(initialValue)
      {
      }

   public:
      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CHistorizableEnum()
      {

      }

      // IHistorizable implementation
      virtual const std::string& getKeyword() const
      {
         return m_keywordName;
      }
      
      virtual const shared::plugin::yadomsApi::CStandardCapacity& getCapacity() const
      {
         return m_capacity;
      }
      
      virtual const std::string formatValue(const std::string& currentValue = shared::CStringExtension::EmptyString) const
      {
         return m_value.getAsString();
      }

      virtual EMeasureType getMeasureType() const
      {
         return kAbsolute;
      }
      // [END] IHistorizable implementation   
   
   
      //-----------------------------------------------------
      ///\brief                     Set value from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      virtual void set(const shared::CDataContainer& yadomsCommand)
      {
         m_value = yadomsCommand.get<TEnum>(m_yadomsIdentifier);
      }

      //-----------------------------------------------------
      ///\brief                     Set value
      ///\param[in] command         
      //-----------------------------------------------------
      virtual void set(TEnum value)
      {
         m_value = value;
      }

      //-----------------------------------------------------
      ///\brief                     Get the value
      ///\return                    The value
      //-----------------------------------------------------
      virtual const TEnum get() const
      {
         return m_value;
      }

      
   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;         
      
      //-----------------------------------------------------
      ///\brief                     The yadoms identifier in datacontainer
      //-----------------------------------------------------
      const std::string m_yadomsIdentifier;

      //-----------------------------------------------------
      ///\brief               The command value
      //-----------------------------------------------------
      TEnum m_value;
      
      //-----------------------------------------------------
      ///\brief               The capacity
      //-----------------------------------------------------      
      const shared::plugin::yadomsApi::CStandardCapacity & m_capacity;
   
   };



} } } // namespace xplrules::rfxLanXpl::data

