#include "stdafx.h"
#include "OpenZWaveNodeUserCodePlugin.h"
#include "OpenZWaveCommandClass.h"
#include <Manager.h>
#include <command_classes/UserCode.h>
#include "OpenZWaveNodeKeywordFactory.h"
#include <boost/regex.hpp>

COpenZWaveNodeUserCodePlugin::COpenZWaveNodeUserCodePlugin()
   :m_bEnrollmentMode(false)
{
}

COpenZWaveNodeUserCodePlugin::~COpenZWaveNodeUserCodePlugin()
{
}

bool COpenZWaveNodeUserCodePlugin::isKeywordManagedByPlugin(OpenZWave::ValueID& vID, const uint32 homeId, const uint8 nodeId)
{
   ECommandClass commandClass(static_cast<int>(vID.GetCommandClassId()));
   if (commandClass == ECommandClass::kUserCode)
      return true;
   return false;
}

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeUserCodePlugin::createKeyword(OpenZWave::ValueID& vID, uint32 homeId, uint8 nodeId, bool includeSystemKeywords)
{
   auto vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);
   boost::regex reg("Code (\\d*):");
   boost::smatch match;

   //Set parameters
   if (boost::regex_search(vLabel, match, reg))
   {
      int slot = boost::lexical_cast<int>(match[1]);
      if (m_codeSlots.find(slot) != m_codeSlots.end())
         m_codeSlots.erase(slot);
      m_codeSlots.insert(std::make_pair(slot, vID));
      

      uint8 *d = NULL;
      uint8 size;
      if (OpenZWave::Manager::Get()->GetValueAsRaw(vID, &d, &size))
      {
         if (size >= 2 && d[0] == 0 && d[1] == 0)
         {
            delete d; //free memory
            //unused tag, do not create keyword
            return boost::shared_ptr<IOpenZWaveNodeKeyword>();
         }
      }
   }

   return  COpenZWaveNodeKeywordFactory::createKeyword(vID, homeId, nodeId, includeSystemKeywords);

}

int COpenZWaveNodeUserCodePlugin::findFirstFreeSlot()
{
   for (auto i = m_codeSlots.begin(); i != m_codeSlots.end(); ++i)
   {
      uint8 *d = NULL;
      uint8 size;
      if (OpenZWave::Manager::Get()->GetValueAsRaw(i->second, &d, &size))
      {
         if (size >= 2 && d[0] == 0 && d[1] == 0)
         {
            delete d; //free memory
            return i->first;
         }
         else
         {
            delete d; //free memory
         }
      }
   }
   return -1;
}
void COpenZWaveNodeUserCodePlugin::onKeywordValueUpdated(OpenZWave::ValueID& vID)
{
   if (m_bEnrollmentMode)
   {
      auto vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);
      if (boost::istarts_with(vLabel, "Enrollment Code"))
      {
         uint8 *d = NULL;
         uint8 size;
         if (OpenZWave::Manager::Get()->GetValueAsRaw(vID, &d, &size))
         {
            int slot = findFirstFreeSlot();
            if (slot >= 0)
            {
               OpenZWave::ValueID vNode = m_codeSlots.at(slot);
               OpenZWave::Manager::Get()->SetValue(vNode, d, size);
            }
            delete d;
         }
      }
   }
}

void COpenZWaveNodeUserCodePlugin::getExtraQueries(std::vector<shared::CDataContainer> & extraQueriesNames)
{
   shared::CDataContainer a;
   a.set("name", "enroll");
   extraQueriesNames.push_back(a);
}

std::string COpenZWaveNodeUserCodePlugin::getName()
{
   return ECommandClass::kUserCode;
}

bool COpenZWaveNodeUserCodePlugin::onExtraQuery(const std::string & query, const shared::CDataContainer &data)
{
   if (boost::iequals(query, "enroll"))
   {
      m_bEnrollmentMode = true;
      return true;
   }
   return false;
}