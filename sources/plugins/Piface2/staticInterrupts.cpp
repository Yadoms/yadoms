#include "stdafx.h"
#include "IOManager.h"
#include "staticInterrupts.h"
#include "IO.h"
//#include "wiringPi.h"

//TODO : Dans chaque interruption, construire une commande identique que pour un write
void OnInterrupt(const int portUsed, const std::string keywordName)
{
   int value = digitalRead(portUsed);
   CIOState Event = { portUsed, keywordName, (bool)value };
   //CIOManager::SendMessage(Event);
}

void interrupt0()
{
   OnInterrupt(0, "DI0");
}

void interrupt1()
{
   OnInterrupt(1, "DI1");
}

void interrupt2()
{
   OnInterrupt(2, "DI2");
}

void interrupt3()
{
   OnInterrupt(3, "DI3");
}

void interrupt4()
{
   OnInterrupt(4, "DI4");
}

void interrupt5()
{
   OnInterrupt(5, "DI5");
}

void interrupt6()
{
   OnInterrupt(6, "DI6");
}

void interrupt7()
{
   OnInterrupt(7, "DI7");
}