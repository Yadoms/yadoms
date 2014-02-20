#pragma once

#include "Parameter.h"

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \class  Hardware plugin configuration templated parameter for simple types
   /// \note   To use this class, types have to support << operator
   //--------------------------------------------------------------
   template<typename T>
   class CSimpleParameter : public CParameter
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] type  Parameter type
      /// \param[in] name  Parameter name
      /// \param[in] description  Parameter description
      /// \param[in] defaultValue  Parameter default value
      //--------------------------------------------------------------
      CSimpleParameter(const std::string& type, const std::string& name, const std::string& description, const T defaultValue)
         :CParameter(type, name, description), m_defaultValue(defaultValue), m_value(defaultValue) {}

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CSimpleParameter() {}

      // Allocation must be done in the shared library memory space. So we can not do it here, or
      // allocation will occur in the caller memory space (which can be freed in the case of unload plugin)

      // CParameter implementation
      void build(boost::property_tree::ptree& pt) const
      {
         CParameter::build(pt);
         pt.put(getName() + ".default", m_defaultValue);
      }
      void setValue(const boost::property_tree::ptree& pt)
      {
         m_value = pt.get<T>(getName() + ".value");
      }
      // [END] CParameter implementation

      //--------------------------------------------------------------
      /// \brief	    Native getter
      /// \return    The current parameter value
      //--------------------------------------------------------------
      T get() const { return m_value; }

   protected:
      //--------------------------------------------------------------
      /// \brief	    The default parameter value
      //--------------------------------------------------------------
      const T m_defaultValue;

      //--------------------------------------------------------------
      /// \brief	    The current parameter value
      //--------------------------------------------------------------
      T m_value;
   };

} } } // namespace shared::plugin::configuration

