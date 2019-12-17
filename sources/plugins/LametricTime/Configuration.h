#pragma once

#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

enum EPortType
{
	kHttp = 8080,
	kHttps = 4343
};

enum EPairingMode
{
	kAuto = 0,
	kManual
};

//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class CConfiguration
{
public:
	//--------------------------------------------------------------
	/// \brief	    Destructor
	//--------------------------------------------------------------
	~CConfiguration() = default;

	//--------------------------------------------------------------
	/// \brief		   Load configuration data
	/// \param [in]    Data The data container
	//--------------------------------------------------------------
	void initializeWith(const shared::CDataContainer& data);

	//--------------------------------------------------------------
	/// \brief	    Just for test, not needed for real plugin
	//--------------------------------------------------------------
	void trace() const;
	/**
	 * \brief  Get Device IP address
	 * \return Returns Device IP address
	 */
	std::string getIPAddress() const;

	/**
	 * \brief  Get port
	 * \return Returns port
	 */
	EPortType getPort() const;
	/**
	 * \brief  Get Device Api Key 
	 * \return Returns Device Api Key
	 */
	std::string getAPIKey() const;
	/**
	 * \brief  Set Device IP address
	 * \param[in] Device IP Address
	* \return Returns Device IP address
	*/
	void setIPAddress(std::string& ipAddress);
	/**
	* \brief  get pairing mode
	* \return Returns pairing mode
	*/
	EPairingMode getPairingMode() const;
private:
	//--------------------------------------------------------------
	/// \brief	    Configuration getter
	//--------------------------------------------------------------
	yApi::YPluginConfiguration m_configuration;
	std::string m_ipAddress;
};
