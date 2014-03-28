
#include <boost/filesystem.hpp>

namespace testCommon
{
   class filesystem
   {
   public:
      static void CreateDirectory(std::string name);

      static void RemoveDirectory(std::string name);

      static void CreateFile(std::string dir, std::string file);

      static void RemoveFile(std::string dir, std::string file);
      static void RemoveFile(std::string file, bool successRequired);

      static void WriteFile(std::string dir, std::string file);

      static void WriteFile(std::string dir, std::string file, std::string content);

      static void RenameFile(std::string dir, std::string oldFile, std::string newFile);
   };

} // namespace testCommon
