#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes mocks
#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"
#include "TeleInfoConfigurationMock.h"

// Includes needed to compile tested classes
#include "TeleInfoFactory.h"

namespace yApi = shared::plugin::yPluginApi;

BOOST_AUTO_TEST_SUITE(TestTeleInfoFactory)

   BOOST_AUTO_TEST_CASE(creation_FTDI_Driver)
   {
      const auto configuration = boost::make_shared<TeleInfoConfigurationMock>();
      const auto evtPortConnection = 100;
      const auto evtPortDataReceived = 101;
      const auto isDeveloperMode = false;

      auto api(boost::make_shared<CDefaultYPluginApiMock>());

      const auto receiveBufferHandler = CTeleInfoFactory::GetBufferHandler(api->getEventHandler(),
                                                                           evtPortDataReceived,
                                                                           isDeveloperMode);

      auto port = CTeleInfoFactory::constructPort(*configuration,
                                                  api->getEventHandler(),
                                                  receiveBufferHandler,
                                                  evtPortConnection);
   }

BOOST_AUTO_TEST_SUITE_END()
