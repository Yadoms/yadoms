#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

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

//--------------------------------------------------------------
/// \brief	                      Declaration of Keyword(s)
/// \param[in] context              Yadoms APi context
//--------------------------------------------------------------
virtual void DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const =0;

//--------------------------------------------------------------
/// \brief	                      Initialize the keyword (Declaration)
/// \param[in] context              Yadoms APi context
//--------------------------------------------------------------
virtual void Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const = 0;

//--------------------------------------------------------------
/// \brief	                      Get the IHistorizable variable
/// \param[in] context              Yadoms APi context
//--------------------------------------------------------------
virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const = 0;
};