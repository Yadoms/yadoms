#include "stdafx.h"
#include "OpenZWaveNodeUserCodePlugin.h"
#include "OpenZWaveCommandClass.h"
#include <Manager.h>
#include <command_classes/UserCode.h>
#include "OpenZWaveNodeKeywordFactory.h"
#include <boost/regex.hpp>
#include "OpenZWaveNode.h"

COpenZWaveNodeUserCodePlugin::COpenZWaveNodeUserCodePlugin(COpenZWaveNode * pMasterNode)
   :m_bEnrollmentMode(false), m_pMasterNode(pMasterNode)
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
      	//check both decimal or ascii value
         if ((size >= 2 && d[0] == 0 && d[1] == 0) || 
			 (size >= 4 && d[0] == '0' && d[1] == '0' && d[2] == '0' && d[3] == '0'))
         {
            delete d; //free memory

            YADOMS_LOG(information) << "Ignoring keyword " << vLabel << " : associated code is unused";
            //unused tag, do not create keyword
            return boost::shared_ptr<IOpenZWaveNodeKeyword>();
         }
         delete d; //free memory
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
std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > COpenZWaveNodeUserCodePlugin::onKeywordValueUpdated(OpenZWave::ValueID& vID)
{
   std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > result;
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
               OpenZWave::ValueID slotNode = m_codeSlots.at(slot);
               OpenZWave::Manager::Get()->SetValue(slotNode, d, size);
               m_pMasterNode->registerKeyword(slotNode, false);
               auto kw = m_pMasterNode->getKeyword(slotNode, false);
               if (kw)
               {
                  auto historizer = kw->getLastKeywordValue();
                  result.push_back(historizer);
               }
               else
               {
                  YADOMS_LOG(warning) << "Fail to get keyword for slot " << slot;
               }
            }
            delete d;
         }
      }
   }
   return result;
}

void COpenZWaveNodeUserCodePlugin::getExtraQueries(std::vector<std::string> & extraQueriesNames)
{
   extraQueriesNames.push_back("enroll");
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