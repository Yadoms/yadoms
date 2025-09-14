#pragma once

//--------------------------------------------------------------
/// \brief	The device pairing helper
//--------------------------------------------------------------
class IPairingHelper
{
public:
    virtual ~IPairingHelper() = default;

    //--------------------------------------------------------------
    /// \brief     Check if pairing is currently active
    /// \return    true if pairing is active
    //--------------------------------------------------------------
    virtual bool isPairing() = 0;

    //--------------------------------------------------------------
    /// \brief     Stop the paring mode
    /// \param[in] deviceName  Name of the device paired (empty if nothing paired)
    //--------------------------------------------------------------
    virtual void stop(const std::string& deviceName) = 0;
};
