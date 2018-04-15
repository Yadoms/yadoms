#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes mocks
#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"
#include "TeleInfoConfigurationMock.h"

// Includes needed to compile tested classes
#include "../../../../sources/plugins/TeleInfo/TeleInfoFactory.h"

namespace yApi = shared::plugin::yPluginApi;

BOOST_AUTO_TEST_SUITE(TestTeleInfoFactory)

   BOOST_AUTO_TEST_CASE(creation_FTDI_Driver)
   {
      boost::shared_ptr<TeleInfoConfigurationMock> m_configuration;
      boost::shared_ptr<CTeleInfoReceiveBufferHandler> m_receiveBufferHandler;

      m_configuration = boost::make_shared<TeleInfoConfigurationMock>();
      int kEvtPortConnection = 100;
      int kEvtPortDataReceived = 101;
      bool m_isDeveloperMode = false;

      auto api(boost::make_shared<CDefaultYPluginApiMock>());

      m_receiveBufferHandler = CTeleInfoFactory::GetBufferHandler(api->getEventHandler(),
                                                                  kEvtPortDataReceived,
                                                                  m_isDeveloperMode);

      auto m_port = CTeleInfoFactory::constructPort(*m_configuration,
                                                    api->getEventHandler(),
                                                    m_receiveBufferHandler,
                                                    kEvtPortConnection);
   }

BOOST_AUTO_TEST_SUITE_END()