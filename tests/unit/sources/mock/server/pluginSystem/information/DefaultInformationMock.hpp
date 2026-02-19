#pragma once
// Includes needed to compile tested classes
#include "../../../../../../../sources/shared/shared/plugin/information/IInformation.h"

class CDefaultInformationMock final : public shared::plugin::information::IInformation
{
public:
    ~CDefaultInformationMock() override = default;

    CDefaultInformationMock() = default;
    CDefaultInformationMock(const CDefaultInformationMock&) = delete;
    CDefaultInformationMock(CDefaultInformationMock&&) = delete;
    CDefaultInformationMock& operator=(const CDefaultInformationMock&) = delete;
    CDefaultInformationMock& operator=(CDefaultInformationMock&&) = delete;

    // shared::plugin::IInformation implementation
    const std::string& getType() const override
    {
        static const std::string Str("mock");
        return Str;
    }

    const shared::versioning::CSemVer& getVersion() const override
    {
        static const shared::versioning::CSemVer Version("1.2.3-beta");
        return Version;
    }

    const std::string& getAuthor() const override
    {
        static const std::string Str("YadomsTeam");
        return Str;
    }

    const std::string& getUrl() const override
    {
        static const std::string Str("www.yadoms.com");
        return Str;
    }

    std::string getIdentity() const override
    {
        std::ostringstream formattedInformation;
        formattedInformation << getType() << " v" << getVersion();
        return formattedInformation.str();
    }

    std::string toString() const override
    {
        std::ostringstream formattedInformation;
        formattedInformation << getIdentity() << " by " << getAuthor() << " (" << getUrl() << ")";
        return formattedInformation.str();
    }

    bool isSupportedOnThisPlatform() const override
    {
        return true;
    }

    bool getSupportManuallyCreatedDevice() const override
    {
        return false;
    }

    bool getSupportDeviceRemovedNotification() const override
    {
        return false;
    }

    boost::shared_ptr<const shared::CDataContainer> getPackage() const override
    {
        return {};
    }

    const boost::filesystem::path& getPath() const override
    {
        static boost::filesystem::path emptyPath; // NOLINT(clang-diagnostic-unique-object-duplication)
        return emptyPath;
    }

    boost::shared_ptr<const shared::CDataContainer> getConfigurationSchema() const override
    {
        return {};
    }

    boost::shared_ptr<const shared::CDataContainer> getLabels(const std::vector<std::string>& locales) const override
    {
        return {};
    }

    boost::shared_ptr<const shared::CDataContainer> getDeviceStaticConfigurationSchema() const override
    {
        return {};
    }

    boost::shared_ptr<const shared::CDataContainer> getDeviceDynamicConfigurationSchema() const override
    {
        return {};
    }

    // [END] shared::plugin::IInformation implementation
};
