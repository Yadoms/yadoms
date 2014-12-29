#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IKeyword.h"

namespace yApi = shared::plugin::yPluginApi;

   //--------------------------------------------------------------
   /// \brief	The Temp capacities
   //--------------------------------------------------------------
   class CTemp : public IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] PluginName           The name of the plugin
      /// \param[in] KeyWordName          The keyword name
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CTemp::CTemp(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName);

      //--------------------------------------------------------------
      /// \brief	                      Set the value from the container
      /// \param[in] ValueContainer       The container where the value is stored
	  /// \param[in] filter               The name of the information into the container
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
	  void SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CTemp();

      // ICapacities implementation
	  virtual void DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
      // [END] ICapacities implementation
      
   protected:

   private:

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_PluginName;

      //--------------------------------------------------------------
      /// \brief	The temperature (°C)
      //--------------------------------------------------------------
      yApi::historization::CTemperature m_temperature;

   };