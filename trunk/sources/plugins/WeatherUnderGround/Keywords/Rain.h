#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

   //--------------------------------------------------------------
   /// \brief	The UV Keyword
   //--------------------------------------------------------------
   class CRain : public IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] PluginName           The name of the plugin
      /// \param[in] KeyWordName          The keyword name
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CRain(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName);

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
