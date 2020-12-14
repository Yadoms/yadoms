#pragma once
class CHueInformations final
{
public:
   CHueInformations();
   CHueInformations(std::string& ip, std::string& friendlyName, std::string& modelName, std::string& modelNumber);
   virtual ~CHueInformations() = default;

   const std::string& getIp() const;
   const std::string& getFriendlyName() const;
   const std::string& getModelName() const;
   const std::string& getModelNumber() const;

   void setIp(const std::string& ip);
   void setFriendlyName(const std::string& friendlyName);
   void setModelName(const std::string& modelName);
   void setModelNumber(const std::string& modelNumber);
private:
   std::string m_ip;
   std::string m_friendlyName;
   std::string m_modelName;
   std::string m_modelNumber;
};

