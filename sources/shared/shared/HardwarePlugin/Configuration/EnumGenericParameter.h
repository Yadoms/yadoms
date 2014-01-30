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

   //--------------------------------------------------------------
   /// \brief	    Get the parameter schema
   /// \param[out] pt   Property-tree containing the schema
   //--------------------------------------------------------------
   virtual void getSchema(boost::property_tree::ptree& pt) const;

protected:
   //--------------------------------------------------------------
   /// \brief	    List of authorized values for the enum
   //--------------------------------------------------------------
   const ValuesNames m_valuesNames;
};
