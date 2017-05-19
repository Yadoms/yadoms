#pragma once



//--------------------------------------------------------------
/// \brief	                     Scale a value from a range to another
/// \param [in] rawValue         Read value from device
/// \param [in] rangeMin         Min range of the read value (ex 0..255 for a byte value)
/// \param [in] rangeMax         Max range of the read value
/// \param [in] scaleMin         Min scale of the real value (ex -40° for temperature sensor of FUNC=2 and Type=1)
/// \param [in] scaleMax         Max scale of the real value (ex 0° for temperature sensor of FUNC=2 and Type=1)
/// \return                      Scaled value
//--------------------------------------------------------------
inline double scaleToDouble(unsigned int rawValue,
                            unsigned int rangeMin,
                            unsigned int rangeMax,
                            double scaleMin,
                            double scaleMax)
{
   auto multiplier = (scaleMax - scaleMin) / (rangeMax - rangeMin);
   return multiplier * (rawValue - rangeMin) + scaleMin;
}