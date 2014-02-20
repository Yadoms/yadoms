#pragma once

#include <shared/Export.h>
#include "Parameter.h"

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \class Enum parameter, generic class
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CEnumGeneric : public CParameter
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
      CEnumGeneric(const std::string& name, const std::string& description, const ValuesNames& valuesNames);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CEnumGeneric();

      // CParameter implementation
      virtual void build(boost::property_tree::ptree& pt) const;
      // [END] CParameter implementation

   protected:
      //--------------------------------------------------------------
      /// \brief	    List of authorized values for the enum
      //--------------------------------------------------------------
      const ValuesNames m_valuesNames;
   };

} } } // namespace shared::plugin::configuration
