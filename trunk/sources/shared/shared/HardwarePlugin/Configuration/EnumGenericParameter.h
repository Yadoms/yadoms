#pragma once

#include <shared/Export.h>
#include "Parameter.h"

//--------------------------------------------------------------
/// \class Enum parameter, generic class
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginConfigurationEnumGeneric : public CHardwarePluginConfigurationParameter
{
public:
   //--------------------------------------------------------------
   /// \brief	    Type representing authorized enum values list
   //--------------------------------------------------------------
   typedef std::map<unsigned int, std::string> ValuesNames;

public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] name  Parameter name
   /// \param[in] description Parameter description
   /// \param[in] valueNames  List of authorized values for the enum
   //--------------------------------------------------------------
   CHardwarePluginConfigurationEnumGeneric(const std::string& name, const std::string& description, const ValuesNames& valuesNames);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginConfigurationEnumGeneric();

   // CHardwarePluginConfigurationParameter implementation
   virtual void build(boost::property_tree::ptree& pt) const;
   // [END] CHardwarePluginConfigurationParameter implementation

protected:
   //--------------------------------------------------------------
   /// \brief	    List of authorized values for the enum
   //--------------------------------------------------------------
   const ValuesNames m_valuesNames;
};
