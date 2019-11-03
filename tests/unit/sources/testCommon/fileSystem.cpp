#include "stdafx.h"
#include "fileSystem.h"
#include <boost/test/unit_test.hpp>

namespace testCommon
{
   void filesystem::createDirectory(const std::string& name)
   {
      const boost::filesystem::path dir(name.c_str());
      BOOST_REQUIRE(boost::filesystem::create_directory(dir)) ;
   }

   void filesystem::removeDirectory(const std::string& name)
   {
      boost::filesystem::remove_all(name.c_str());
   }

   void filesystem::createFile(const boost::filesystem::path& file,
                               const std::string& content,
                               bool withBom)
   {
      boost::filesystem::remove(file);

      if (!file.parent_path().empty() && !boost::filesystem::exists(file.parent_path()))
         boost::filesystem::create_directories(file.parent_path());

      if (withBom)
      {
         std::ofstream binaryStream;
         binaryStream.open(file.string(), std::ios::out | std::ios::binary);
         unsigned char bom[3] = {0xEF, 0xBB, 0xBF};
         binaryStream.write(reinterpret_cast<char*>(bom), sizeof bom);
         binaryStream.close();
      }

      std::ofstream textStream;
      textStream.open(file.string(), std::ios::out | (withBom ? std::ios::app : 0));
      textStream << content;
      textStream.close();
   }

   void filesystem::removeFile(const std::string& dir,
                               const std::string& file)
   {
      const auto fullPath = boost::filesystem::path(dir) / file;
      BOOST_REQUIRE(boost::filesystem::remove(fullPath)) ;
   }

   void filesystem::removeFile(const std::string& file,
                               bool successRequired)
   {
      if (successRequired)
         BOOST_REQUIRE(boost::filesystem::remove(file)) ;
      else
         boost::filesystem::remove(file);
   }

   void filesystem::removeFile(const boost::filesystem::path& file,
                               bool successRequired)
   {
      if (successRequired)
         BOOST_REQUIRE(boost::filesystem::remove(file.string())) ;
      else
         boost::filesystem::remove(file.string());
   }

   void filesystem::writeFile(const std::string& dir,
                              const std::string& file)
   {
      writeFile(dir, file, "some text...");
   }

   void filesystem::writeFile(const std::string& dir,
                              const std::string& file,
                              const std::string& content)
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

   void filesystem::writeFile(const boost::filesystem::path& file,
                              const std::string& content)
   {
      if (!file.parent_path().empty() && !boost::filesystem::exists(file.parent_path()))
         boost::filesystem::create_directories(file.parent_path());

      std::ofstream outfile(file.string(), std::ios_base::out);
      outfile << content;
   }

   void filesystem::renameFile(const std::string& dir,
                               const std::string& oldFile,
                               const std::string& newFile)
   {
      const auto oldFullPath = boost::filesystem::path(dir) / oldFile;
      const auto newFullPath = boost::filesystem::path(dir) / newFile;
      boost::filesystem::rename(oldFullPath, newFullPath);
   }
} // namespace testCommon
