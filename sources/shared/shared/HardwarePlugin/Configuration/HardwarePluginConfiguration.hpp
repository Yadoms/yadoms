#pragma once

#include "HardwarePluginConfigurationParameters.hpp"
#include <shared/Exceptions/BadConversionException.hpp>
#include <shared/Exceptions/OutOfRangeException.hpp>

// TODO : un fichier par classe

//--------------------------------------------------------------
/// \class Hardware plugin configuration serializer interface
//--------------------------------------------------------------
class IHardwarePluginConfigurationSerializer
{
public:
   virtual ~IHardwarePluginConfigurationSerializer() {}
   virtual const std::string serialize(const boost::property_tree::ptree& pt) const = 0;
   virtual void unserialize(const std::string& str, boost::property_tree::ptree& pt) = 0;
};

//--------------------------------------------------------------
/// \class Hardware plugin configuration JSON serializer
//--------------------------------------------------------------
class CJsonSerializer : public IHardwarePluginConfigurationSerializer
{
public:
   virtual ~CJsonSerializer() {}

   virtual const std::string serialize(const boost::property_tree::ptree& pt) const
   {
      std::ostringstream buf; 
      boost::property_tree::json_parser::write_json (buf, pt, false);
      return buf.str();
   }

   virtual void unserialize(const std::string& str, boost::property_tree::ptree& pt)
   {
      if (str.empty())
         return;

      std::istringstream is(str);
      boost::property_tree::json_parser::read_json(is, pt);
   }
};

//--------------------------------------------------------------
/// \class Hardware plugin configuration
//--------------------------------------------------------------
class CHardwarePluginConfiguration
{
public:
   typedef std::map<std::string, boost::shared_ptr<CHardwarePluginConfigurationParameter> > CHardwarePluginConfigurationMap;

public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CHardwarePluginConfiguration()
      :m_configurationSerializer(new CJsonSerializer)
   {

   }

   //--------------------------------------------------------------
   /// \brief	    Copy constructor
   /// \note       All parameters are also copied
   //--------------------------------------------------------------
   CHardwarePluginConfiguration(const CHardwarePluginConfiguration& src)
      :m_configurationSerializer(src.m_configurationSerializer)
   {
      // Full copy of parameters
      for (CHardwarePluginConfigurationMap::const_iterator it=src.m_configurationMap.begin() ; it!=src.m_configurationMap.end() ; ++it)
      {
         boost::shared_ptr<CHardwarePluginConfigurationParameter> copiedParameter = ((*it).second)->clone();
         AddParameter(copiedParameter);
      }
   }

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginConfiguration(){}

   //--------------------------------------------------------------
   /// \brief	    Add a parameter in the configuration
   /// \param [in] Pointer on the new parameter
   //--------------------------------------------------------------
   void AddParameter(boost::shared_ptr<CHardwarePluginConfigurationParameter> parameter)
   {
      BOOST_ASSERT(m_configurationMap.find(parameter->getName()) == m_configurationMap.end());  // Item already exists
      m_configurationMap[parameter->getName()] = parameter;
   }

   //--------------------------------------------------------------
   /// \brief	    Unserialize values from Json string
   /// \param [in] json string (from Yadoms database)
   //--------------------------------------------------------------
   void unserializeValues(const std::string& json)
   {
      boost::property_tree::ptree pt;
      m_configurationSerializer->unserialize(json, pt);
      for (CHardwarePluginConfigurationMap::iterator it=m_configurationMap.begin() ; it!=m_configurationMap.end() ; ++it)
      {
         try
         {
            ((boost::shared_ptr<CHardwarePluginConfigurationParameter>)((*it).second))->load(pt);
         }
         catch (boost::property_tree::ptree_bad_path&)
         {
            // If parameter was added since last plugin version, it has no value in the database.
            // So ignore it to let its default value.
         }
      }
   }

   //--------------------------------------------------------------
   /// \brief	    Serialize values to Json string
   /// \return     Json string containing configuration values
   //--------------------------------------------------------------
   const std::string serializeValues() const
   {
      boost::property_tree::ptree pt;
      for (CHardwarePluginConfigurationMap::const_iterator it=m_configurationMap.begin() ; it!=m_configurationMap.end() ; ++it)
         ((boost::shared_ptr<CHardwarePluginConfigurationParameter>)((*it).second))->save(pt);
      return m_configurationSerializer->serialize(pt);
   }

   //--------------------------------------------------------------
   /// \brief	    Direct const access to parameter
   /// \param [in] parameterName    The parameter name
   /// \return     Const reference on the expected parameter
   //--------------------------------------------------------------
   const CHardwarePluginConfigurationParameter& operator[](const std::string parameterName) const
   {
      return *m_configurationMap.at(parameterName);
   }

