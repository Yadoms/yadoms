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
   virtual ~ILoad() {};

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYadomsApi> context) = 0;
   virtual void read() = 0;
   virtual void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const = 0;
   // [END] ILoad Implementation
};

