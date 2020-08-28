#pragma once

#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class CConfiguration
{
public:
	//--------------------------------------------------------------
	/// \brief	    Destructor
	//--------------------------------------------------------------
	virtual ~CConfiguration();

	//--------------------------------------------------------------
	/// \brief		   Load configuration data
	/// \param [in] data The data container
	//--------------------------------------------------------------
	void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data);

	//--------------------------------------------------------------
	/// \brief	    Just for test, not needed for real plugin
	//--------------------------------------------------------------
	void trace() const;

	//--------------------------------------------------------------
	//--------------------------------------------------------------
	// You can add your own accessors for better code readability
	//--------------------------------------------------------------
	//--------------------------------------------------------------

	std::string getUsbDevice() const;



private:
	//--------------------------------------------------------------
	/// \brief	    Configuration getter
	//--------------------------------------------------------------
	yApi::YPluginConfiguration m_configuration;
};
