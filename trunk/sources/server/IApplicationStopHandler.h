//
// System signals handler
//
#pragma once

//-----------------------------------------------------------------------------
/// \class              Application stop handler
//-----------------------------------------------------------------------------
class IApplicationStopHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	    The stop mode
   //--------------------------------------------------------------
   enum EStopMode
   {
      kYadomsOnly,         ///< Stops only Yadoms
      kStopSystem,         ///< Stops Yadoms + halts system
      kRestartSystem,      ///< Stops Yadoms + reboot system
   };

public:
   //-----------------------------------------------------------------------------
   /// \brief		                     Constructor
   /// \param[in] targetEventHandler   Event handler to notify
   /// \param[in] eventId              Event ID to send when stop occurs
   //-----------------------------------------------------------------------------
   virtual void requestToStop(EStopMode stopMode) = 0;
};
