#include "stdafx.h"
#include "FakeController.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

DECLARE_ENUM_IMPLEMENTATION(EFakeControllerValues,
   ((Stop))
   ((Run))
   ((Back))
   ((Left))
   ((Right))
)


DECLARE_CAPACITY(FakeControllerCapacity, "fakecontroller_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kEnum);

CControllerValue::CControllerValue(const std::string& keywordName)
   : CSingleHistorizableData<EFakeControllerValues>(keywordName,
                                                    FakeControllerCapacity(),
                                                    shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
{
}

CControllerValue::~CControllerValue()
{
}


CFakeController::CFakeController(const std::string& deviceName)
   : m_deviceName(deviceName),
     m_currentValues(boost::make_shared<CControllerValue>("controllerValue"))
{
}

CFakeController::~CFakeController()
{
}

void CFakeController::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, getType(), getModel());

   if (!api->keywordExists(m_deviceName, m_currentValues))
      api->declareKeyword(m_deviceName, m_currentValues);
}

void CFakeController::read() const
{
   if ((*m_currentValues)() == EFakeControllerValues::kStop)
      m_currentValues->set(EFakeControllerValues::kRun);
   else if ((*m_currentValues)() == EFakeControllerValues::kRun)
      m_currentValues->set(EFakeControllerValues::kBack);
   else if ((*m_currentValues)() == EFakeControllerValues::kBack)
      m_currentValues->set(EFakeControllerValues::kLeft);
   else if ((*m_currentValues)() == EFakeControllerValues::kLeft)
      m_currentValues->set(EFakeControllerValues::kRight);
   else if ((*m_currentValues)() == EFakeControllerValues::kRight)
      m_currentValues->set(EFakeControllerValues::kStop);
}

void CFakeController::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   api->historizeData(m_deviceName, m_currentValues);
}

const std::string& CFakeController::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeController::getModel()
{
   static const std::string model("Fake controller");
   return model;
}

const std::string& CFakeController::getType()
{
   static const std::string type("fakeControllerType");
   return type;
}

