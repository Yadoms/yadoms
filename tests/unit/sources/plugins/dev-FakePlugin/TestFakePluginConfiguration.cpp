#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/dev-FakePlugin/FakePluginConfiguration.h"

BOOST_AUTO_TEST_SUITE(TestFakePluginConfiguration)

const boost::shared_ptr<shared::CDataContainer> conf = shared::CDataContainer::make("{"
	"\"EnumParameter\": \"EnumValue1\""
	"}");

BOOST_AUTO_TEST_CASE(ReadFakePluginDefaultConfiguration)
{
	std::string configPath = "../../../sources/plugins/dev-FakePlugin/package.in.json";
	// Need to copy the package.json file from fakePlugin, needed by initializeWith method to get default configuration
	if (!boost::filesystem::exists(configPath)) {
		configPath = "../../../../sources/plugins/dev-FakePlugin/package.in.json";
		if (!boost::filesystem::exists(configPath)) {
			BOOST_ERROR("dev-FakePlugin/package.in.json not found");
		}
	}
	CFakePluginConfiguration cfg;
	cfg.initializeWith(conf, boost::filesystem::path(configPath));

	BOOST_CHECK_EQUAL(cfg.getEnumParameter(), static_cast<EEnumType>(kEnumValue1));
}

BOOST_AUTO_TEST_SUITE_END()