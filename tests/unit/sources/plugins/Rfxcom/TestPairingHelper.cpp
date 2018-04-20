#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Rfxcom/PairingHelper.h"
#include "server/pluginSystem/DefaultYPluginApiMock.hpp"

BOOST_AUTO_TEST_SUITE(TestPairingHelper)

   BOOST_AUTO_TEST_CASE(AutoMode)
   {
      const auto api(boost::make_shared<CDefaultYPluginApiMock>());
      CPairingHelper ph(api, CPairingHelper::kAuto);

      BOOST_CHECK_EQUAL(ph.getMode(), CPairingHelper::kAuto);
      BOOST_CHECK_EQUAL(ph.isPairingEnable(), true);

      // Check always need pairing
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device2"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device2"), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device2"), true);

      BOOST_CHECK_THROW(ph.startPairing(), std::invalid_argument);
   }

   BOOST_AUTO_TEST_CASE(ManualMode)
   {
      const auto api(boost::make_shared<CDefaultYPluginApiMock>());
      CPairingHelper ph(api, CPairingHelper::kManual);

      BOOST_CHECK_EQUAL(ph.getMode(), CPairingHelper::kManual);

      // Pairing disable
      BOOST_CHECK_EQUAL(ph.isPairingEnable(), false);
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), false);

      // Stop pending pairing
      ph.startPairing();
      BOOST_CHECK_EQUAL(ph.isPairingEnable(), true);
      ph.stopPairing();
      BOOST_CHECK_EQUAL(ph.isPairingEnable(), false);

      // Enable one-shot pairing
      ph.startPairing();
      BOOST_CHECK_EQUAL(ph.isPairingEnable(), true);
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), true);
      BOOST_CHECK_EQUAL(ph.isPairingEnable(), false);
      BOOST_CHECK_EQUAL(ph.needPairing("device1"), false);
      BOOST_CHECK_EQUAL(ph.needPairing("device2"), false);
   }

   BOOST_AUTO_TEST_CASE(ChangeMode)
   {
      const auto api(boost::make_shared<CDefaultYPluginApiMock>());

      // Create in auto mode
      CPairingHelper ph1(api, CPairingHelper::kAuto);

      BOOST_CHECK_EQUAL(ph1.getMode(), CPairingHelper::kAuto);
      BOOST_CHECK_EQUAL(ph1.isPairingEnable(), true);

      ph1.setMode(CPairingHelper::kManual);
      BOOST_CHECK_EQUAL(ph1.getMode(), CPairingHelper::kManual);
      BOOST_CHECK_EQUAL(ph1.isPairingEnable(), false);

      ph1.setMode(CPairingHelper::kAuto);
      BOOST_CHECK_EQUAL(ph1.getMode(), CPairingHelper::kAuto);
      BOOST_CHECK_EQUAL(ph1.isPairingEnable(), true);


      // Create in manual mode
      CPairingHelper ph2(api, CPairingHelper::kManual);

      BOOST_CHECK_EQUAL(ph2.getMode(), CPairingHelper::kManual);
      BOOST_CHECK_EQUAL(ph2.isPairingEnable(), false);

      ph2.setMode(CPairingHelper::kAuto);
      BOOST_CHECK_EQUAL(ph2.getMode(), CPairingHelper::kAuto);
      BOOST_CHECK_EQUAL(ph2.isPairingEnable(), true);

      ph2.setMode(CPairingHelper::kManual);
      BOOST_CHECK_EQUAL(ph2.getMode(), CPairingHelper::kManual);
      BOOST_CHECK_EQUAL(ph2.isPairingEnable(), false);
   }


BOOST_AUTO_TEST_SUITE_END()
