#include "stdafx.h"
#include "fileSystem.h"
#include <boost/test/unit_test.hpp>

namespace testCommon
{
   void filesystem::CreateDirectory(std::string name)
   {
      boost::filesystem::path dir(name.c_str());
      BOOST_REQUIRE(boost::filesystem::create_directory(dir)) ;
   }

   void filesystem::RemoveDirectory(std::string name)
   {
      boost::filesystem::remove_all(name.c_str());
   }

   void filesystem::CreateFile(std::string dir,
                               std::string file)
   {
      auto fullPath = boost::filesystem::path(dir) / file;
      std::ofstream outfile(fullPath.string().c_str(), std::ios_base::out);
      BOOST_REQUIRE(outfile.is_open()) ;
   }

   void filesystem::RemoveFile(std::string dir,
                               std::string file)
   {
      auto fullPath = boost::filesystem::path(dir) / file;
      BOOST_REQUIRE(boost::filesystem::remove(fullPath)) ;
   }

   void filesystem::RemoveFile(std::string file,
                               bool successRequired)
   {
      if (successRequired)
      BOOST_REQUIRE(boost::filesystem::remove(file)) ;
      else
         boost::filesystem::remove(file);
   }

   void filesystem::RemoveFile(const boost::filesystem::path& file,
                               bool successRequired)
   {
      if (successRequired)
      BOOST_REQUIRE(boost::filesystem::remove(file.string())) ;
      else
         boost::filesystem::remove(file.string());
   }

   void filesystem::WriteFile(std::string dir, std::string file)
   {
      WriteFile(dir, file, "some text...");
   }

   void filesystem::WriteFile(std::string dir,
                              std::string file,
                              std::string content)
   {
      boost::filesystem::path fullPath;
      if (dir.empty())
         fullPath = file;
      else
      {
         boost::filesystem::create_directories(dir);
         fullPath = boost::filesystem::path(dir) / file;
      }

      std::ofstream outfile(fullPath.string().c_str(), std::ios_base::out);
      outfile << content;
   }

   void filesystem::WriteFile(const boost::filesystem::path& file,
                              std::string content)
   {
      if (!file.parent_path().empty() && !boost::filesystem::exists(file.parent_path()))
         boost::filesystem::create_directories(file.parent_path());

      std::ofstream outfile(file.string(), std::ios_base::out);
      outfile << content;
   }

   void filesystem::RenameFile(std::string dir,
                               std::string oldFile,
                               std::string newFile)
   {
      auto oldFullPath = boost::filesystem::path(dir) / oldFile;
      auto newFullPath = boost::filesystem::path(dir) / newFile;
      boost::filesystem::rename(oldFullPath, newFullPath);
   }
} // namespace testCommon
