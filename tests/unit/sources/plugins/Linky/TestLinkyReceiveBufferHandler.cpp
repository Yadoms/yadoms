#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Linky/LinkyReceiveBufferHandler.h"
#include <../../../../sources/shared/shared/communication/Buffer.hpp>
#include <../../../../sources/shared/shared/communication/IBufferLogger.h>

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

// Includes needed to compile the test
#include "../../testCommon/serialTeleInfoMessage.h"

using namespace testCommon;

class BufferLoggerMock : public shared::communication::IBufferLogger
{
public:
	explicit BufferLoggerMock() {}
	virtual ~BufferLoggerMock(){}
	void logReceived(const shared::communication::CByteBuffer& data) {}
	void logSent(const shared::communication::CByteBuffer& data) {}
	std::string msgToString(const shared::communication::CByteBuffer& data) const { return ""; }
};

// A Mock just to gain public visibility of normally protected methods
class CLinkyReceiveBufferHandlerMock : public CLinkyReceiveBufferHandler
{
public:
   CLinkyReceiveBufferHandlerMock(const EProtocolType type, 
                                  shared::event::CEventHandler& receiveDataEventHandler,
                                  int receiveDataEventId,
                                  boost::shared_ptr<shared::communication::IBufferLogger> logger)
      : CLinkyReceiveBufferHandler(type,
                                   receiveDataEventHandler,
                                   receiveDataEventId,
                                   logger,
                                   false)
   {
   }

   bool isCheckSumOk(const std::string& message)
   {
      return CLinkyReceiveBufferHandler::isCheckSumOk(message);
   }

   boost::shared_ptr<std::map<std::string, std::vector<std::string> > > getMessages(boost::shared_ptr<const std::vector<unsigned char>> frame)
   {
	   return CLinkyReceiveBufferHandler::getMessages(frame);
   }
};

BOOST_AUTO_TEST_SUITE(TestLinkyReceiveBufferHandler)

   BOOST_AUTO_TEST_CASE(CheckCrcStandard)
   {
      shared::event::CEventHandler eventHandler;
      CLinkyReceiveBufferHandlerMock bufferHandler(Standard,
                                                   eventHandler,
                                                   shared::event::kUserFirstId,
                                                   boost::make_shared<BufferLoggerMock>());


      // Too small message
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(""), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk("1"), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk("12"), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk("123"), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk("123"), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::string{ testCommon::kStartMessage, testCommon::kHorizontalTab, testCommon::kEndMessage }), false);

      // Empty message
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::string{ testCommon::kStartMessage, testCommon::kHorizontalTab, 0x29, testCommon::kEndMessage }), true); // 0x09+0x20
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::string{ testCommon::kStartMessage, testCommon::kHorizontalTab, 0x30, testCommon::kEndMessage }), false);

      // Message OK
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("ADSC<ht>041067003463<ht>/")), true);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("VTIC<ht>01<ht>I")), true);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("DATE<ht>h150101150844<ht><ht>_")), true);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("NGTF<ht>       HC       <ht>,")), true);

      // Wrong CRC
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("ADSC<ht>041067003463<ht>1")), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("LTARF<ht>       BASE     <ht>G")), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("EAST<ht>000046245<ht>4")), false);
      BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("SMAXN-1<ht>h141230000000<ht>00000<ht>D")), false);
   }

BOOST_AUTO_TEST_CASE(CheckCrcHistorical)
{
   shared::event::CEventHandler eventHandler;
   CLinkyReceiveBufferHandlerMock bufferHandler(Historic,
                                                eventHandler,
                                                shared::event::kUserFirstId,
                                                boost::make_shared<BufferLoggerMock>());


   // Too small message
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(""), false);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk("1"), false);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk("12"), false);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk("123"), false);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk("123"), false);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::string{ testCommon::kStartMessage, testCommon::kHorizontalTab, testCommon::kEndMessage }), false);

   // Empty message
   //BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::string{ testCommon::kStartMessage, testCommon::kHorizontalTab, 0x29, testCommon::kEndMessage }), true); // 0x09+0x20
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(std::string{ testCommon::kStartMessage, testCommon::kHorizontalTab, 0x30, testCommon::kEndMessage }), false);

   //-----------------------------------------------------
   //   Historical frames
   //-----------------------------------------------------

   // Message OK
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("OPTARIF BASE 0")), true);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("MOTDETAT 000000 B")), true);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("ADCO 031428097115 @")), true);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("PTEC TH.. $")), true);

   // Wrong CRC
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("OPTARIF BASE 1")), false);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("MOTDETAT 000000 Z")), false);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("ADCO 031428097115 5")), false);
   BOOST_CHECK_EQUAL(bufferHandler.isCheckSumOk(testCommon::serialTeleInfoMessage::toMessage("PTEC TH.. @")), false);
}

