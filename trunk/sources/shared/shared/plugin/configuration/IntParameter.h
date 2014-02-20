#pragma once

#include <shared/Export.h>
#include "SimpleParameter.hpp"

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \type Hardware plugin configuration int parameter
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CIntParameter : public CSimpleParameter<int>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] name  Parameter name
      /// \param[in] description  Parameter description
      /// \param[in] defaultValue  Parameter default value
      //--------------------------------------------------------------
      CIntParameter(const std::string& name, const std::string& description, int defaultValue);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CIntParameter() {}

      // CParameter implementation
      virtual boost::shared_ptr<CParameter> clone() const;
      // [END] CParameter implementation
   };

   //--------------------------------------------------------------
   /// \brief	    Macro helper to get the parameter value
   /// \param paramName    Parameter name
   //--------------------------------------------------------------
   #define CFG_GET_INT(paramName) getParam<shared::plugin::configuration::CIntParameter>(paramName).get()

} } } // namespace shared::plugin::configuration
