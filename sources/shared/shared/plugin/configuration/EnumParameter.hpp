#pragma once


#include <shared/Export.h>
#include "EnumGenericParameter.h"
#include <shared/Log.h>

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \class Hardware plugin configuration enum parameter
   //--------------------------------------------------------------
   template<typename Enum>
   class CEnumParameter : public CEnumGeneric
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] description  Parameter description
      /// \param[in] defaultValue  Parameter default value
      /// \param[in] valueNames  List of authorized values for the enum
      //--------------------------------------------------------------
      CEnumParameter(const std::string& name, const std::string& description, Enum defaultValue, const ValuesNames& valueNames)
         :CEnumGeneric(name, description, valueNames), m_defaultValue(defaultValue), m_value(defaultValue) {}

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CEnumParameter() {}

      // CParameter implementation
      virtual boost::shared_ptr<CParameter> clone() const
      {
         boost::shared_ptr<CParameter> p(new CEnumParameter<Enum>(getName(), getDescription(), m_value, m_valuesNames));
         return p;
      }

      void build(boost::property_tree::ptree& pt) const
      {
         CEnumGeneric::build(pt);
         pt.put(getName() + ".default", defaultValueToString());
      }

      virtual void setValue(const boost::property_tree::ptree& pt)
      {
         std::string stringValue = pt.get<std::string>(getName() + ".value");
         ValuesNames::const_iterator it;
         for (it = m_valuesNames.begin() ; it != m_valuesNames.end() ; ++it)
         {
            if ((*it).second == stringValue)
            {
               m_value = (Enum)((*it).first);
               break;
            }
         }
         if (it == m_valuesNames.end())
         {
            // It can occur if the plugin was updated and value doesn't exist anymore in the new version.
            // In this case, use default value, and log.
            m_value = m_defaultValue;
            YADOMS_LOG(info) << "Value \"" << stringValue << "\" was not found for \"" << getName() << "\" parameter, use default";
         }
      }
      // [END] CParameter implementation

      //--------------------------------------------------------------
      /// \brief	    Native getter
      /// \return    The current parameter value
      //--------------------------------------------------------------
      Enum get() const { return m_value; }

   protected:
      const std::string defaultValueToString() const
      {
         try
         {
            return m_valuesNames.at(m_defaultValue);
         }
         catch (const std::out_of_range& oor)
         {
            BOOST_ASSERT(false);  // Name of the value doesn't exist
            YADOMS_LOG(error) << "Out of Range error: " << oor.what();
            return std::string(getName() + " : Invalid value ");
         }
      }

   private:
      //--------------------------------------------------------------
      /// \brief	    The default parameter value
      //--------------------------------------------------------------
      const Enum m_defaultValue;

      //--------------------------------------------------------------
      /// \brief	    The current parameter value
      //--------------------------------------------------------------
      Enum m_value;
   };

   //--------------------------------------------------------------
   /// \brief	    Macro helper to get the parameter value
   /// \param enumType     The native enum type
   /// \param paramName    Parameter name
   //--------------------------------------------------------------
   #define CFG_GET_ENUM(enumType,paramName) getParam<shared::plugin::configuration::CEnumParameter<enumType> >(paramName).get()

} } } // namespace shared::plugin::configuration