   //--------------------------------------------------------------
   /// \brief	    Get the parameters map
   /// \return     Return const reference to the parameters map to iterate over
   //--------------------------------------------------------------
   const CHardwarePluginConfigurationMap& getMap() const
   {
      return m_configurationMap;
   }

   //--------------------------------------------------------------
   /// \brief	    Get parameter value as bool
   /// \param [in] parameterName    Name of the parameter
   /// \return     Parameter value
   /// \throw      CBadConversionException if parameter is not bool
   //--------------------------------------------------------------
   bool asBool(const std::string parameterName) const
   {
      try
      {
         return (dynamic_cast<const CHardwarePluginConfigurationBoolParameter&>
            (*m_configurationMap.at(parameterName))).get();
      }
      catch (std::bad_cast&)
      {
      	throw CBadConversionException(parameterName, "bool");
      }
   }

   //--------------------------------------------------------------
   /// \brief	    Get parameter value as string
   /// \param [in] parameterName    Name of the parameter
   /// \return     Parameter value
   /// \throw      CBadConversionException if parameter is not string
   //--------------------------------------------------------------
   const std::string& asString(const std::string parameterName) const
   {
      try
      {
         return (dynamic_cast<const CHardwarePluginConfigurationStringParameter&>
            (*m_configurationMap.at(parameterName))).get();
      }
      catch (std::bad_cast&)
      {
         throw CBadConversionException(parameterName, "bool");
      }
   }

   //--------------------------------------------------------------
   /// \brief	    Get parameter value as serial port
   /// \param [in] parameterName    Name of the parameter
   /// \return     Parameter value
   /// \throw      CBadConversionException if parameter is not a serial port
   //--------------------------------------------------------------
   const std::string& asSerialPort(const std::string parameterName) const
   {
      try
      {
         return (dynamic_cast<const CHardwarePluginConfigurationSerialPortParameter&>
            (*m_configurationMap.at(parameterName))).get();
      }
      catch (std::bad_cast&)
      {
         throw CBadConversionException(parameterName, "bool");
      }
   }

   //--------------------------------------------------------------
   /// \brief	    Get parameter value as enum
   /// \param [in] parameterName    Name of the parameter
   /// \return     Parameter value
   /// \throw      CBadConversionException if parameter is not the expected enum type
   //--------------------------------------------------------------
   template<typename Enum>
   Enum asEnum(const std::string parameterName) const
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
   /// \brief	    Change parameter value
   /// \param [in] parameterName    Name of the parameter
   /// \param [in] valueAsString    New value (as string)
   /// \throw      std::out_of_range if value is not convertible to parameter type
   //--------------------------------------------------------------
   void set(const std::string parameterName, const std::string& valueAsString)
   {
      try
      {
	      m_configurationMap[parameterName]->valueFromString(valueAsString);
      }
      catch (std::out_of_range& e)
      {
         throw COutOfRangeException(e.what());      	
      }
   }


private:
   CHardwarePluginConfigurationMap m_configurationMap;
   boost::shared_ptr<IHardwarePluginConfigurationSerializer> m_configurationSerializer;
};

#define ADD_CONFIGURATION_PARAMETER_BASE(type,name,description,defaultValue)                                      \
   {                                                                                                              \
      boost::shared_ptr<type> p(new type(name,description,defaultValue));                                         \
      configuration.AddParameter(p);                                                                              \
   }

#define ADD_CONFIGURATION_PARAMETER_INT(name,description,defaultValue)                                            \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationIntParameter,name,description,defaultValue)

#define ADD_CONFIGURATION_PARAMETER_DOUBLE(name,description,defaultValue)                                         \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationDoubleParameter,name,description,defaultValue)

#define ADD_CONFIGURATION_PARAMETER_STRING(name,description,defaultValue)                                         \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationStringParameter,name,description,defaultValue)

#define ADD_CONFIGURATION_PARAMETER_BOOL(name,description,defaultValue)                                           \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationBoolParameter,name,description,defaultValue)

#define ADD_CONFIGURATION_PARAMETER_ENUM(enumType,name,description,defaultValue,valueNames)                       \
   {                                                                                                              \
      boost::shared_ptr<CHardwarePluginConfigurationEnumParameter <enumType> >                                    \
         p(new CHardwarePluginConfigurationEnumParameter<enumType>(name,description,defaultValue,valueNames));    \
      configuration.AddParameter(p);                                                                              \
   }
   
#define ADD_CONFIGURATION_PARAMETER_SERIAL_PORT(name,description,defaultValue)                                    \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationSerialPortParameter,name,description,defaultValue)

   