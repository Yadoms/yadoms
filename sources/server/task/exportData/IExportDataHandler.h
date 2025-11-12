#pragma once

namespace task
{
   namespace exportData
   {
      class IExportDataHandler
      {
      public:
         virtual ~IExportDataHandler() = default;

         virtual std::string taskName() = 0;
         virtual int maxTries() = 0;
         virtual bool checkEnoughSpace(const boost::filesystem::path& tempFolder) = 0;
         virtual void collectDataTo(const boost::filesystem::path& tempFolder,
                                    std::function<void(int)> onProgressionPercentFct) = 0;
      };
   } //namespace exportData
} //namespace task
