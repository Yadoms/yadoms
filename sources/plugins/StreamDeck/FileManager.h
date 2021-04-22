#pragma once

class CFileManager
{
public:
   CFileManager(std::string& path);
   virtual ~CFileManager() = default;

   void read();
   void close();
   std::string getData() const;
private:
   std::string m_path;
   std::ifstream m_ifs;
   std::string m_fileDataAsString;
};