BOOST_AUTO_TEST_CASE(getMessagesLinky)
{
	const boost::shared_ptr<const std::vector<unsigned char>> frame = boost::make_shared<const std::vector<unsigned char>>(testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>"));
	const std::map<std::string, std::vector<std::string> > expectedMap = {
      { "ADSC", {"041067003463"} },
      { "VTIC", {"01"} },
      { "DATE", {"h150101150844"} },
      { "NGTF", {"       HC       "} },
      { "LTARF", {"       BASE     "} },
      { "SMAXN-1", {"h141230000000","00000"} },
      { "EAST", {"000046245"} } };

	shared::event::CEventHandler evtHandler;
	CLinkyReceiveBufferHandlerMock bufferHandler(Standard,
                                                evtHandler,
												            shared::event::kUserFirstId,
													         boost::make_shared<BufferLoggerMock>());

	const auto out = bufferHandler.getMessages(frame);
	BOOST_CHECK_EQUAL(*out == expectedMap, true);
}

   BOOST_AUTO_TEST_CASE(NominalLinky)
   {
      const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
                                               shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);

      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(twoframesOnePushLinky)
   {
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
      
      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(twoframesTwoPushLinky)
   {
	   const auto frame1 = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);

      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out1 = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out1 == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbetweenCRetxLinky)
   {                                                                                                                                                                                                                                                                                         //*******//
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr>D4R } $<etx>");

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbetweenstxLFLinky)
   {                                                                           //*******//                                                                                                                                                                                                
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx>D4R } $<lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbeforestxLinky)
   {
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbeforestx2Linky)
   {
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("0000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(OneCRCFailedLinky)
   {                                                                                           //|// Here the CRC Error
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>T<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(MissingCRLinky)
   {                                                                                                                                                                                                         //|// Missing CR
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(MultiframeLinky)
   {
	   const auto frame1 = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTI");
	   const auto frame2 = testCommon::serialTeleInfoMessage::normalizeFrame("C<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht> ");
	   const auto frame3 = testCommon::serialTeleInfoMessage::normalizeFrame("      HC       <ht>,<cr><lf>LTARF<ht>       BAS");
	   const auto frame4 = testCommon::serialTeleInfoMessage::normalizeFrame("E     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h1");
	   const auto frame5 = testCommon::serialTeleInfoMessage::normalizeFrame("41230000000<ht>00000<ht>C<cr><etx>");

      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);

      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
	   bufferHandler.push(shared::communication::CByteBuffer(frame2));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
	   bufferHandler.push(shared::communication::CByteBuffer(frame3));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
	   bufferHandler.push(shared::communication::CByteBuffer(frame4));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
	   bufferHandler.push(shared::communication::CByteBuffer(frame5));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(finalLinky)
   {
	   const auto frame1 = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr>");
	   const auto frame2 = testCommon::serialTeleInfoMessage::normalizeFrame("<etx>");
	   
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
												           shared::event::kUserFirstId,
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
	   bufferHandler.push(shared::communication::CByteBuffer(frame2));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(LF_Change_to_STXLinky)
   {
      const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Standard,
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);

      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(getMessagesHistorical)
   {
      const boost::shared_ptr<const std::vector<unsigned char>> frame = boost::make_shared<const std::vector<unsigned char>>(testCommon::serialTeleInfoMessage::normalizeFrame("5 F<cr><lf>PAPP 00490 .<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006251693 +<cr><lf>PTEC TH.. $<cr><lf>IINST 002 Y<cr><lf>IMAX 025 F<cr><lf>PAPP 00490 .<cr><lf>MOTDETAT 000000 B<cr><etx>"));
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADCO", {"031428097115"} },
         { "OPTARIF", {"BASE"} },
         { "ISOUSC", {"30"} },
         { "BASE", {"006251693"} },
         { "PTEC", {"TH.."} },
         { "IINST", {"002"} },
         { "IMAX", {"025"} },
         { "PAPP", {"00490"} },
         { "MOTDETAT", {"000000"} } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandlerMock bufferHandler(Historic,
                                                   evtHandler,
                                                   shared::event::kUserFirstId,
                                                   boost::make_shared<BufferLoggerMock>());

      const auto out = bufferHandler.getMessages(frame);
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(NominalHistorical)
   {
      const auto frame = serialTeleInfoMessage::normalizeFrame("<stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");
      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "006238747" },
         { "PTEC", "TH.." },
         { "IINST", "008" },
         { "IMAX", "025" },
         { "PAPP", "01940" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(twoframesOnePushHistorical)
   {
      const auto frame = serialTeleInfoMessage::normalizeFrame("<stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");
      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "006238747" },
         { "PTEC", "TH.." },
         { "IINST", "008" },
         { "IMAX", "025" },
         { "PAPP", "01940" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(twoframesTwoPushHistorical)
   {
      const auto frame1 = serialTeleInfoMessage::normalizeFrame("<stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");
      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "006238747" },
         { "PTEC", "TH.." },
         { "IINST", "008" },
         { "IMAX", "025" },
         { "PAPP", "01940" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame1));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
      bufferHandler.push(shared::communication::CByteBuffer(frame1));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out1 = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out1 == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbetweenCRetxHistorical)
   {                                                                                                                                                                                                                                          //*******//
      const auto frame = serialTeleInfoMessage::normalizeFrame("<stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr>D4R } $<etx>");

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbetweenstxLFHistorical)
   {                                                               //*******//                                                                                                                                                                                                
      const auto frame = serialTeleInfoMessage::normalizeFrame("<stx>D4R } $<lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");
      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "006238747" },
         { "PTEC", "TH.." },
         { "IINST", "008" },
         { "IMAX", "025" },
         { "PAPP", "01940" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbeforestxHistorical)
   {                                    //******************************************************************************************************************************************************************************************************//                                                                                                                                                                                                
      const auto frame = serialTeleInfoMessage::normalizeFrame("<lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");
      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "006238747" },
         { "PTEC", "TH.." },
         { "IINST", "008" },
         { "IMAX", "025" },
         { "PAPP", "01940" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbeforestx2Historical)
   {
      const auto frame = serialTeleInfoMessage::normalizeFrame("T 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006251729 +<cr><lf>PTEC TH.. $<cr><lf>IINST 002 Y<cr><lf>IMAX 025 F<cr><lf>PAPP 00460 +<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006251729 +<cr><lf>PTEC TH.. $<cr><lf>IINST 002 Y<cr><lf>IMAX 025 F<cr><lf>PAPP 00450 *<cr><lf>MOTDETAT 000000 B<cr><etx>");
      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "006251729" },
         { "PTEC", "TH.." },
         { "IINST", "002" },
         { "IMAX", "025" },
         { "PAPP", "00450" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(OneCRCFailedHistorical)
   {                                                                                     //|// Here the CRC Error
      const auto frame = serialTeleInfoMessage::normalizeFrame("<stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE Z<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(MissingCRHistorical)
   {                                                                                                                                                    //|// Missing CR
      const auto frame = serialTeleInfoMessage::normalizeFrame("<stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(MultiframeHistorical)
   {
      const auto frame1 = serialTeleInfoMessage::normalizeFrame("<stx><lf>ADCO 031428097115 @<cr><lf>OPTA");
      const auto frame2 = serialTeleInfoMessage::normalizeFrame("RIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE ");
      const auto frame3 = serialTeleInfoMessage::normalizeFrame("006238747 0<cr><lf>PTEC TH.. $<cr><lf>I");
      const auto frame4 = serialTeleInfoMessage::normalizeFrame("INST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 0");
      const auto frame5 = serialTeleInfoMessage::normalizeFrame("1940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");

      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "006238747" },
         { "PTEC", "TH.." },
         { "IINST", "008" },
         { "IMAX", "025" },
         { "PAPP", "01940" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);

      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame1));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
      bufferHandler.push(shared::communication::CByteBuffer(frame2));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
      bufferHandler.push(shared::communication::CByteBuffer(frame3));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
      bufferHandler.push(shared::communication::CByteBuffer(frame4));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
      bufferHandler.push(shared::communication::CByteBuffer(frame5));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(finalHistorical)
   {
      const auto frame1 = serialTeleInfoMessage::normalizeFrame("<stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr>");
      const auto frame2 = serialTeleInfoMessage::normalizeFrame("<etx>");

      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "006238747" },
         { "PTEC", "TH.." },
         { "IINST", "008" },
         { "IMAX", "025" },
         { "PAPP", "01940" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      useTimeMock();

      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame1));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
      bufferHandler.push(shared::communication::CByteBuffer(frame2));
      auto timeMock = useTimeMock();
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   //
   // This case is a real case found during exploitation
   //

   BOOST_AUTO_TEST_CASE(LF_Change_to_STXHistorical)
   {
      const auto frame = serialTeleInfoMessage::normalizeFrame("<stx>PAPP 00160 (<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 006238747 0<cr><lf>PTEC TH.. $<cr><lf>IINST 008 _<cr><lf>IMAX 025 F<cr><lf>PAPP 01940 /<cr><lf>MOTDETAT 000000 B<cr><etx>");
      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "006238747" },
         { "PTEC", "TH.." },
         { "IINST", "008" },
         { "IMAX", "025" },
         { "PAPP", "01940" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);

      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(LF_Change_to_STX2Historical)
   {
      const auto frame = serialTeleInfoMessage::normalizeFrame("<stx>F<cr><lf>PAPP 00280 +<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 008453671 -<cr><lf>PTEC TH.. $<cr><lf>IINST 001 X<cr><lf>IMAX 025 F<cr><lf>PAPP 00280 +<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 008453671 -<cr><lf>PTEC TH.. $<cr><lf>IINST 001 X<cr><lf>IMAX 025 F<cr><lf>PAPP 00270 *<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 008453671 -<cr><lf>PTEC TH.. $<cr><lf>IINST 001 X<cr><lf>IMAX 025 F<cr><lf>PAPP 00270 *<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @");
      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "008453671" },
         { "PTEC", "TH.." },
         { "IINST", "001" },
         { "IMAX", "025" },
         { "PAPP", "00270" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);

      bufferHandler.activate();
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(Last_DefaultHistorical)
   {
      //<stx>F<cr><lf>PAPP 00280 +<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 008453671 -<cr><lf>PTEC TH.. $<cr><lf>IINST 001 X<cr><lf>IMAX 025 F<cr><lf>PAPP 00280 +<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 008453671 -<cr><lf>PTEC TH.. $<cr><lf>IINST 001 X<cr><lf>IMAX 025 F<cr><lf>PAPP 00270 *<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 008453671 -<cr><lf>PTEC TH.. $<cr><lf>IINST 001 X<cr><lf>IMAX 025 F<cr><lf>PAPP 00270 *<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @
      const auto frame = serialTeleInfoMessage::normalizeFrame("@<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 008718847 6<cr><lf>PTEC TH.. $<cr><lf>IINST 001 X<cr><lf>IMAX 025 F<cr><lf>PAPP 00270 *<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BASE 008718847 6<cr><lf>PTEC TH.. $<cr><lf>IINST 001 X<cr><lf>IMAX 025 F<cr><lf>PAPP 00270 *<cr><lf>MOTDETAT 000000 B<cr><etx><stx><lf>");
      const std::map<std::string, std::string> expectedMap = {
         { "ADCO", "031428097115" },
         { "OPTARIF", "BASE" },
         { "ISOUSC", "30" },
         { "BASE", "008718847" },
         { "PTEC", "TH.." },
         { "IINST", "001" },
         { "IMAX", "025" },
         { "PAPP", "00270" },
         { "MOTDETAT", "000000" } };

      shared::event::CEventHandler evtHandler;
      CLinkyReceiveBufferHandler bufferHandler(Historic, 
                                               evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);

      bufferHandler.activate();
      // First frame
      bufferHandler.push(shared::communication::CByteBuffer(serialTeleInfoMessage::normalizeFrame("<etx><stx><lf>ADCO 031428097115 @<cr><lf>OPTARIF BASE 0<cr><lf>ISOUSC 30 9<cr><lf>BAS")));

      //push after all last characters individually
      for (int counter = 0; counter < frame.size(); ++counter)
      {
         std::vector<unsigned char> vect;
         vect.push_back(frame[counter]);
         bufferHandler.push(shared::communication::CByteBuffer(vect));
      }
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::string>>>();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_SUITE_END()