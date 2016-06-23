#pragma once

//--------------------------------------------------------------
/// \brief	The unsecured protocols filter interface
//--------------------------------------------------------------
class IUnsecuredProtocolFilter
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IUnsecuredProtocolFilter()
   {
   }

   //--------------------------------------------------------------
   /// \brief	                     Check if data is valid (filter only once seen devices)
   /// \param[in] deviceName        Seen device name
   /// \return                      true if device enough seen to be considered as valid
   //--------------------------------------------------------------
   virtual bool isValid(const std::string& deviceName) = 0;
};

