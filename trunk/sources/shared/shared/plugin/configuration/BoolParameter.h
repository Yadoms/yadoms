#pragma once

#include <shared/Export.h>
#include "SimpleParameter.hpp"

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \class Plugin configuration bool parameter
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CBoolParameter : public CSimpleParameter<bool>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] name  Parameter name
      /// \param[in] description  Parameter description
      /// \param[in] defaultValue  Parameter default value
      //--------------------------------------------------------------
      CBoolParameter(const std::string& name, const std::string& description, bool defaultValue);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CBoolParameter() {}

      // CParameter implementation
      virtual boost::shared_ptr<CParameter> clone() const;
      // [END] CParameter implementation
   };

   //--------------------------------------------------------------
   /// \brief	    Macro helper to get the parameter value
   /// \param paramName    Parameter name
   //--------------------------------------------------------------
   #define CFG_GET_BOOL(paramName) getParam<shared::plugin::configuration::CBoolParameter>(paramName).get()

} } } // namespace shared::plugin::configuration
