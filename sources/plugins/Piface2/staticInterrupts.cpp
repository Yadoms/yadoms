#include "stdafx.h"
#include "Piface2Factory.h"
#include "staticInterrupts.h"
#include "IO.h"
#include "wiringPi.h"

//TODO : Dans chaque interruption, construire une commande identique que pour un write
void OnInterrupt(const int portUsed, const std::string keywordName, bool value)
{
   CIOState Event = { portUsed, keywordName, value };
   CPiface2Factory::SendMessage(Event);
}

void interrupt0()
{
   int value = digitalRead ( m_portUsed );
   OnInterrupt(0, "DI0", (bool)value);
}

void interrupt1()
{
   int value = digitalRead ( m_portUsed );
   OnInterrupt(1, "DI1", (bool)value);
}

void interrupt2()
{
   int value = digitalRead ( m_portUsed );
   OnInterrupt(2, "DI2", (bool)value);
}

void interrupt3()
{
   int value = digitalRead ( m_portUsed );
   OnInterrupt(3, "DI3", (bool)value);
}

void interrupt4()
{
   int value = digitalRead ( m_portUsed );
   OnInterrupt(4, "DI4", (bool)value);
}

void interrupt5()
{
   int value = digitalRead ( m_portUsed );
   OnInterrupt(5, "DI5", (bool)value);
}

void interrupt6()
{
   int value = digitalRead ( m_portUsed );
   OnInterrupt(6, "DI6", (bool)value);
}

void interrupt7()
{
   int value = digitalRead ( m_portUsed );
   OnInterrupt(7, "DI7", (bool)value);
}