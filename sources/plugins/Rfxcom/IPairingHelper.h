#pragma once

//--------------------------------------------------------------
/// \brief	The device pairing helper
//--------------------------------------------------------------
class IPairingHelper
{
public:
   enum EPairingMode
   {
      kAuto = 0,
      kManual
   };

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

   //--------------------------------------------------------------
   /// \brief     Get the pairing mode
   /// \return    The pairing mode
   //--------------------------------------------------------------
   virtual EPairingMode getMode() const = 0;
};
