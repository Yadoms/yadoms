#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include <tools/DirectoryChangeListener.h>

// Includes needed to compile the test
#include "../../testCommon/fileSystem.h"
#include <boost/chrono.hpp>

BOOST_AUTO_TEST_SUITE(TestDirectoryChangeListener)


class CTestContext
{
public:
   CTestContext(const std::string& testDirectory)
      :m_testDirectory(testDirectory)
   {
      // Ensure TestDirectory is empty
      testCommon::filesystem::RemoveDirectory(m_testDirectory);
      testCommon::filesystem::CreateDirectory(m_testDirectory);
   }
   virtual ~CTestContext()
   {
      // Clean-up TestDirectory
      testCommon::filesystem::RemoveDirectory(m_testDirectory);
   }
private:
   const std::string& m_testDirectory;
};

static const std::string TestDirectory("test_directory");
static const std::string TestFile("test_file");
static const boost::filesystem::path ExpectedPath(boost::filesystem::path(TestDirectory) / boost::filesystem::path(TestFile));
std::vector<boost::asio::dir_monitor_event> Events;
boost::mutex EventsMutex;

//--------------------------------------------------------------
/// \brief	    The callback function, called when event is signaled on directory
//--------------------------------------------------------------
void onEventOnDirectory(const boost::asio::dir_monitor_event& evt)
{
   std::cout << "onEventOnDirectory : " << evt << std::endl;
   boost::mutex::scoped_lock lock(EventsMutex);
   Events.push_back(evt);
}

//--------------------------------------------------------------
/// \brief	    Test of new file creation
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(NewFile_Test)
{
   CTestContext context(TestDirectory);
   Events.clear();
   tools::CDirectoryChangeListener listener (TestDirectory, onEventOnDirectory);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

   testCommon::filesystem::CreateFile(TestDirectory, TestFile);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

   // Now, check results
   boost::mutex::scoped_lock lock(EventsMutex);
   BOOST_REQUIRE_EQUAL(Events.size(), (size_t)1);
   BOOST_CHECK_EQUAL(Events[0].path, ExpectedPath);
   BOOST_CHECK_EQUAL(Events[0].type, boost::asio::dir_monitor_event::added);
}

//--------------------------------------------------------------
/// \brief	    Test of file deletion
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(DeleteFile_Test)
{
   CTestContext context(TestDirectory);
   Events.clear();
   testCommon::filesystem::CreateFile(TestDirectory, TestFile);

   tools::CDirectoryChangeListener listener (TestDirectory, onEventOnDirectory);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
   testCommon::filesystem::RemoveFile(TestDirectory, TestFile);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

   // Now, check results
   boost::mutex::scoped_lock lock(EventsMutex);
   BOOST_REQUIRE_EQUAL(Events.size(), (size_t)1);
   BOOST_CHECK_EQUAL(Events[0].path, ExpectedPath);
   BOOST_CHECK_EQUAL(Events[0].type, boost::asio::dir_monitor_event::removed);
}

#if 0 // TODO : à remettre en place si on souhaite faire fonctionner CDirectoryChangeListener
//--------------------------------------------------------------
/// \brief	    Test of file modification
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ModifyFile_Test)
{
   CTestContext context(TestDirectory);
   Events.clear();
   testCommon::filesystem::CreateFile(TestDirectory, TestFile);

   tools::CDirectoryChangeListener listener (TestDirectory, onEventOnDirectory);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
   testCommon::filesystem::WriteFile(TestDirectory, TestFile);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

   // Now, check results (2 events : at file open, at file close)
   boost::mutex::scoped_lock lock(EventsMutex);
   BOOST_REQUIRE_EQUAL(Events.size(), (size_t)2);
   BOOST_CHECK_EQUAL(Events[0].path, ExpectedPath);
   BOOST_CHECK_EQUAL(Events[0].type, boost::asio::dir_monitor_event::modified);
   BOOST_CHECK_EQUAL(Events[1].path, ExpectedPath);
   BOOST_CHECK_EQUAL(Events[1].type, boost::asio::dir_monitor_event::modified);
}

//--------------------------------------------------------------
/// \brief	    Test of rename file
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RenameFile_Test)
{
   CTestContext context(TestDirectory);
   Events.clear();
   testCommon::filesystem::CreateFile(TestDirectory, TestFile);

   tools::CDirectoryChangeListener listener (TestDirectory, onEventOnDirectory);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
   const std::string newFileName(TestFile + "2");
   testCommon::filesystem::RenameFile(TestDirectory, TestFile, newFileName);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

   // Now, check results

   static const boost::filesystem::path ExpectedRenamedPath(boost::filesystem::path(TestDirectory) / boost::filesystem::path(newFileName));

   boost::mutex::scoped_lock lock(EventsMutex);
   BOOST_REQUIRE_EQUAL(Events.size(), (size_t)3);
   BOOST_CHECK_EQUAL(Events[0].path, ExpectedPath);
   BOOST_CHECK_EQUAL(Events[0].type, boost::asio::dir_monitor_event::renamed_old_name);
   BOOST_CHECK_EQUAL(Events[1].path, ExpectedRenamedPath);
   BOOST_CHECK_EQUAL(Events[1].type, boost::asio::dir_monitor_event::renamed_new_name);
   BOOST_CHECK_EQUAL(Events[2].path, ExpectedRenamedPath);
   BOOST_CHECK_EQUAL(Events[2].type, boost::asio::dir_monitor_event::modified);
}
#endif

BOOST_AUTO_TEST_SUITE_END()