#include "stdafx.h"
#include "Piface2Factory.h"
#include "staticInterrupts.h"
#include "eventIdentification.h"
#include "IO.h"
#include "wiringPi.h"

//TODO : Dans chaque interruption, construire une commande identique que pour un write

void interrupt0()
{
   const int m_portUsed=0;
   const std::string m_keywordName("DO0");
   int value = digitalRead ( m_portUsed );

   CPiface2Factory::m_Event.postEvent<const IOState>(kEvtIOStateReceived, { m_portUsed, m_keywordName, value });
}

void interrupt1()
{
   const int m_portUsed=1;
   const std::string m_keywordName("DO1");
   int value = digitalRead ( m_portUsed );

   CPiface2Factory::m_Event.postEvent<const IOState>(kEvtIOStateReceived, { m_portUsed, m_keywordName, value });
}

void interrupt2()
{
   const int m_portUsed=2;
   const std::string m_keywordName("DO2");
   int value = digitalRead ( m_portUsed );

   CPiface2Factory::m_Event.postEvent<const IOState>(kEvtIOStateReceived, { m_portUsed, m_keywordName, value });
}

void interrupt3()
{
   const int m_portUsed=3;
   const std::string m_keywordName("DO3");
   int value = digitalRead ( m_portUsed );

   CPiface2Factory::m_Event.postEvent<const IOState>(kEvtIOStateReceived, { m_portUsed, m_keywordName, value });
}

void interrupt4()
{
   const int m_portUsed=4;
   const std::string m_keywordName("DO4");
   int value = digitalRead ( m_portUsed );

   CPiface2Factory::m_Event.postEvent<const IOState>(kEvtIOStateReceived, { m_portUsed, m_keywordName, value });
}

void interrupt5()
{
   const int m_portUsed=5;
   const std::string m_keywordName("DO5");
   int value = digitalRead ( m_portUsed );

   CPiface2Factory::m_Event.postEvent<const IOState>(kEvtIOStateReceived, { m_portUsed, m_keywordName, value });
}

void interrupt6()
{
   const int m_portUsed=6;
   const std::string m_keywordName("DO6");
   int value = digitalRead ( m_portUsed );

   CPiface2Factory::m_Event.postEvent<const IOState>(kEvtIOStateReceived, { m_portUsed, m_keywordName, value });
}

void interrupt7()
{
   const int m_portUsed=7;
   const std::string m_keywordName("DO7");
   int value = digitalRead ( m_portUsed );

   CPiface2Factory::m_Event.postEvent<const IOState>(kEvtIOStateReceived, { m_portUsed, m_keywordName, value });
}
