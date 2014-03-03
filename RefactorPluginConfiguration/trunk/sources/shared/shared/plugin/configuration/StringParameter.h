#pragma once

#include <shared/Export.h>
#include "Parameter.h"

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \class Plugin configuration string parameter
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CStringParameter : public CParameter
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] name  Parameter name
      /// \param[in] description  Parameter description
      /// \param[in] defaultValue  Parameter default value
      //--------------------------------------------------------------
      CStringParameter(const std::string& name, const std::string& description, const std::string& defaultValue);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CStringParameter() {}

      // CParameter implementation
      virtual boost::shared_ptr<CParameter> clone() const;
      virtual void build(boost::property_tree::ptree& pt) const;
      virtual void setValue(const boost::property_tree::ptree& pt);
      // [END] CParameter implementation

      //--------------------------------------------------------------
      /// \brief	    Native getter
      /// \return    The current parameter value
      //--------------------------------------------------------------
      const std::string& get() const;

   private:
      //--------------------------------------------------------------
      /// \brief	    The default parameter value
      //--------------------------------------------------------------
      const std::string m_defaultValue;

      //--------------------------------------------------------------
      /// \brief	    The current parameter value
      //--------------------------------------------------------------
      std::string m_value;
   };

   //--------------------------------------------------------------
   /// \brief	    Macro helper to get the parameter value
   /// \param paramName    Parameter name
   //--------------------------------------------------------------
   #define CFG_GET_STRING(paramName) getParam<shared::plugin::configuration::CStringParameter>(paramName).get()

} } } // namespace shared::plugin::configuration
