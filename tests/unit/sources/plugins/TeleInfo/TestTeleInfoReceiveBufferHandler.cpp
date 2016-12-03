#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/TeleInfo/TeleInfoReceiveBufferHandler.h"
#include <../../../../sources/shared/shared/communication/Buffer.hpp>

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

// Includes needed to compile the test
#include "../../testCommon/fileSystem.h"


enum
{
   kSTX = 0x02,
   kETX = 0x03,

   kStartMessage = 0x0a,
   kEndMessage = 0x0d
};

// A Mock just to gain public visibility of normally protected methods
class CTeleInfoReceiveBufferHandlerMock : public CTeleInfoReceiveBufferHandler
{
public:
   CTeleInfoReceiveBufferHandlerMock(shared::event::CEventHandler& receiveDataEventHandler,
                                     int receiveDataEventId,
                                     const boost::posix_time::time_duration suspendDelay)
      : CTeleInfoReceiveBufferHandler(receiveDataEventHandler,
                                      receiveDataEventId,
                                      suspendDelay)
   {
   }

   static bool isCheckSumOk(const std::vector<unsigned char>& message)
   {
      return CTeleInfoReceiveBufferHandler::isCheckSumOk(message);
   }
};

std::vector<unsigned char> toMessage(const std::string& content)
{
   std::vector<unsigned char> message;
   message.push_back(kStartMessage);
   for (const auto car:content)
      message.push_back(car);
   message.push_back(kEndMessage);
   return message;
}

std::vector<unsigned char> normalizeFrame(const std::string& content)
{
   auto outString = content;
   boost::replace_all(outString, "<etx>", std::string(1, kETX));
   boost::replace_all(outString, "<stx>", std::string(1, kSTX));
   boost::replace_all(outString, "<lf>", std::string(1, kStartMessage));
   boost::replace_all(outString, "<cr>", std::string(1, kEndMessage));

   std::vector<unsigned char> outVector;
   for (const auto car : outString)
      outVector.push_back(static_cast<unsigned char>(car));

   return outVector;
}

BOOST_AUTO_TEST_SUITE(TestTeleInfoReceiveBufferHandler)

   BOOST_AUTO_TEST_CASE(CheckCrc)
   {
      shared::event::CEventHandler eventHandler;
      CTeleInfoReceiveBufferHandlerMock bufferHandler(eventHandler,
                                                      shared::event::kUserFirstId,
                                                      boost::posix_time::seconds(30));


      // Too small message
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::vector<unsigned char>{ 0x55 }), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::vector<unsigned char>{ 0x55, 0xAA }), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::vector<unsigned char>{ 0x55, 0xAA, 0x55 }), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::vector<unsigned char>{ kStartMessage, ' ', kEndMessage }), false);

      // Empty message
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::vector<unsigned char>{ kStartMessage, ' ', 0x20, kEndMessage }), true);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::vector<unsigned char>{ kStartMessage, ' ', 0x21, kEndMessage }), false);

      // Message OK
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(toMessage("OPTARIF BASE 0")), true);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(toMessage("MOTDETAT 000000 B")), true);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(toMessage("ADCO 031428097115 @")), true);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(toMessage("PTEC TH.. $")), true);

      // Wrong CRC
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(toMessage("OPTARIF BASE 1")), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(toMessage("MOTDETAT 000000 Z")), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(toMessage("ADCO 031428097115 5")), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(toMessage("PTEC TH.. @")), false);
   }

   BOOST_AUTO_TEST_CASE(Nominal)
   {
      const auto frame = normalizeFrame("<stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");

      shared::event::CEventHandler evtHandler;
      CTeleInfoReceiveBufferHandler bufferHandler(evtHandler,
                                                  shared::event::kUserFirstId,
                                                  boost::posix_time::seconds(1));
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);

      BOOST_CHECK_NO_THROW(evtHandler.getEventData<const std::vector<unsigned char>>());
      const auto out = evtHandler.getEventData<const std::vector<unsigned char>>();
      BOOST_CHECK_EQUAL(std::equal(out.begin(), out.end(), frame.begin()), true);
   }

   // TODO ajouter tests :
   // - caractères inattendus avant STX
   // - 2 trames enchainées
   // - Trame arrivant en plusieurs morceaux
   // - Trame refusée si un CRC KO sur un message
   // - Délai de suspend (utiliser le DefaultCurrentTimeMock pour simuler le temps) :
   //   - Pas de message pendant le délai
   //   - Message reçu OK et cohérent (début et fin correctes)
   // - Caractère de fin de message <cr> manquant
   // - Caractère inattendu entre fin de message <cr> et fin de trame <etx>
   // - Caractère inattendu entre début de message <lf> et début de trame <stx>

   BOOST_AUTO_TEST_SUITE_END()
