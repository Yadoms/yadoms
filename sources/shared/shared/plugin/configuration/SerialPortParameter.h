#pragma once

#include <shared/Export.h>
#include "EnumGenericParameter.h"

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \class Hardware plugin configuration serial port parameter
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CSerialPortParameter : public CEnumGeneric
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] name  Parameter name
      /// \param[in] description  Parameter description
      //--------------------------------------------------------------
      CSerialPortParameter(const std::string& name, const std::string& description);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CSerialPortParameter() {}

      // CParameter implementation
      virtual boost::shared_ptr<CParameter> clone() const;
      virtual void setValue(const boost::property_tree::ptree& pt);
      // [END] CParameter implementation

      // CEnumGeneric override
      virtual void build(boost::property_tree::ptree& pt) const;
      // [END] CEnumGeneric override

      //--------------------------------------------------------------
      /// \brief	    Native getter
      /// \return    The current parameter value
      //--------------------------------------------------------------
      const std::string& get() const;

   private:
      //--------------------------------------------------------------
      /// \brief	    The current parameter value
      //--------------------------------------------------------------
      std::string m_value;

      //--------------------------------------------------------------
      /// \brief	    The current parameter value
      //--------------------------------------------------------------
      const std::vector<std::string> m_valueNames;
   };

   //--------------------------------------------------------------
   /// \brief	    Macro helper to get the parameter value
   /// \param paramName    Parameter name
   //--------------------------------------------------------------
   #define CFG_GET_SERIAL_PORT(paramName) getParam<shared::plugin::configuration::CSerialPortParameter>(paramName).get()

} } } // namespace shared::plugin::configuration
