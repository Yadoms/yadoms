// Includes needed to compile tested classes
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include "../../sources/shared/shared/ThreadBase.h"
#include "../../sources/server/tools/tools/DirectoryChangeListener.h"

// Includes needed to compile the test
#include <boost/chrono.hpp>

#define BOOST_TEST_MODULE TestDirectoryChangeListener

#include <boost/test/unit_test.hpp>


void CreateDirectory(std::string name)
{
   boost::filesystem::path dir(name.c_str());
   BOOST_TEST_REQUIRE(boost::filesystem::create_directory(dir));
}

void RemoveDirectory(std::string name)
{
   boost::filesystem::remove_all(name.c_str());
}

void CreateFile(std::string dir, std::string file)
{
   boost::filesystem::path fullPath = boost::filesystem::path(dir) / file;
   std::ofstream outfile(fullPath.string());
   BOOST_TEST_REQUIRE(outfile.is_open());
}

void RemoveFile(std::string dir, std::string file)
{
   boost::filesystem::path fullPath = boost::filesystem::path(dir) / file;
   BOOST_TEST_REQUIRE(boost::filesystem::remove(fullPath));
}

void WriteFile(std::string dir, std::string file)
{
   boost::filesystem::path fullPath = boost::filesystem::path(dir) / file;
   std::ofstream outfile(fullPath.string());
   outfile << "some text...";
}

void RenameFile(std::string dir, std::string oldFile, std::string newFile)
{
   boost::filesystem::path oldFullPath = boost::filesystem::path(dir) / oldFile;
   boost::filesystem::path newFullPath = boost::filesystem::path(dir) / newFile;
   boost::filesystem::rename(oldFullPath, newFullPath);
}

class CTestContext
{
public:
   CTestContext(const std::string& testDirectory)
      :m_testDirectory(testDirectory)
   {
      // Ensure TestDirectory is empty
      RemoveDirectory(m_testDirectory);
      CreateDirectory(m_testDirectory);
   }
   virtual ~CTestContext()
   {
      // Clean-up TestDirectory
      RemoveDirectory(m_testDirectory);
   }
private:
   const std::string& m_testDirectory;
};

static const std::string TestDirectory("test_directory");
static const std::string TestFile("test_file");
std::vector<const boost::asio::dir_monitor_event> Events;
boost::mutex EventsMutex;

//--------------------------------------------------------------
/// \brief	    The callback function, called when event is signaled on directory
//--------------------------------------------------------------
void onEventOnDirectory(const boost::asio::dir_monitor_event& evt)
{
   std::cout << "onEventOnDirectory : " << evt.dirname << ", " << evt.filename << ", " << evt.type << std::endl;
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

   CreateFile(TestDirectory, TestFile);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

   // Now, check results
   boost::mutex::scoped_lock lock(EventsMutex);
   BOOST_REQUIRE_EQUAL(Events.size(), (size_t)1);
   BOOST_CHECK_EQUAL(Events[0].dirname, TestDirectory);
   BOOST_CHECK_EQUAL(Events[0].filename, TestFile);
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
   CreateFile(TestDirectory, TestFile);

   tools::CDirectoryChangeListener listener (TestDirectory, onEventOnDirectory);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
   RemoveFile(TestDirectory, TestFile);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

   // Now, check results
   boost::mutex::scoped_lock lock(EventsMutex);
   BOOST_REQUIRE_EQUAL(Events.size(), (size_t)1);
   BOOST_CHECK_EQUAL(Events[0].dirname, TestDirectory);
   BOOST_CHECK_EQUAL(Events[0].filename, TestFile);
   BOOST_CHECK_EQUAL(Events[0].type, boost::asio::dir_monitor_event::removed);
}

//--------------------------------------------------------------
/// \brief	    Test of file modification
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ModifyFile_Test)
{
   CTestContext context(TestDirectory);
   Events.clear();
   CreateFile(TestDirectory, TestFile);

   tools::CDirectoryChangeListener listener (TestDirectory, onEventOnDirectory);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
   WriteFile(TestDirectory, TestFile);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

   // Now, check results (2 events : at file open, at file close)
   boost::mutex::scoped_lock lock(EventsMutex);
   BOOST_REQUIRE_EQUAL(Events.size(), (size_t)2);
   BOOST_CHECK_EQUAL(Events[0].dirname, TestDirectory);
   BOOST_CHECK_EQUAL(Events[0].filename, TestFile);
   BOOST_CHECK_EQUAL(Events[0].type, boost::asio::dir_monitor_event::modified);
   BOOST_CHECK_EQUAL(Events[1].dirname, TestDirectory);
   BOOST_CHECK_EQUAL(Events[1].filename, TestFile);
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
   CreateFile(TestDirectory, TestFile);

   tools::CDirectoryChangeListener listener (TestDirectory, onEventOnDirectory);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
   const std::string newFileName (TestFile + "2");
   RenameFile(TestDirectory, TestFile, newFileName);
   boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

   // Now, check results
   boost::mutex::scoped_lock lock(EventsMutex);
   BOOST_REQUIRE_EQUAL(Events.size(), (size_t)3);
   BOOST_CHECK_EQUAL(Events[0].dirname, TestDirectory);
   BOOST_CHECK_EQUAL(Events[0].filename, TestFile);
   BOOST_CHECK_EQUAL(Events[0].type, boost::asio::dir_monitor_event::renamed_old_name);
   BOOST_CHECK_EQUAL(Events[1].dirname, TestDirectory);
   BOOST_CHECK_EQUAL(Events[1].filename, newFileName);
   BOOST_CHECK_EQUAL(Events[1].type, boost::asio::dir_monitor_event::renamed_new_name);
   BOOST_CHECK_EQUAL(Events[2].dirname, TestDirectory);
   BOOST_CHECK_EQUAL(Events[2].filename, newFileName);
   BOOST_CHECK_EQUAL(Events[2].type, boost::asio::dir_monitor_event::modified);
}
