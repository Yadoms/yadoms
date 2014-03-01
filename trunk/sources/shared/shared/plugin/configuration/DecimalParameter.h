#pragma once

#include <shared/Export.h>
#include "SimpleParameter.hpp"

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \type Plugin configuration decimal parameter
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CDecimalParameter : public CSimpleParameter<double>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] name  Parameter name
      /// \param[in] description  Parameter description
      /// \param[in] defaultValue  Parameter default value
      //--------------------------------------------------------------
      CDecimalParameter(const std::string& name, const std::string& description, double defaultValue);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CDecimalParameter() {}

      // CParameter implementation
      virtual boost::shared_ptr<CParameter> clone() const;
      // [END] CParameter implementation
   };

   //--------------------------------------------------------------
   /// \brief	    Macro helper to get the parameter value
   /// \param paramName    Parameter name
   //--------------------------------------------------------------
   #define CFG_GET_DECIMAL(paramName) getParam<shared::plugin::configuration::CDecimalParameter>(paramName).get()

} } } // namespace shared::plugin::configuration
