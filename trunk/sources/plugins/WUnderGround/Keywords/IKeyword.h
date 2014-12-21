#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace IKeyword
{
   //--------------------------------------------------------------
   /// \brief	The KeyWord Interface interface
   //--------------------------------------------------------------
   class IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IKeyword() {}

	  virtual void DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const =0;

      //--------------------------------------------------------------
      /// \brief	                        Historize message data to Yadoms
      /// \param[in] context              Yadoms APi context
      //--------------------------------------------------------------
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const = 0;
   };
}
