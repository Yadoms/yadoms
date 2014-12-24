#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace WUCapacities
{
   //--------------------------------------------------------------
   /// \brief	The UV Keyword
   //--------------------------------------------------------------
   class CRain : public IKeyword::IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] PluginName           The name of the plugin
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CRain(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName);

	  void GetValue(double temp);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CRain();

      // IKeyword implementation
	  virtual void DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
      // [END] IKeyword implementation
      
   protected:

   private:
      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_PluginName;

      //--------------------------------------------------------------
      /// \brief	The UV index
      //--------------------------------------------------------------
      yApi::historization::CRain m_rain;
   };
} // namespace WUCapacities
