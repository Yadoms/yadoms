#pragma once

#include <value_classes/ValueID.h>
#include <Poco/Types.h>

class COpenZWaveHelpers
{
public:
   //-----------------------------------------------------------------------------
   /// \brief	Generate the keyword name from a ValueID
   /// \param [in] value   The valueID from OpenZWave library
   /// \return The yadoms keyword name
   //-----------------------------------------------------------------------------
   static std::string GenerateKeywordName(OpenZWave::ValueID& value);

   //-----------------------------------------------------------------------------
   /// \brief	Generate the device name from a home and node id
   /// \param [in] homeId   The home id
   /// \param [in] nodeId   The node id
   /// \return The yadoms device name
   //-----------------------------------------------------------------------------
   static std::string GenerateDeviceName(Poco::UInt32 homeId,
                                         Poco::UInt8 nodeId);
};

