#pragma once

#include <shared/Export.h>
#include "Parameters.h"
#include "../Interfaces/IHardwarePluginConfigurationSchema.h"
#include "../../Serialization/IPtreeToStringSerializer.h"
#include <shared/Exceptions/BadConversionException.hpp>


namespace shared { namespace plugin { namespace configuration
{

   class IConfigurationSerializer;

   //--------------------------------------------------------------
   /// \class Hardware plugin configuration
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CConfiguration : public IHardwarePluginConfigurationSchema
   {
   public:
      typedef std::map<std::string, boost::shared_ptr<CParameter> > CHardwarePluginConfigurationMap;

   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      //--------------------------------------------------------------
      CConfiguration();

      //--------------------------------------------------------------
      /// \brief	    Constructs and set values
      //--------------------------------------------------------------
      CConfiguration(const CConfiguration& src, const std::string& configurationValues);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CConfiguration();

      //--------------------------------------------------------------
      /// \brief	    Build the configuration schema
      //--------------------------------------------------------------
      void buildSchema();

      // IHardwarePluginConfigurationSchema implementation
      virtual std::string getSchema() const;
      // [END] IHardwarePluginConfigurationSchema implementation

      //--------------------------------------------------------------
      /// \brief	    Set configuration values from saved data
      /// \param[in] serializedConfiguration    String containing the configuration values (JSON format)
      //--------------------------------------------------------------
      virtual void setValues(const std::string& serializedConfiguration);

      //--------------------------------------------------------------
      /// \brief	    Add a parameter in the configuration
      /// \param [in] Pointer on the new parameter
      //--------------------------------------------------------------
      void AddParameter(boost::shared_ptr<CParameter> parameter);
      void AddParameter(CParameter* parameter);

      //--------------------------------------------------------------
      /// \brief	    Direct const access to parameter
      /// \param [in] parameterName    The parameter name
      /// \return     Const reference on the expected parameter
      //--------------------------------------------------------------
      const CParameter& operator[](const std::string& parameterName) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter
      /// \throw      CBadConversionException if parameter is not the expected type
      //--------------------------------------------------------------
      template<typename T>
      const T& getParam(const std::string& parameterName) const
      {
         try
         {
            return dynamic_cast<const T&> (*m_configurationMap.at(parameterName));
         }
         catch (std::bad_cast&)
         {
            throw CBadConversionException(parameterName, "getParam");
         }
      }

   protected:
      //--------------------------------------------------------------
      /// \brief	    Build the configuration schema
      /// \note       Plugin must override this function to describe its configuration
      //--------------------------------------------------------------
      virtual void doBuildSchema() = 0;


   private:
      //--------------------------------------------------------------
      /// \brief	   The map containing the configuration parameters
      //--------------------------------------------------------------
      CHardwarePluginConfigurationMap m_configurationMap;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the configuration map
      //--------------------------------------------------------------
      mutable boost::mutex m_configurationMapMutex;

      //--------------------------------------------------------------
      /// \brief	   The serializer used for schema and values serialization
      //--------------------------------------------------------------
      boost::shared_ptr<IPtreeToStringSerializer> m_configurationSerializer;
   };

} } } // namespace shared::plugin::configuration
