#pragma once

#include <shared/Export.h>
#include "Parameter.h"

//--------------------------------------------------------------
/// \class Hardware plugin configuration booleans list parameter
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginConfigurationBitsFieldParameter : public CHardwarePluginConfigurationParameter
{
public:
   //--------------------------------------------------------------
   /// \brief	    Type representing authorized enum values list
   //--------------------------------------------------------------
   typedef std::map<std::string, bool> Items;

public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] name  Parameter name
   /// \param[in] description  Parameter description
   /// \param[in] items  items list (with theirs default value)
   //--------------------------------------------------------------
   CHardwarePluginConfigurationBitsFieldParameter(const std::string& name, const std::string& description, const Items& items);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   ~CHardwarePluginConfigurationBitsFieldParameter() {}

   // CHardwarePluginConfigurationParameter implementation
   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const;
   virtual const std::string valueToString(bool current) const;
   virtual void valueFromString(const std::string& valueAsString);
   virtual void setValue(const boost::property_tree::ptree& pt);
   virtual void getSchema(boost::property_tree::ptree& pt) const;
   // [END] CHardwarePluginConfigurationParameter implementation

   //--------------------------------------------------------------
   /// \brief	    Native getter
   /// \return    The current bits field values
   //--------------------------------------------------------------
   Items get() const { return m_value; }


private:
   //--------------------------------------------------------------
   /// \brief	    The default parameter value
   //--------------------------------------------------------------
   const Items m_defaultValue;

   //--------------------------------------------------------------
   /// \brief	    The current parameter value
   //--------------------------------------------------------------
   Items m_value;
};
