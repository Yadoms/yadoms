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
    /// \brief     Start the pairing
    //--------------------------------------------------------------
	virtual bool start() = 0;

    //--------------------------------------------------------------
    /// \brief     Stop the pairing
    //--------------------------------------------------------------
    virtual void stop() = 0;
};
