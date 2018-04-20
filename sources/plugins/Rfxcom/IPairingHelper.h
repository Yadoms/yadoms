#pragma once

//--------------------------------------------------------------
/// \brief	The device pairing helper
//--------------------------------------------------------------
class IPairingHelper
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IPairingHelper()
   {
   }

   //--------------------------------------------------------------
   /// \brief     Check if device need to be paired
   /// \param[in] deviceName  Device to check for pairing
   /// \return    true if device need to be paired and pairing is enable
   //--------------------------------------------------------------
   virtual bool needPairing(const std::string deviceName) = 0;
};
