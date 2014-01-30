#pragma once

#include <shared/Export.h>
#include <boost/property_tree/ptree.hpp>


//--------------------------------------------------------------
/// \class Hardware plugin configuration parameter base class
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginConfigurationParameter
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] type  Parameter type
   /// \param[in] name  Parameter name
   /// \param[in] description  Parameter description
   //--------------------------------------------------------------
   CHardwarePluginConfigurationParameter(const std::string& type, const std::string& name, const std::string& description);

   //--------------------------------------------------------------
   /// \brief	    Clone method
   //--------------------------------------------------------------
   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the parameter name
   /// \return     The parameter name
   //--------------------------------------------------------------
   virtual const std::string& getName() const { return m_name; }

   //--------------------------------------------------------------
   /// \brief	    Get the parameter description
   /// \return     The parameter description
   //--------------------------------------------------------------
   virtual const std::string& getDescription() const { return m_description; }

   //--------------------------------------------------------------
   /// \brief	    Get the parameter value as string
   /// \param[in] current     Get the current value if true, or the default value if false
   /// \return     The parameter value
   //--------------------------------------------------------------
   virtual const std::string valueToString(bool current = true) const = 0;

   //--------------------------------------------------------------
   /// \brief	    Set the parameter value
   /// \param[in] valueAsString     New parameter value, as string
   //--------------------------------------------------------------
   virtual void valueFromString(const std::string& valueAsString) = 0;

   //--------------------------------------------------------------
   /// \brief	    Set the parameter value
   /// \param[in] pt     Property-tree containing the new value
   //--------------------------------------------------------------
   virtual void setValue(const boost::property_tree::ptree& pt);

   //--------------------------------------------------------------
   /// \brief	    Get the parameter schema
   /// \param[out] pt   Property-tree containing the schema
   //--------------------------------------------------------------
   virtual void getSchema(boost::property_tree::ptree& pt) const;

private:
   //--------------------------------------------------------------
   /// \brief	    The parameter type
   //--------------------------------------------------------------
   std::string m_type;

   //--------------------------------------------------------------
   /// \brief	    The parameter name
   //--------------------------------------------------------------
   std::string m_name;

   //--------------------------------------------------------------
   /// \brief	    The parameter description
   //--------------------------------------------------------------
   std::string m_description;
};