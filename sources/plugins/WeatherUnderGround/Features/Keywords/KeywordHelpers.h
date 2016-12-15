#pragma once
#include "shared/DataContainer.h"

//--------------------------------------------------------------
/// \brief convert a std::string from a CDataContainer to a double
/// \param[out] value                the final double value
/// \param[in]  valueContainer       The container where to find the parameter with the name "label"
/// \param[in]  label                The name of the parameter whe need
/// \return                          true is the conversion is ok
//--------------------------------------------------------------
bool convertDouble(double& value, const shared::CDataContainer& valueContainer, const std::string& label);

//--------------------------------------------------------------
/// \brief convert a std::string from a CDataContainer to a int
/// \param[out] value                the final int value
/// \param[in]  valueContainer       The container where to find the parameter with the name "label"
/// \param[in]  label                The name of the parameter whe need
/// \return                          true is the conversion is ok
//--------------------------------------------------------------
bool convertInt(int& value, const shared::CDataContainer& valueContainer, const std::string& label);

//--------------------------------------------------------------
/// \brief	returnString = "--" if the value is not a number
//--------------------------------------------------------------

//--------------------------------------------------------------
/// \brief  convert Km/h to m/s
/// \brief	returnString = "--" if the value is not a number
/// \param[out] returnString         the final string value
/// \param[in]  valueContainer       The container where to find the parameter with the name "label"
/// \param[in]  label                The name of the parameter whe need
//--------------------------------------------------------------
void convertKmhToMs(std::string& returnString, const shared::CDataContainer& valueContainer, const std::string& label);

//--------------------------------------------------------------
/// \brief  convert cm to mm
/// \brief	returnString = "--" if the value is not a number
/// \param[out] returnString         the final string value
/// \param[in]  valueContainer       The container where to find the parameter with the name "label"
/// \param[in]  label                The name of the parameter whe need
//--------------------------------------------------------------
void convertcmTomm(std::string& returnString, const shared::CDataContainer& valueContainer, const std::string& label);