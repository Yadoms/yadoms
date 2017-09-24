#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Linky/LinkyReceiveBufferHandler.h"
#include <../../../../sources/shared/shared/communication/Buffer.hpp>
#include <../../../../sources/shared/shared/communication/IBufferLogger.h>

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

// Includes needed to compile the test
#include "../../testCommon/serialTeleInfoMessage.h"


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
   CLinkyReceiveBufferHandlerMock(shared::event::CEventHandler& receiveDataEventHandler,
                                  int receiveDataEventId,
                                  const boost::posix_time::time_duration suspendDelay,
                                  boost::shared_ptr<shared::communication::IBufferLogger> logger)
      : CLinkyReceiveBufferHandler(receiveDataEventHandler,
                                   receiveDataEventId,
                                   suspendDelay,
                                   logger,
                                   false)
   {
   }

   static bool isCheckSumOk(const std::string& message)
   {
      return CLinkyReceiveBufferHandler::isCheckSumOk(message);
   }

   static  boost::shared_ptr<std::map<std::string, std::vector<std::string> > > getMessages(boost::shared_ptr<const std::vector<unsigned char>> frame)
   {
	   return CLinkyReceiveBufferHandler::getMessages(frame);
   }


};

BOOST_AUTO_TEST_SUITE(TestLinkyReceiveBufferHandler)

   BOOST_AUTO_TEST_CASE(CheckCrc)
   {
      shared::event::CEventHandler eventHandler;
      CLinkyReceiveBufferHandlerMock bufferHandler(eventHandler,
                                                   shared::event::kUserFirstId,
                                                   boost::posix_time::seconds(30),
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

BOOST_AUTO_TEST_CASE(getMessages)
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
	CLinkyReceiveBufferHandlerMock bufferHandler(evtHandler,
												            shared::event::kUserFirstId,
												            boost::posix_time::seconds(0),
													         boost::make_shared<BufferLoggerMock>());

	const auto out = bufferHandler.getMessages(frame);
	BOOST_CHECK_EQUAL(*out == expectedMap, true);
}

   BOOST_AUTO_TEST_CASE(Nominal)
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
      CLinkyReceiveBufferHandler bufferHandler(evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(twoframesOnePush)
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
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(twoframesTwoPush)
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
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out1 = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out1 == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbetweenCRetx)
   {                                                                                                                                                                                                                                                                                         //*******//
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr>D4R } $<etx>");

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbetweenstxLF)
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
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbeforestx)
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
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(extraCharactersbeforestx2)
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
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(OneCRCFailed)
   {                                                                                           //|// Here the CRC Error
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>T<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(MissingCR)
   {                                                                                                                                                                                                         //|// Missing CR
	   const auto frame = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(Multiframe)
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
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);

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

   BOOST_AUTO_TEST_CASE(final)
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
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(0),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
	   bufferHandler.push(shared::communication::CByteBuffer(frame2));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(threeImmediateframesTwoPushdelay30s)
   {
	   shared::currentTime::Provider().setProvider(boost::make_shared<CDefaultCurrentTimeMock>());

	   const auto frame1 = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(30),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   }

   BOOST_AUTO_TEST_CASE(threeframesat30sTwoPushdelay30s)
   {
	   auto timeMock = boost::make_shared<CDefaultCurrentTimeMock>();
	   shared::currentTime::Provider().setProvider(timeMock);

	   const auto frame1 = testCommon::serialTeleInfoMessage::normalizeFrame("<stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx><stx><lf>ADSC<ht>041067003463<ht>/<cr><lf>VTIC<ht>01<ht>I<cr><lf>DATE<ht>h150101150844<ht><ht>_<cr><lf>NGTF<ht>       HC       <ht>,<cr><lf>LTARF<ht>       BASE     <ht>F<cr><lf>EAST<ht>000046245<ht>$<cr><lf>SMAXN-1<ht>h141230000000<ht>00000<ht>C<cr><etx>");
      const std::map<std::string, std::vector<std::string> > expectedMap = {
         { "ADSC",{ "041067003463" } },
         { "VTIC",{ "01" } },
         { "DATE",{ "h150101150844" } },
         { "NGTF",{ "       HC       " } },
         { "LTARF",{ "       BASE     " } },
         { "SMAXN-1",{ "h141230000000","00000" } },
         { "EAST",{ "000046245" } } };

	   shared::event::CEventHandler evtHandler;
	   CLinkyReceiveBufferHandler bufferHandler(evtHandler,
												           shared::event::kUserFirstId,
												           boost::posix_time::seconds(30),
												           boost::make_shared<BufferLoggerMock>(),
                                               false);
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out == expectedMap, true);
	   timeMock->sleep(boost::posix_time::seconds(30));
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out1 = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out1 == expectedMap, true);
	   timeMock->sleep(boost::posix_time::seconds(30));
	   bufferHandler.push(shared::communication::CByteBuffer(frame1));
	   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
	   const auto out2 = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
	   BOOST_CHECK_EQUAL(*out2 == expectedMap, true);
   }

   BOOST_AUTO_TEST_CASE(LF_Change_to_STX)
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
      CLinkyReceiveBufferHandler bufferHandler(evtHandler,
                                               shared::event::kUserFirstId,
                                               boost::posix_time::seconds(0),
                                               boost::make_shared<BufferLoggerMock>(),
                                               false);
      bufferHandler.push(shared::communication::CByteBuffer(frame));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      const auto out = evtHandler.getEventData<boost::shared_ptr<std::map<std::string, std::vector<std::string> > > >();
      BOOST_CHECK_EQUAL(*out == expectedMap, true);
   }

   BOOST_AUTO_TEST_SUITE_END()