#include "stdafx.h"
#include "fileSystem.h"

namespace testCommon
{
void filesystem::CreateDirectory(std::string name)
{
   boost::filesystem::path dir(name.c_str());
   BOOST_REQUIRE(boost::filesystem::create_directory(dir));
}

void filesystem::RemoveDirectory(std::string name)
{
   boost::filesystem::remove_all(name.c_str());
}

void filesystem::CreateFile(std::string dir, std::string file)
{
   boost::filesystem::path fullPath = boost::filesystem::path(dir) / file;
   std::ofstream outfile(fullPath.string().c_str(), std::ios_base::out);
   BOOST_REQUIRE(outfile.is_open());
}

void filesystem::RemoveFile(std::string dir, std::string file)
{
   boost::filesystem::path fullPath = boost::filesystem::path(dir) / file;
   BOOST_REQUIRE(boost::filesystem::remove(fullPath));
}

void filesystem::RemoveFile(std::string file, bool successRequired)
{
   if (successRequired)
      BOOST_REQUIRE(boost::filesystem::remove(file));
   else
      boost::filesystem::remove(file);
}

void filesystem::WriteFile(std::string dir, std::string file)
{
   WriteFile(dir, file, "some text...");
}

void filesystem::WriteFile(std::string dir, std::string file, std::string content)
{
   boost::filesystem::path fullPath;
   if (dir.empty())
      fullPath = file;
   else
      fullPath = boost::filesystem::path(dir) / file;

   std::ofstream outfile(fullPath.string().c_str(), std::ios_base::out);
   outfile << content;
}

void filesystem::RenameFile(std::string dir, std::string oldFile, std::string newFile)
{
   boost::filesystem::path oldFullPath = boost::filesystem::path(dir) / oldFile;
   boost::filesystem::path newFullPath = boost::filesystem::path(dir) / newFile;
   boost::filesystem::rename(oldFullPath, newFullPath);
}
} // namespace testCommon
