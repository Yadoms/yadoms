#include "stdafx.h"
#include "inputOutput.h"

namespace specificHistorizers
{
   CInputOuput::CInputOuput(const std::string& keywordName,
                            const std::string& hardwareName,
                            const yApi::EKeywordAccessMode& accessMode)
      : CSingleHistorizableData<bool>(keywordName,
                                      yApi::CStandardCapacities::Switch(),
                                      accessMode,
                                      false,
                                      yApi::EMeasureType::kAbsolute),
        m_hardwareName(hardwareName)
   {
   }

   std::string CInputOuput::getHardwareName() const
   {
      return m_hardwareName;
   }

   void CInputOuput::setNewHardwareName(const std::string& newName)
   {
      m_hardwareName = newName;
   }

   CInputOuput::~CInputOuput()
   {
   }
} // namespace specificHistorizers


