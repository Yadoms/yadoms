#pragma once

#include <shared/Export.h>
#include "Parameters.h"
#include "../Interfaces/IHardwarePluginConfigurationSchema.h"
#include "../../Serialization/IPtreeToStringSerializer.h"
#include <shared/Exceptions/BadConversionException.hpp>

class IConfigurationSerializer;

//--------------------------------------------------------------
/// \class Hardware plugin configuration
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginConfiguration : public IHardwarePluginConfigurationSchema
{
public:
   typedef std::map<std::string, boost::shared_ptr<CHardwarePluginConfigurationParameter> > CHardwarePluginConfigurationMap;

public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CHardwarePluginConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Constructs and set values
   //--------------------------------------------------------------
   CHardwarePluginConfiguration(const CHardwarePluginConfiguration& src, const std::string& configurationValues);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginConfiguration();

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
   void AddParameter(boost::shared_ptr<CHardwarePluginConfigurationParameter> parameter);
   void AddParameter(CHardwarePluginConfigurationParameter* parameter);

   //--------------------------------------------------------------
   /// \brief	    Direct const access to parameter
   /// \param [in] parameterName    The parameter name
   /// \return     Const reference on the expected parameter
   //--------------------------------------------------------------
   const CHardwarePluginConfigurationParameter& operator[](const std::string& parameterName) const;

   //--------------------------------------------------------------
   /// \brief	    Get parameter value as bool
   /// \param [in] parameterName    Name of the parameter
   /// \return     Parameter value
   /// \throw      CBadConversionException if parameter is not bool
   //--------------------------------------------------------------
   bool asBool(const std::string& parameterName) const;

   //--------------------------------------------------------------
   /// \brief	    Get parameter value as string
   /// \param [in] parameterName    Name of the parameter
   /// \return     Parameter value
   /// \throw      CBadConversionException if parameter is not string
   //--------------------------------------------------------------
   const std::string& asString(const std::string& parameterName) const;

   //--------------------------------------------------------------
   /// \brief	    Get parameter value as serial port
   /// \param [in] parameterName    Name of the parameter
   /// \return     Parameter value
   /// \throw      CBadConversionException if parameter is not a serial port
   //--------------------------------------------------------------
   const std::string& asSerialPort(const std::string& parameterName) const;

   //--------------------------------------------------------------
   /// \brief	    Get parameter value as enum
   /// \param [in] parameterName    Name of the parameter
   /// \return     Parameter value
   /// \throw      CBadConversionException if parameter is not the expected enum type
   //--------------------------------------------------------------
   template<typename Enum>
   Enum asEnum(const std::string& parameterName) const
   {
      try
      {
         return (dynamic_cast<const CHardwarePluginConfigurationEnumParameter<Enum>&>
            (*m_configurationMap.at(parameterName))).get();
      }
      catch (std::bad_cast&)
      {
         throw CBadConversionException(parameterName, "bool");
      }
   }

   //--------------------------------------------------------------
   /// \brief	    Get parameter value as bool, from a bits field
   /// \param [in] parameterName    Name of the parameter
   /// \return     Bits field map
   /// \throw      CBadConversionException if parameter is not bool
   //--------------------------------------------------------------
   CHardwarePluginConfigurationBitsFieldParameter::Items asBitsField(const std::string& parameterName) const;

   // TODO voir si on peut faire des accesseurs template. L'appel donnerait qqc du genre :
   //m_Configuration.get<bool>("BoolParameter");
   //TODO commenter (sans oublier le throw)
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
   CHardwarePluginConfigurationMap m_configurationMap;
   boost::mutex m_configurationMapMutex;
   boost::shared_ptr<IPtreeToStringSerializer> m_configurationSerializer;
};

