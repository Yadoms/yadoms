#include "stdafx.h"
// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/plugin/information/Information.h"
#include "../../../../sources/shared/shared/exception/InvalidParameter.hpp"

#define BOOST_TEST_MODULE TestPluginInformation
#include <boost/test/unit_test.hpp>

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

static const std::string DefaultPackageFileContent(
"{"
"   \"name\": \"FakePackage\","
"      \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
"      \"version\": \"0.1\","
"      \"releaseType\": \"beta\","
"      \"author\": \"yadoms-team\","
"     \"url\": \"http://sourceforge.net/projects/yadoms/\","
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
   CTestPackageFile(const std::string& content)
   {
      // Create the package.json file
      testCommon::filesystem::WriteFile(std::string(), "package.json", content);
   }
   virtual ~CTestPackageFile()
   {
      // Clean-up
      testCommon::filesystem::RemoveFile("package.json", false);
   }
private:
   const std::string m_configFile;
};

BOOST_AUTO_TEST_CASE(ReadDefaultInformation)
{
   CTestPackageFile packageFile(DefaultPackageFileContent);
   shared::plugin::information::CInformation info("");// Test in local directory

   BOOST_CHECK_EQUAL(info.getName(), "FakePackage");
   BOOST_CHECK_EQUAL(info.getDescription(), "This is just a fake package file, used to test shared::plugin::information::CInformation class.");
   BOOST_CHECK_EQUAL(info.getVersion(), "0.1");
   BOOST_CHECK_EQUAL(info.getReleaseType(), (shared::plugin::information::EReleaseType)shared::plugin::information::kBeta);
   BOOST_CHECK_EQUAL(info.getAuthor(), "yadoms-team");
   BOOST_CHECK_EQUAL(info.getUrl(), "http://sourceforge.net/projects/yadoms/");
   BOOST_CHECK_EQUAL(info.toString(), "FakePackage v0.1[beta] by yadoms-team (http://sourceforge.net/projects/yadoms/)");
}

BOOST_AUTO_TEST_CASE(WrongPackageFileJsonFormat)
{
   const std::string fileContent(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\","); // Closing brace is missing

   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContent), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_CASE(NullOrEmptyPluginName)
{
   const std::string fileContentWithEmptyName(
      "{"
      "   \"name\": \"\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithEmptyName), shared::exception::CInvalidParameter);

   const std::string fileContentWithoutName(
      "{"
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithoutName), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_CASE(NullOrEmptyPluginDescription)
{
   const std::string fileContentWithEmptyDescription(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithEmptyDescription), shared::exception::CInvalidParameter);

   const std::string fileContentWithoutDescription(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithoutDescription), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_CASE(BadPluginVersionFormat)
{
   const std::string fileContentWithEmptyVersion(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithEmptyVersion), shared::exception::CInvalidParameter);

   const std::string fileContentWithoutVersion(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithoutVersion), shared::exception::CInvalidParameter);

   const std::string fileContentWithBadVersion1(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithBadVersion1), shared::exception::CInvalidParameter);

   const std::string fileContentWithBadVersion2(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \".1\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithBadVersion2), shared::exception::CInvalidParameter);

   const std::string fileContentWithBadVersion3(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.a\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithBadVersion3), shared::exception::CInvalidParameter);

   const std::string fileContentWithBadVersion4(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"12\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithBadVersion4), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_CASE(PluginInformationBadReleaseType)
{
   const std::string fileContentWithEmptyReleaseType(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithEmptyReleaseType), shared::exception::CInvalidParameter);

   const std::string fileContentWithoutReleaseType(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.1\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithoutReleaseType), shared::exception::CInvalidParameter);

   const std::string fileContentWithBadReleaseType(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"betas\","
      "   \"author\": \"yadoms-team\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithBadReleaseType), shared::exception::CInvalidParameter);
}

BOOST_AUTO_TEST_CASE(NullOrEmptyPluginAuthor)
{
   const std::string fileContentWithEmptyAuthor(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"beta\","
      "   \"author\": \"\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithEmptyAuthor), shared::exception::CInvalidParameter);

   const std::string fileContentWithoutAuthor(
      "{"
      "   \"name\": \"FakePackage\","
      "   \"description\": \"This is just a fake package file, used to test shared::plugin::information::CInformation class.\","
      "   \"version\": \"0.1\","
      "   \"releaseType\": \"beta\","
      "   \"url\": \"http://sourceforge.net/projects/yadoms/\""
      "}");
   BOOST_REQUIRE_THROW(shared::plugin::information::CInformation info(fileContentWithoutAuthor), shared::exception::CInvalidParameter);
}

