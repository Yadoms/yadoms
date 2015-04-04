#pragma once

//--------------------------------------------------------------
/// \brief	An example of an enum type of parameter
//--------------------------------------------------------------
enum EPeriod
{
   kDay = 0,
   kHour = 1
};

extern const std::string EPeriodEnum[sizeof(EPeriod)];