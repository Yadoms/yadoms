#pragma once

#include <shared/plugin/yadomsApi/historization/IHistorizable.h>
#include <shared/DataContainer.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief     Template class which can be used to declare a single data IHistorizable value
   //-----------------------------------------------------
   template<class T>
   class CSingleHistorizableData : public IHistorizable
   {
   protected:
      //-----------------------------------------------------
      ///\brief                        Constructor
      ///\param[in] keywordName        the keyword name
      ///\param[in] capacity           the capacity
      ///\param[in] yadomsIdentifier   the identifier used by yadoms in DataContainer
      ///\param[in] accessMode         The access mode
      ///\param[in] measureType        The measure type
      //-----------------------------------------------------
      CSingleHistorizableData(const std::string& keywordName, const CStandardCapacity& capacity, const std::string & yadomsIdentifier, const EKeywordAccessMode& accessMode, const EMeasureType measureType = EMeasureType::kAbsolute)
         :m_keywordName(keywordName), m_capacity(capacity), m_yadomsIdentifier(yadomsIdentifier), m_accessMode(accessMode), m_measureType(measureType)
      {
      }      
      
      //-----------------------------------------------------
      ///\brief                        Constructor
      ///\param[in] keywordName        the keyword name
      ///\param[in] capacity           the capacity
      ///\param[in] yadomsIdentifier   the identifier used by yadoms in DataContainer
      ///\param[in] accessMode         The access mode
      ///\param[in] initialValue       the initial value
      ///\param[in] measureType        The measure type
      //-----------------------------------------------------
      CSingleHistorizableData(const std::string& keywordName, const CStandardCapacity& capacity, const std::string & yadomsIdentifier, const EKeywordAccessMode& accessMode, const T initialValue, const EMeasureType measureType = EMeasureType::kAbsolute)
         :m_keywordName(keywordName), m_capacity(capacity), m_yadomsIdentifier(yadomsIdentifier), m_value(initialValue), m_accessMode(accessMode), m_measureType(measureType)
      {
      }

   public:
      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CSingleHistorizableData()
      {

      }

      // IHistorizable implementation
      virtual const std::string& getKeyword() const
      {
         return m_keywordName;
      }
      
      virtual const CStandardCapacity& getCapacity() const
      {
         return m_capacity;
      }
      
      virtual const std::string formatValue() const
      {
         return boost::lexical_cast<std::string>(m_value);
      }

      virtual const EKeywordAccessMode& getAccessMode() const
      {
         return m_accessMode;
      }

      virtual const EMeasureType& getMeasureType() const
      {
         return m_measureType;
      }
      // [END] IHistorizable implementation   
   
   
      //-----------------------------------------------------
      ///\brief                     Set value from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      virtual void set(const CDataContainer& yadomsCommand)
      {
         m_value = Normalize(yadomsCommand.get<T>(m_yadomsIdentifier));
      }

      //-----------------------------------------------------
      ///\brief                     Set value
      ///\param[in] command         
      //-----------------------------------------------------
      virtual void set(T value)
      {
         m_value = Normalize(value);
      }

      //-----------------------------------------------------
      ///\brief                     Get the value
      ///\return                    The value
      //-----------------------------------------------------
      virtual const T get() const
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
      T const operator() () const
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
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;         
      
      //-----------------------------------------------------
      ///\brief               The capacity
      //-----------------------------------------------------      
      const CStandardCapacity & m_capacity;

      //-----------------------------------------------------
      ///\brief                     The yadoms identifier in datacontainer
      //-----------------------------------------------------
      const std::string m_yadomsIdentifier;

      //-----------------------------------------------------
      ///\brief               The command value
      //-----------------------------------------------------
      T m_value;

      //-----------------------------------------------------
      ///\brief               The access mode
      //-----------------------------------------------------
      const EKeywordAccessMode& m_accessMode;

      //-----------------------------------------------------
      ///\brief               The measure type
      //-----------------------------------------------------     
      const EMeasureType m_measureType;
   };



} } } } // namespace shared::plugin::yadomsApi::historization
