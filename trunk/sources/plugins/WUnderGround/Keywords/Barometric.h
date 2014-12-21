#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace WUCapacities
{
   //--------------------------------------------------------------
   /// \brief	The Barometric protocol support (reception only)
   //--------------------------------------------------------------
   class CBarometric : public IKeyword::IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CBarometric(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CBarometric();

	  void GetValue(double temp);
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
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_PluginName;

      //--------------------------------------------------------------
      /// \brief	The device model
      //--------------------------------------------------------------
      std::string m_deviceModel;

      //--------------------------------------------------------------
      /// \brief	The pressure (hPa)
      //--------------------------------------------------------------
      yApi::historization::CPressure m_pressure;
   };
} // namespace WUCapacities
