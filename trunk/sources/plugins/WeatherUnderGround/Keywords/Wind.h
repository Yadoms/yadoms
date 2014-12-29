#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

   //--------------------------------------------------------------
   /// \brief	The UV Keyword
   //--------------------------------------------------------------
   class CWind : public IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] PluginName           The name of the plugin
	  /// \param[in] Prefix               The Prefexi before the keyword name
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CWind(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string Prefix);

	  //--------------------------------------------------------------
      /// \brief	                         Set the value from the container
      /// \param[in] ValueContainer          The container where the value is stored
	  /// \param[in] filterWindDirection     The name of the first information into the container
	  /// \param[in] filterWindAverageSpeed  The name of the second information into the container
	  /// \param[in] filterWindMaxSpeed      The name of the third information into the container
      /// \note                              Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
	  void SetValue(const shared::CDataContainer & ValueContainer, const std::string & filterWindDirection, const std::string & filterWindAverageSpeed ="", const std::string & filterWindMaxSpeed ="");

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CWind();

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
      /// \brief	The wind direction
      //--------------------------------------------------------------
      yApi::historization::CDirection m_windDirection;

      //--------------------------------------------------------------
      /// \brief	The wind average speed
      //--------------------------------------------------------------
      yApi::historization::CSpeed m_windAverageSpeed;

      //--------------------------------------------------------------
      /// \brief	The wind max speed
      //--------------------------------------------------------------
      yApi::historization::CSpeed m_windMaxSpeed;
   };