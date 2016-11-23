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
   static std::string GenerateDeviceName(Poco::UInt32 homeId, Poco::UInt8 nodeId);

   //-----------------------------------------------------------------------------
   /// \brief	Get value information for strings
   /// \param [in]  value  The input value
   /// \param [out] help   The value help string
   /// \param [out] name   The value name
   /// \return The yadoms device name
   //-----------------------------------------------------------------------------
   static void GetStringValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help);

   //-----------------------------------------------------------------------------
   /// \brief	Get value information for booleans
   /// \param [in]  value  The input value
   /// \param [out] help   The value help string
   /// \param [out] name   The value name
   /// \return The yadoms device name
   //-----------------------------------------------------------------------------
   static void GetBooleanValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help);


   //-----------------------------------------------------------------------------
   /// \brief	Get value information for integers
   /// \param [in]  value  The input value
   /// \param [out] name   The value name
   /// \param [out] help   The value help string
   /// \param [out] min    The value max
   /// \param [out] max    The value min 
   /// \param [out] unit   The value units
   /// \return The yadoms device name
   //-----------------------------------------------------------------------------
   static void GetIntegerValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help, int & min, int & max, std::string & unit);

   //-----------------------------------------------------------------------------
   /// \brief	Get value information for decimals
   /// \param [in]  value  The input value
   /// \param [out] name   The value name
   /// \param [out] help   The value help string
   /// \param [out] min    The value max
   /// \param [out] max          The value min 
   /// \param [out] unit         The value units
   /// \param [out] precision    The value precision
   /// \return The yadoms device name
   //-----------------------------------------------------------------------------
   static void GetDecimalValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help, int & min, int & max, std::string & unit, int & precision);   
   
   //-----------------------------------------------------------------------------
   /// \brief	Get value information for enumerations
   /// \param [in]  value  The input value
   /// \param [out] name   The value name
   /// \param [out] help   The value help string
   /// \param [out] values    The value values (value:label)
   /// \return The yadoms device name
   //-----------------------------------------------------------------------------
   static void GetEnumValueInfo(OpenZWave::ValueID& value, std::string & name, std::string & help, std::map<int, std::string> & values);
};

