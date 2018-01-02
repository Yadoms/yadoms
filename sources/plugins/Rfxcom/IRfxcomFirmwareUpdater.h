#pragma once

//--------------------------------------------------------------
/// \brief	Interface of RFXCom firmware update process
//--------------------------------------------------------------
class IRfxcomFirmwareUpdater
{
public:
   virtual ~IRfxcomFirmwareUpdater() {}

   virtual void update() = 0;
};

