#pragma once

#include <shared/Export.h>
#include "Parameters.h"
#include "../../serialization/IPtreeToStringSerializer.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/BadConversion.hpp>


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
      /// \throw      shared::exception::CBadConversion if parameter is not the expected type
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<typename T>
      T getValue(const std::string& parameterName) const
      {
         boost::lock_guard<boost::mutex> lock(m_treeMutex);

         try
         {
            return m_tree.get<T> (parameterName + ".value");
         }
         catch (boost::property_tree::ptree_bad_path& e)
         {
            throw exception::CInvalidParameter(parameterName + " : " + e.what());
         }
         catch (boost::property_tree::ptree_bad_data& e)
         {
            throw exception::CBadConversion(parameterName, std::string("getParam : ") + e.what());
         }
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

      //--------------------------------------------------------------
      /// \brief	   The serializer used for schema and values serialization
      //--------------------------------------------------------------
      boost::shared_ptr<serialization::IPtreeToStringSerializer> m_configurationSerializer;
   };

} } } // namespace shared::plugin::configuration
