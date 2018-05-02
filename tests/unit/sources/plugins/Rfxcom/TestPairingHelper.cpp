#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Rfxcom/PairingHelper.h"
#include "server/pluginSystem/DefaultYPluginApiMock.hpp"

class CExtraQueryMock : public yApi::IExtraQuery
{
public:
   virtual ~CExtraQueryMock()
   {
   }

   boost::shared_ptr<yApi::IExtraQueryData> getData() const override { return boost::shared_ptr<yApi::IExtraQueryData>(); }

   void sendSuccess(const shared::CDataContainer& data) override
   {
   }

   void sendError(const std::string& errorMessage) override
   {
   }

   void reportProgress(const float progression, const std::string& message) override
   {
   }
};

BOOST_AUTO_TEST_SUITE(TestPairingHelper)

   BOOST_AUTO_TEST_CASE(AutoMode)
   {
      const auto api(boost::make_shared<CDefaultYPluginApiMock>());
      CPairingHelper ph(api, CPairingHelper::kAuto);

      BOOST_CHECK_EQUAL(ph.getMode(), CPairingHelper::kAuto);

      // Check always need pairing
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device2"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device2"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device2"), true);

      BOOST_CHECK_EQUAL(ph.startPairing(boost::make_shared<CExtraQueryMock>()), false);

      for (auto i = 0; i < 10; ++i)
         BOOST_CHECK_EQUAL(ph.onProgressPairing(), true);
   }

   BOOST_AUTO_TEST_CASE(ManualMode)
   {
      const auto api(boost::make_shared<CDefaultYPluginApiMock>());
      CPairingHelper ph(api, CPairingHelper::kManual);

      BOOST_CHECK_EQUAL(ph.getMode(), CPairingHelper::kManual);

      // Pairing disable
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), false);
      for (auto i = 0; i < 10; ++i)
      BOOST_CHECK_EQUAL(ph.onProgressPairing(), true);

      // Start pairing but no device found
      BOOST_CHECK_EQUAL(ph.startPairing(boost::make_shared<CExtraQueryMock>()), true);
      BOOST_CHECK_EQUAL(ph.startPairing(boost::make_shared<CExtraQueryMock>()), false); // Already started
      for (auto i = 0; i < 5; ++i)
      {
         BOOST_CHECK_EQUAL(ph.onProgressPairing(), false);
      }
      BOOST_CHECK_EQUAL(ph.onProgressPairing(), true);

      // Start pairing but no device found
      BOOST_CHECK_EQUAL(ph.startPairing(boost::make_shared<CExtraQueryMock>()), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), false);
      BOOST_CHECK_EQUAL(ph.needPairing("device2"), false);
   }


BOOST_AUTO_TEST_SUITE_END()
