#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

   //--------------------------------------------------------------
   /// \brief	The Load class
   //--------------------------------------------------------------
   class CCurrentMeter : public IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
	   /// \param[in] PluginName           The name of the plugin
	   /// \param[in] KeyWordName          The keyword name
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CCurrentMeter( boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName );

      //--------------------------------------------------------------
      /// \brief	                      Set the value of the keyword
      /// \param[in] Value                The new value
      //--------------------------------------------------------------
	  void SetValue ( double Value );

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CCurrentMeter();

      // IKeywords implementation
	  virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const;
      // [END] IKeywords implementation
      
   protected:

   private:
      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_PluginName;

      //--------------------------------------------------------------
      /// \brief	The current (A)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CCurrent> m_current;
   };
