#pragma once

#include <shared/Export.h>
#include "../../serialization/IPtreeToStringSerializer.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/OutOfRange.hpp>


namespace shared { namespace plugin { namespace configuration
{

   class IConfigurationSerializer;

   //--------------------------------------------------------------
   /// \class Plugin configuration
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CConfiguration
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      //--------------------------------------------------------------
      CConfiguration();
      
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CConfiguration();

      //--------------------------------------------------------------
      /// \brief	    Set configuration values from saved data
      /// \param[in] serializedConfiguration    String containing the configuration values (JSON format)
      //--------------------------------------------------------------
      virtual void set(const std::string& serializedConfiguration);

      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<typename T>
      T getValue(const std::string& parameterName) const
      {
         boost::lock_guard<boost::mutex> lock(m_treeMutex);

         try
         {
            return m_tree.get<T> (parameterName);
         }
         catch (boost::property_tree::ptree_bad_path& e)
         {
            throw exception::CInvalidParameter(parameterName + " : " + e.what());
         }
         catch (boost::property_tree::ptree_bad_data& e)
         {
            throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
         }
      }

      //--------------------------------------------------------------
      /// \brief	    Type representing authorized enum values list
      //--------------------------------------------------------------
      typedef std::map<std::string, unsigned int> EnumValuesNames;

      //--------------------------------------------------------------
      /// \brief	    Get enum parameter value
      /// \param [in] parameterName    Name of the enum parameter
      /// \param[in]  valueNames  List of authorized values for the enum
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter value is not in the provided list
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<typename EnumType>
      EnumType getEnumValue(const std::string& parameterName, const EnumValuesNames& valuesNames) const
      {
         std::string stringValue = getValue<std::string>(parameterName);
         EnumValuesNames::const_iterator it = valuesNames.find(stringValue);
         if (it != valuesNames.end())
            return (EnumType)(it->second);

         throw exception::COutOfRange(std::string("Value ") + stringValue + " was not found for " + parameterName + " parameter");
      }

   private:
      //--------------------------------------------------------------
      /// \brief	   The configuration content
      //--------------------------------------------------------------
      boost::property_tree::ptree m_tree;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the configuration content
      //--------------------------------------------------------------
      mutable boost::mutex m_treeMutex;
   };

} } } // namespace shared::plugin::configuration
