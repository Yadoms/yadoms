#include "HueLightInformations.h"

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

const ELightType& CHueLightInformations::getModelId() const
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

const EColorType& CHueLightInformations::getColorType() const
{
   return m_colorType;
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
   if (modelId == ELightType::kLCT001.toString())
   {
      m_modelId = ELightType::kLCT001;
   }
   else if (modelId == ELightType::kLCT002.toString())
   {
      m_modelId = ELightType::kLCT002;
   }
   else if (modelId == ELightType::kLCT003.toString())
   {
      m_modelId = ELightType::kLCT003;
   }
   else if (modelId == ELightType::kLCT007.toString())
   {
      m_modelId = ELightType::kLCT007;
   }
   else if (modelId == ELightType::kLLM001.toString())
   {
      m_modelId = ELightType::kLLM001;
   }
   else if (modelId == ELightType::kLCT010.toString())
   {
      m_modelId = ELightType::kLCT010;
   }
   else if (modelId == ELightType::kLCT011.toString())
   {
      m_modelId = ELightType::kLCT011;
   }
   else if (modelId == ELightType::kLCT012.toString())
   {
      m_modelId = ELightType::kLCT012;
   }
   else if (modelId == ELightType::kLCT014.toString())
   {
      m_modelId = ELightType::kLCT014;
   }
   else if (modelId == ELightType::kLCT015.toString())
   {
      m_modelId = ELightType::kLCT015;
   }
   else if (modelId == ELightType::kLCT016.toString())
   {
      m_modelId = ELightType::kLCT016;
   }
   else if (modelId == ELightType::kLLC020.toString())
   {
      m_modelId = ELightType::kLLC020;
   }
   else if (modelId == ELightType::kLST002.toString())
   {
      m_modelId = ELightType::kLST002;
   }
   else if (modelId == ELightType::kLCA003.toString())
   {
      m_modelId = ELightType::kLCA003;
   }
   else if (modelId == ELightType::kLCB001.toString())
   {
      m_modelId = ELightType::kLCB001;
   }
   else if (modelId == ELightType::kLST001.toString())
   {
      m_modelId = ELightType::kLST001;
   }
   else if (modelId == ELightType::kLLC005.toString())
   {
      m_modelId = ELightType::kLLC005;
   }
   else if (modelId == ELightType::kLLC006.toString())
   {
      m_modelId = ELightType::kLLC006;
   }
   else if (modelId == ELightType::kLLC007.toString())
   {
      m_modelId = ELightType::kLLC007;
   }
   else if (modelId == ELightType::kLLC010.toString())
   {
      m_modelId = ELightType::kLLC010;
   }
   else if (modelId == ELightType::kLLC011.toString())
   {
      m_modelId = ELightType::kLLC011;
   }
   else if (modelId == ELightType::kLLC012.toString())
   {
      m_modelId = ELightType::kLLC012;
   }
   else if (modelId == ELightType::kLLC013.toString())
   {
      m_modelId = ELightType::kLLC013;
   }
   else if (modelId == ELightType::kLLC014.toString())
   {
      m_modelId = ELightType::kLLC014;
   }
   else
   {
      m_modelId = ELightType::kOther;
   }
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

void CHueLightInformations::setColorType(const EColorType& colorType)
{
   m_colorType = colorType;
}
