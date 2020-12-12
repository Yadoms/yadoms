#include "HueLightInformations.h"

CHueLightInformations::CHueLightInformations(const CHueState& state, const CHueSwUpdate& swUpdate,
                                             const std::string& type,
                                             const std::string& name, const std::string& modelId,
                                             const std::string& manufacturerName,
                                             const std::string& productName, const CHueCapabilities& capabilities,
                                             const CHueConfig& config,
                                             const std::string& uniqueId, const std::string& swVersion,
                                             const std::string& swConfigId,
                                             const std::string& productId)
   : m_state(state),
     m_swUpdate(swUpdate),
     m_type(type),
     m_name(name),
     m_modelId(modelId),
     m_manufacturerName(manufacturerName),
     m_productName(productName),
     m_capabilities(capabilities),
     m_config(config),
     m_uniqueId(uniqueId),
     m_swVersion(swVersion),
     m_swConfigId(swConfigId),
     m_productId(productId)
{
}

const CHueState& CHueLightInformations::getState() const
{
   return m_state;
}

const CHueSwUpdate& CHueLightInformations::getSwUpdate() const
{
   return m_swUpdate;
}

const std::string& CHueLightInformations::getType() const
{
   return m_type;
}

const std::string& CHueLightInformations::getName() const
{
   return m_name;
}

const std::string& CHueLightInformations::getModelId() const
{
   return m_modelId;
}

const std::string& CHueLightInformations::getManufacturerName() const
{
   return m_manufacturerName;
}

const std::string& CHueLightInformations::getProductName() const
{
   return m_productName;
}

const CHueCapabilities& CHueLightInformations::getCapabilities() const
{
   return m_capabilities;
}

const CHueConfig& CHueLightInformations::getConfig() const
{
   return m_config;
}

const std::string& CHueLightInformations::getUniqueId() const
{
   return m_uniqueId;
}

const std::string& CHueLightInformations::getSwVersion() const
{
   return m_swVersion;
}

const std::string& CHueLightInformations::getSwConfigId() const
{
   return m_swConfigId;
}

const std::string& CHueLightInformations::getProductId() const
{
   return m_productId;
}

void CHueLightInformations::setState(const CHueState& state)
{
   m_state = state;
}

void CHueLightInformations::setSwUpdate(const CHueSwUpdate& swUpdate)
{
   m_swUpdate = swUpdate;
}

void CHueLightInformations::setType(const std::string& type)
{
   m_type = type;
}

void CHueLightInformations::setName(const std::string& name)
{
   m_name = name;
}

void CHueLightInformations::setModelId(const std::string& modelId)
{
   m_modelId = modelId;
}

void CHueLightInformations::setManufacturerName(const std::string& manufacturerName)
{
   m_manufacturerName = manufacturerName;
}

void CHueLightInformations::setProductName(const std::string& productName)
{
   m_productName = productName;
}

void CHueLightInformations::setCapabilities(const CHueCapabilities& capabilities)
{
   m_capabilities = capabilities;
}

void CHueLightInformations::setConfig(const CHueConfig& config)
{
   m_config = config;
}

void CHueLightInformations::setUniqueId(const std::string& uniqueId)
{
   m_uniqueId = uniqueId;
}

void CHueLightInformations::setSwVersion(const std::string& swVersion)
{
   m_swVersion = swVersion;
}

void CHueLightInformations::setSwConfigId(const std::string& swConfigId)
{
   m_swConfigId = swConfigId;
}

void CHueLightInformations::setProductId(const std::string& productId)
{
   m_productId = productId;
}
