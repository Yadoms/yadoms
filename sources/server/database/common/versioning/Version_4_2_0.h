#pragma once
#include "Version_4_1_0.h"
#include "database/IDatabaseRequester.h"


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Database version 4.2.0 update manager
         //
         // ReSharper disable once CppInconsistentNaming
         class CVersion_4_2_0 : public CVersion_4_1_0
         {
         public:
            CVersion_4_2_0() = default;
            ~CVersion_4_2_0() override = default;

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                 const shared::versioning::CSemVer& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         protected:
            static void updateDatabaseVersion(boost::shared_ptr<IDatabaseRequester> requester,
                                              const shared::versioning::CSemVer& newVersion,
                                              const boost::posix_time::ptime& insertDate = shared::currentTime::Provider().now());

         private:
            static const shared::versioning::CSemVer Version;

            // ReSharper disable once CppInconsistentNaming
            static void updateFrom4_1_0(const boost::shared_ptr<IDatabaseRequester>& requester);

            static boost::optional<bool> loadFirstStart(const boost::shared_ptr<IDatabaseRequester>& requester);
            static boost::optional<std::string> loadLocation(const boost::shared_ptr<IDatabaseRequester>& requester);
            static boost::optional<std::string> loadLanguage(const boost::shared_ptr<IDatabaseRequester>& requester);
            static boost::optional<bool> loadAdvancedParameters(const boost::shared_ptr<IDatabaseRequester>& requester);
            static boost::optional<std::string> loadDateFormatString(const boost::shared_ptr<IDatabaseRequester>& requester);
            static boost::optional<bool> loadRefreshPage(const boost::shared_ptr<IDatabaseRequester>& requester);
            static boost::optional<std::string> loadBasicAuthentication(const boost::shared_ptr<IDatabaseRequester>& requester);

            static boost::optional<boost::shared_ptr<shared::CDataContainer>> convertLocation(const boost::optional<std::string>& oldLocation);
            static boost::optional<boost::shared_ptr<shared::CDataContainer>> convertBasicAuthentication(
               const boost::optional<std::string>& oldBasicAuthentication);

            static void insertConfigurationValue(const boost::shared_ptr<IDatabaseRequester> requester,
                                                 const std::string& section,
                                                 const shared::CDataContainer& value,
                                                 const boost::posix_time::ptime& insertDate);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 
