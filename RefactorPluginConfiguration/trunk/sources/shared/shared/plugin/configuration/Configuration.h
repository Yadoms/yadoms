#pragma once

#include <shared/Export.h>
#include "Parameters.h"
#include "ISchema.h"
#include "../../serialization/IPtreeToStringSerializer.h"
#include <shared/exception/BadConversion.hpp>


namespace shared { namespace plugin { namespace configuration
{

   class IConfigurationSerializer;

   //--------------------------------------------------------------
   /// \class Plugin configuration
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CConfiguration : public ISchema
   {
   public:
      typedef std::map<std::string, boost::shared_ptr<CParameter> > CParametersMap;

   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      //--------------------------------------------------------------
      CConfiguration();

      //--------------------------------------------------------------
      /// \brief	    Constructs and set values
      //--------------------------------------------------------------
      CConfiguration(const CConfiguration& src, const std::string& configurationValues);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CConfiguration();

      //--------------------------------------------------------------
      /// \brief	    Build the configuration schema
      //--------------------------------------------------------------
      void buildSchema();

      // ISchema implementation
      virtual std::string getSchema() const;
      // [END] ISchema implementation

      //--------------------------------------------------------------
      /// \brief	    Set configuration values from saved data
      /// \param[in] serializedConfiguration    String containing the configuration values (JSON format)
      //--------------------------------------------------------------
      virtual void setValues(const std::string& serializedConfiguration);

      //--------------------------------------------------------------
      /// \brief	    Add a parameter in the configuration
      /// \param [in] Pointer on the new parameter
      //--------------------------------------------------------------
      void AddParameter(boost::shared_ptr<CParameter> parameter);
      void AddParameter(CParameter* parameter);

      //--------------------------------------------------------------
      /// \brief	    Direct const access to parameter
      /// \param [in] parameterName    The parameter name
      /// \return     Const reference on the expected parameter
      //--------------------------------------------------------------
      const CParameter& operator[](const std::string& parameterName) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter
      /// \throw      exception::CBadConversion if parameter is not the expected type
      //--------------------------------------------------------------
      template<typename T>
      const T& getParam(const std::string& parameterName) const
      {
         try
         {
            return dynamic_cast<const T&> (*m_parametersMap.at(parameterName));
         }
         catch (std::bad_cast&)
         {
            throw exception::CBadConversion(parameterName, "getParam");
         }
      }

   protected:
      //--------------------------------------------------------------
      /// \brief	    Build the configuration schema
      /// \note       Plugin must override this function to describe its configuration
      //--------------------------------------------------------------
      virtual void doBuildSchema() = 0;


   private:
      //--------------------------------------------------------------
      /// \brief	   The map containing the configuration parameters
      //--------------------------------------------------------------
      CParametersMap m_parametersMap;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the configuration map
      //--------------------------------------------------------------
      mutable boost::mutex m_parametersMapMutex;

      //--------------------------------------------------------------
      /// \brief	   The serializer used for schema and values serialization
      //--------------------------------------------------------------
      boost::shared_ptr<serialization::IPtreeToStringSerializer> m_configurationSerializer;
   };

} } } // namespace shared::plugin::configuration
