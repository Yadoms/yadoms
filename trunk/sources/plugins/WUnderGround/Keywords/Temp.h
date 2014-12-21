#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IKeyword.h"

namespace yApi = shared::plugin::yPluginApi;

namespace WUCapacities
{
   //--------------------------------------------------------------
   /// \brief	The Temp capacities
   //--------------------------------------------------------------
   class CTemp : public IKeyword::IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CTemp::CTemp(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName, std::string Unit);

	  void GetValue(double temp);
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CTemp();

      // ICapacities implementation
	  virtual void DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
      //virtual const std::string& getDeviceName() const;
      // [END] ICapacities implementation
      
   protected:
      //--------------------------------------------------------------
      /// \brief	Global initialization method
      /// \param[in] context              Yadoms APi context
      //--------------------------------------------------------------
      //void Init(boost::shared_ptr<yApi::IYPluginApi> context);

      //--------------------------------------------------------------
      /// \brief	                        Build the device name
      //--------------------------------------------------------------
      //void buildDeviceName();

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
} // namespace WUCapacities
