#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

   //--------------------------------------------------------------
   /// \brief	The Wind Keyword
   //--------------------------------------------------------------
   class CWindSpeed : public IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] PluginName           The name of the plugin
	  /// \param[in] Name                 The keyword name
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CWindSpeed( std::string PluginName, std::string Name );

	  //--------------------------------------------------------------
     /// \brief	                         Set the value from the container
     /// \param[in] ValueContainer          The container where the value is stored
	  /// \param[in] filterWindDirection     The name of the first information into the container
	  /// \param[in] filterWindAverageSpeed  The name of the second information into the container
	  /// \param[in] filterWindMaxSpeed      The name of the third information into the container
     /// \note                              Use this constructor initialising the keyword for this plugin
     //--------------------------------------------------------------
	  void SetValue(const shared::CDataContainer & ValueContainer, const std::string & filterWindDirection);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CWindSpeed();

      // IKeyword implementation
	  virtual void DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const;
      virtual void Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const;
	  virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const;
      // [END] IKeyword implementation
      
   protected:

   private:
      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_PluginName;

      //--------------------------------------------------------------
      /// \brief	The wind speed
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSpeed> m_windSpeed;
   };