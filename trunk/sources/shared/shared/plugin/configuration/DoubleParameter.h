#pragma once

#include <shared/Export.h>
#include "SimpleParameter.hpp"

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \type Plugin configuration double parameter
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CDoubleParameter : public CSimpleParameter<double>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] name  Parameter name
      /// \param[in] description  Parameter description
      /// \param[in] defaultValue  Parameter default value
      //--------------------------------------------------------------
      CDoubleParameter(const std::string& name, const std::string& description, double defaultValue);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CDoubleParameter() {}

      // CParameter implementation
      virtual boost::shared_ptr<CParameter> clone() const;
      // [END] CParameter implementation
   };

   //--------------------------------------------------------------
   /// \brief	    Macro helper to get the parameter value
   /// \param paramName    Parameter name
   //--------------------------------------------------------------
   #define CFG_GET_DOUBLE(paramName) getParam<shared::plugin::configuration::CDoubleParameter>(paramName).get()

} } } // namespace shared::plugin::configuration
