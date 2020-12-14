#pragma once

class IHueService
{
public:
   virtual ~IHueService() = default;

   virtual void startReadingBridgeButtonState() = 0;
   virtual void closeReadingBridgeButtonState() = 0;
};
