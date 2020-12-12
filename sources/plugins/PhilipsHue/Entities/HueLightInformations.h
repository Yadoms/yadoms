#pragma once
#include "HueState.h"
#include "HueSwUpdate.h"
#include "HueCapabilities.h"
#include "HueConfig.h"

class CHueLightInformations
{
public:

   CHueLightInformations() = delete;

   CHueLightInformations(const CHueState& state, const CHueSwUpdate& swUpdate, const std::string& type,
                         const std::string& name, const std::string& modelId, const std::string& manufacturerName,
                         const std::string& productName, const CHueCapabilities& capabilities, const CHueConfig& config,
                         const std::string& uniqueId, const std::string& swVersion, const std::string& swConfigId,
                         const std::string& productId);

   virtual ~CHueLightInformations() = default;

   const CHueState& getState() const;
   const CHueSwUpdate& getSwUpdate() const;
   const std::string& getType() const;
   const std::string& getName() const;
   const std::string& getModelId() const;
   const std::string& getManufacturerName() const;
   const std::string& getProductName() const;
   const CHueCapabilities& getCapabilities() const;
   const CHueConfig& getConfig() const;
   const std::string& getUniqueId() const;
   const std::string& getSwVersion() const;
   const std::string& getSwConfigId() const;
   const std::string& getProductId() const;

   void setState(const CHueState& state);
   void setSwUpdate(const CHueSwUpdate& swUpdate);
   void setType(const std::string& type);
   void setName(const std::string& name);
   void setModelId(const std::string& modelId);
   void setManufacturerName(const std::string& manufacturerName);
   void setProductName(const std::string& productName);
   void setCapabilities(const CHueCapabilities& capabilities);
   void setConfig(const CHueConfig& config);
   void setUniqueId(const std::string& uniqueId);
   void setSwVersion(const std::string& swVersion);
   void setSwConfigId(const std::string& swConfigId);
   void setProductId(const std::string& productId);

private:
   CHueState m_state;
   CHueSwUpdate m_swUpdate;
   std::string m_type;
   std::string m_name;
   std::string m_modelId;
   std::string m_manufacturerName;
   std::string m_productName;
   CHueCapabilities m_capabilities;
   CHueConfig m_config;
   std::string m_uniqueId;
   std::string m_swVersion;
   std::string m_swConfigId;
   std::string m_productId;
};
