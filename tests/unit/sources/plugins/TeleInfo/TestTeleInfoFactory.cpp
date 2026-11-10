#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes mocks
#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"
#include "TeleInfoConfigurationMock.h"

// Includes needed to compile tested classes
#include "TeleInfoFactory.h"

BOOST_AUTO_TEST_SUITE(TestTeleInfoFactory)

   BOOST_AUTO_TEST_CASE(CreationFtdiDriver)
   {
      const auto configuration = boost::make_shared<TeleInfoConfigurationMock>();
      constexpr auto evtPortConnection = 100;
      constexpr auto evtPortDataReceived = 101;
      constexpr auto isDeveloperMode = false;

      const auto api(boost::make_shared<CDefaultYPluginApiMock>());

      const auto receiveBufferHandler = CTeleInfoFactory::GetBufferHandler(api->getEventHandler(),
                                                                           evtPortDataReceived,
                                                                           isDeveloperMode);

      auto port = CTeleInfoFactory::constructPort(*configuration,
                                                  api->getEventHandler(),
                                                  receiveBufferHandler,
                                                  evtPortConnection);
   }

BOOST_AUTO_TEST_SUITE_END()
