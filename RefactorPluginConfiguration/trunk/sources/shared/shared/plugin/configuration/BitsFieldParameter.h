#pragma once

#include <shared/Export.h>
#include "Parameter.h"

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \class Plugin configuration booleans list parameter
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CBitsFieldParameter : public CParameter
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
      CBitsFieldParameter(const std::string& name, const std::string& description, const Items& items);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CBitsFieldParameter() {}

      // CParameter implementation
      virtual boost::shared_ptr<CParameter> clone() const;
      virtual void setValue(const boost::property_tree::ptree& pt);
      virtual void build(boost::property_tree::ptree& pt) const;
      // [END] CParameter implementation

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

   //--------------------------------------------------------------
   /// \brief	    Macro helper to get the parameter value
   /// \param paramName    Parameter name
   //--------------------------------------------------------------
   #define CFG_GET_BITS_FIELD(paramName) getParam<shared::plugin::configuration::CBitsFieldParameter>(paramName).get()

} } } // namespace shared::plugin::configuration
