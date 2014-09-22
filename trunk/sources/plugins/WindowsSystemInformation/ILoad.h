#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	Interface Load definition
/// \note   return the load in pourcentage of a value
//--------------------------------------------------------------
class ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ILoad() {}

   //--------------------------------------------------------------
   /// \brief	            Declare the keywords
   /// \param[in] context  YadomsApi context
   //--------------------------------------------------------------
   virtual void declareKeywords(boost::shared_ptr<yApi::IYadomsApi> context) = 0;

   //--------------------------------------------------------------
   /// \brief	    Read actual value
   //--------------------------------------------------------------
   virtual void read() = 0;

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] context  YadomsApi context to which historize data
   //--------------------------------------------------------------
   virtual void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const = 0;
};

