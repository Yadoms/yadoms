#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/exception/InvalidParameter.hpp"
#include "../../../../sources/server/pluginSystem/Information.h"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

BOOST_AUTO_TEST_SUITE(TestPluginInformation)

static const boost::filesystem::path DefaultPackagePath("FakePackage");
static const std::string DefaultPackageFileContent(
"{"
"   \"type\": \"FakePackage\","
"      \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
"      \"version\": \"0.0.1-beta\","
"      \"author\": \"yadoms-team\","
"     \"url\": \"https://github.com/Yadoms/yadoms/\","
"     \"configurationSchema\":"
"  {"
"      \"StringParameter\":"
"     {"
"         \"name\": \"A string example\","
"           \"description\": \"Just a string parameter\","
"            \"type\": \"string\","
"            \"defaultValue\": \"Yadoms is so powerful !\""
"      },"
"      \"BoolParameter\":"
"         {"
"            \"name\": \"A bool example\","
"               \"description\": \"Just a boolean example\","
"               \"type\": \"bool\","
"               \"defaultValue\": \"false\""
"         },"
"         \"IntParameter\":"
"            {"
"               \"name\": \"An integer example\","
"                  \"description\": \"This is my int parameter example\","
"                  \"type\": \"int\","
"                  \"defaultValue\": \"258\""
"            },"
"            \"DecimalParameter\":"
"               {"
"                  \"name\": \"A decimal example\","
"                     \"description\": \"Now a decimal\","
"                     \"type\": \"decimal\","
"                     \"defaultValue\": \"25.3\""
"               },"
"               \"EnumParameter\":"
"                  {"
"                     \"name\": \"A custom enum example\","
"                        \"description\": \"Example of an enum with 3 values\","
"                        \"type\": \"enum\","
"                        \"values\":"
"                     {"
"                        \"EnumValue1\": \"My first value\","
"                           \"EnumValue2\": \"My second value\","
"                           \"EnumValue3\": \"My third value\""
"                     },"
"                     \"defaultValue\": \"EnumValue2\""
"                  },"
"                  \"Serial Port\":"
"                        {"
"                           \"name\": \"A serial port\","
"                              \"description\": \"Specific serial port parameter. Yadoms will populate available values with system serial ports.\","
"                              \"type\": \"serialPort\""
"                        },"
"                        \"MySection\":"
"                           {"
"                              \"name\": \"A section\","
"                                 \"description\": \"Example of using a section in configuration tree.\","
"                                 \"type\" : \"section\","
"                                 \"content\":"
"                              {"
"                                 \"SubIntParameter\":"
"                                 {"
"                                    \"name\": \"An integer-in-section example\","
"                                       \"description\": \"An int in sub-section example\","
"                                       \"type\": \"int\","
"                                       \"defaultValue\": \"65535\""
"                                 },"
"                                 \"SubStringParameter\":"
"                                    {"
"                                       \"name\": \"A string-in-section example\","
"                                          \"description\": \"Just a string parameter in the sub-section\","
"                                          \"type\": \"string\","
"                                          \"defaultValue\": \"Sub-sections make configuration very powerful !\""
"                                    }"
"                              }"
"                           }"
"   }"
"}");



class CTestPackageFile
{
public:
   explicit CTestPackageFile(const std::string& content)
      :m_packagePath(DefaultPackagePath / boost::filesystem::path("package.json"))
   {
      // Create the package.json file
      testCommon::filesystem::writeFile(m_packagePath, content);
   }
   virtual ~CTestPackageFile()
   {
      // Clean-up
      testCommon::filesystem::removeFile(m_packagePath, false);
   }
private:
   const boost::filesystem::path m_packagePath;
};

BOOST_AUTO_TEST_CASE(ReadDefaultInformation)
{
   CTestPackageFile packageFile(DefaultPackageFileContent);
   pluginSystem::CInformation info(DefaultPackagePath);

   BOOST_CHECK_EQUAL(info.getType(), "FakePackage");
   BOOST_CHECK_EQUAL(info.getVersion().toString(), "0.0.1-beta");
   BOOST_CHECK_EQUAL(info.getAuthor(), "yadoms-team");
   BOOST_CHECK_EQUAL(info.getUrl(), "https://github.com/Yadoms/yadoms/");
   BOOST_CHECK_EQUAL(info.getIdentity(), "FakePackage v0.0.1-beta");
   BOOST_CHECK_EQUAL(info.toString(), "FakePackage v0.0.1-beta by yadoms-team (https://github.com/Yadoms/yadoms/)");
}

BOOST_AUTO_TEST_CASE(WrongPackageFileJsonFormat)
{
   const std::string fileContent(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \"0.1.0-beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\","); // Closing brace is missing

   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContent);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_CASE(NullOrEmptyPluginName)
{
   const std::string fileContentWithEmptyName(
      "{"
      "   \"name\": \"\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithEmptyName);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);

   const std::string fileContentWithoutName(
      "{"
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \"0.1.0-beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithoutName);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_CASE(NullOrEmptyPluginDescription)
{
   const std::string fileContentWithEmptyDescription(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"\","
      "   \"version\": \"0.1.0-beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithEmptyDescription);
   BOOST_REQUIRE_NO_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"));

   const std::string fileContentWithoutDescription(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"version\": \"0.1.0-beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithoutDescription);
   BOOST_REQUIRE_NO_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"));
}

BOOST_AUTO_TEST_CASE(BadPluginVersionFormat)
{
   const std::string fileContentWithEmptyVersion(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \"\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithEmptyVersion);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);

   const std::string fileContentWithoutVersion(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithoutVersion);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);

   const std::string fileContentWithBadVersion1(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \"0.\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithBadVersion1);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);

   const std::string fileContentWithBadVersion2(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \".1\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithBadVersion2);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);

   const std::string fileContentWithBadVersion3(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \"0.a\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithBadVersion3);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);

   const std::string fileContentWithBadVersion4(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \"12\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithBadVersion4);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_CASE(NullOrEmptyPluginAuthor)
{
   const std::string fileContentWithEmptyAuthor(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \"0.1.0-beta\","
      "   \"author\": \"\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithEmptyAuthor);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);

   const std::string fileContentWithoutAuthor(
      "{"
      "   \"type\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test pluginSystem::CInformation class.\","
      "   \"version\": \"0.1.0-beta\","
      "   \"url\": \"https://github.com/Yadoms/yadoms/\""
      "}");
   testCommon::filesystem::createFile(boost::filesystem::temp_directory_path() / "FakePackage" / "package.json", fileContentWithoutAuthor);
   BOOST_REQUIRE_THROW(pluginSystem::CInformation info(boost::filesystem::temp_directory_path() / "FakePackage"), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_SUITE_END()