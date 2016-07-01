#include "stdafx.h"
#include "Transceiver.h"
#include <shared/DataContainer.h>
#include "IncrementSequenceNumber.h"
#include "TeleInfotrxHelpers.h"

CTransceiver::CTransceiver()
   : m_seqNumberProvider(boost::make_shared<CIncrementSequenceNumber>()),
     m_deviceCreated(false),
     m_optarif(OP_NOT_DEFINED)
{
   CTransceiver::ResetRefreshTags();
}

CTransceiver::~CTransceiver()
{
}

void CTransceiver::decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                         const shared::communication::CByteBuffer& data)
{
   auto buf = reinterpret_cast<const unsigned char *>(data.begin());

   m_api = api;

   ParseData(buf, data.size());
}

/* Explanation of the checksum computation issued from the official EDF specification

A "checksum" is calculated on the set of characters from the beginning of the label field to the end of the field given character SP included.
We first make ??the sum of all ASCII codes of all characters.
to avoid introduce ASCII (00 to 1F hex) functions, it retains only the six least significant bits of
result (this translates into a logical AND between the amount previously calculated and 03Fh).
Finally, we added 20 hexadecimal. The result will always be a printable ASCII character (sign, digit,
capital letter) of from 0x20 to hexadecimal 0x5F

La "checksum" est calcul� sur l'ensemble des caract�res allant du d�but du champ �tiquette � la fin du champ
donn�e, caract�re SP inclus. On fait tout d'abord la somme des codes ASCII de tous ces caract�res. Pour �viter
d'introduire des fonctions ASCII (00 � 1F en hexad�cimal), on ne conserve que les six bits de poids faible du
r�sultat obtenu (cette op�ration se traduit par un ET logique entre la somme pr�c�demment calcul�e et 03Fh).
Enfin, on ajoute 20 en hexad�cimal. Le r�sultat sera donc toujours un caract�re ASCII imprimable (signe, chiffre,
lettre majuscule) allant de 20 � 5F en hexad�cimal.
*/

bool CTransceiver::isCheckSumOk(const unsigned char* buffer)
{
   unsigned int checksum = 0x00;
   int i;

   for (i = 0; i < int(strlen((char*)buffer)) - 2; i++)
   {
      checksum += buffer[i];
   }
   checksum = (checksum & 0x3F) + 0x20;
   return (checksum == buffer[strlen((char*)buffer) - 1]);
}

void CTransceiver::ParseData(const unsigned char* pData,
                             int Len)
{
   auto ii = 0;
   unsigned char buffer[readBufferSize];
   auto m_bufferpos = 0;

   while (ii < Len)
   {
      const auto c = pData[ii];

      if ((c == 0x0a) || (c == 0x00))
      {
         ii++;
         m_bufferpos = 0;
         continue;
      }

      buffer[m_bufferpos] = c;
      if (c == 0x0d || m_bufferpos == sizeof(buffer) - 1)
      {
         // discard newline, close string, parse line and clear it.
         if (m_bufferpos > 0)
            buffer[m_bufferpos] = 0;

         //We check the line only if the checksum is ok
         if (isCheckSumOk(buffer))
            MatchLine(buffer);

         m_bufferpos = 0;
      }
      else
      {
         m_bufferpos++;
      }
      ii++;
   }
   //historizing all information push in the list
   m_api->historizeData(m_DeviceName, m_KeywordList);

   //Clear the vector
   m_KeywordList.clear();
}

template <class T>
void CTransceiver::HistorizeTeleInfoData(std::string KeywordName,
                                         long Value)
{
   if (m_deviceCreated)
   {
      auto m_keyword = boost::make_shared<T>(KeywordName);

      if (!m_api->keywordExists(m_DeviceName,
                                m_keyword))
         m_api->declareKeyword(m_DeviceName,
                               m_keyword,
                               m_KeywordDetails);

      m_keyword->set(Value);
      std::cout << m_keyword->getKeyword() << "=" << m_keyword->get() << std::endl;
      m_KeywordList.push_back(m_keyword);
   }
}

void CTransceiver::CreateDevice(std::string CounterId)
{
   m_DeviceName = CounterId;

   if (!m_api->deviceExists(CounterId))
      m_api->declareDevice(CounterId,
                           "TeleInfoUSB : Id = " + CounterId,
                           std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>(),
                           m_DeviceDetails);

   //Set the counter id for each keyword
   m_KeywordDetails.set("id", CounterId);

   m_deviceCreated = true;
}

void CTransceiver::ResetRefreshTags()
{
   m_baseUpdated = false;
   m_lowCostUpdated = false;
   m_normalCostUpdated = false;
   m_instantCurrentUpdated = false;
   m_apparentPowerUpdated = false;

   m_eJPPeakPeriodUpdated = false;
   m_eJPNormalPeriodUpdated = false;

   m_tempoBlueDaysLowCostUpdated = false;
   m_tempoBlueDaysNormalCostUpdated = false;

   m_tempoWhiteDaysLowCostUpdated = false;
   m_tempoWhiteDaysNormalCostUpdated = false;

   m_tempoRedDaysLowCostUpdated = false;
   m_tempoRedDaysNormalCostUpdated = false;

   m_timePeriodUpdated = false;
   m_forecastPeriodUpdated = false;
}

bool CTransceiver::IsInformationUpdated()
{
   // We do not take account about the apparent power. Some counters may not have this tag
   if ((((m_optarif == OP_BASE) && (m_baseUpdated)) ||
         ((m_optarif == OP_CREUSE) && (m_lowCostUpdated) && (m_normalCostUpdated)) ||
         ((m_optarif == OP_EJP) && (m_eJPPeakPeriodUpdated) && (m_eJPNormalPeriodUpdated)) ||
         ((m_optarif == OP_TEMPO) && (m_tempoBlueDaysLowCostUpdated) && (m_tempoBlueDaysNormalCostUpdated) && (m_tempoWhiteDaysLowCostUpdated) && (m_tempoWhiteDaysNormalCostUpdated) && (m_tempoRedDaysLowCostUpdated) && (m_tempoRedDaysNormalCostUpdated) && (m_forecastPeriodUpdated))
      )
      && (m_timePeriodUpdated)
      && (m_instantCurrentUpdated))
      return true;

   return false;
}

void CTransceiver::MatchLine(const unsigned char* buffer)
{
   static const EnumValuesTypes EEnumValuesTypes = boost::assign::map_list_of
      (TE_ADCO, TELEINFO_TYPE_ADCO)
      (TE_OPTARIF, TELEINFO_TYPE_OPTARIF)
      (TE_ISOUSC, TELEINFO_TYPE_ISOUSC)
      (TE_BASE, TELEINFO_TYPE_BASE)
      (TE_HCHC, TELEINFO_TYPE_HCHC)
      (TE_HCHP, TELEINFO_TYPE_HCHP)
      (TE_EJPHPM, TELEINFO_TYPE_EJPHPM)
      (TE_EJPHN, TELEINFO_TYPE_EJPHN)
      (TE_BBRHCJB, TELEINFO_TYPE_BBRHCJB)
      (TE_BBRHPJB, TELEINFO_TYPE_BBRHPJB)
      (TE_BBRHCJW, TELEINFO_TYPE_BBRHCJW)
      (TE_BBRHPJW, TELEINFO_TYPE_BBRHPJW)
      (TE_BBRHCJR, TELEINFO_TYPE_BBRHCJR)
      (TE_BBRHPJR, TELEINFO_TYPE_BBRHPJR)
      (TE_PTEC, TELEINFO_TYPE_PTEC)
      (TE_IINST, TELEINFO_TYPE_IINST)
      (TE_IMAX, TELEINFO_TYPE_IMAX)
      (TE_PAPP, TELEINFO_TYPE_PAPP)
      (TE_DEMAIN, TELEINFO_TYPE_DEMAIN)
      (TE_HHPHC, TELEINFO_TYPE_HHPHC)
      (TE_ADPS, TELEINFO_TYPE_ADPS)
      (TE_MOTDETAT, TELEINFO_TYPE_MOTDETAT);

   if ((strlen(reinterpret_cast<const char*>(buffer)) < 1) || (buffer[0] == 0x0a))
      return;

   Match t[22] = {
      {12}, //TELEINFO_TYPE_ADCO
      {4}, //TELEINFO_TYPE_OPTARIF
      {2}, //TELEINFO_TYPE_ISOUSC
      {9}, //TELEINFO_TYPE_BASE
      {9}, //TELEINFO_TYPE_HCHC
      {9}, //TELEINFO_TYPE_HCHP
      {9}, //TELEINFO_TYPE_EJPHPM
      {9}, //TELEINFO_TYPE_EJPHN
      {9}, //TELEINFO_TYPE_BBRHCJB
      {9}, //TELEINFO_TYPE_BBRHPJB
      {9}, //TELEINFO_TYPE_BBRHCJW
      {9}, //TELEINFO_TYPE_BBRHPJW
      {9}, //TELEINFO_TYPE_BBRHCJR
      {9}, //TELEINFO_TYPE_BBRHPJR
      {4}, //TELEINFO_TYPE_PTEC
      {3}, //TELEINFO_TYPE_IINST
      {3}, //TELEINFO_TYPE_IMAX
      {5}, //TELEINFO_TYPE_PAPP
      {4}, //TELEINFO_TYPE_DEMAIN 
      {1}, //TELEINFO_TYPE_HHPHC
      {3}, //TELEINFO_TYPE_ADPS
      {6} //TELEINFO_TYPE_MOTDETAT
   };

   //We get the id --> This function could be adjust for the following
   auto pos = reinterpret_cast<unsigned char *>(strchr((char*)buffer, ' '));
   if (pos == nullptr)
      return;
   auto position = int(pos - buffer);
   char id[15] = "";
   strncpy(id, (char*)buffer, position);
   id[position] = 0;

   EnumValuesTypes::const_iterator it = EEnumValuesTypes.find(id);
   if (it != EEnumValuesTypes.end())
   {
      //We get the width car after the space
      pos = reinterpret_cast<unsigned char *>(strchr((char*)buffer, ' '));
      if (pos == nullptr)
         return;
      position = int(pos - const_cast<unsigned char*>(buffer));
      char value[20] = "";
      strncpy(value, reinterpret_cast<const char*>(&buffer[position + 1]), t[it->second].width);
      value[t[it->second].width] = 0;

      long lvalue = 0;
      bool lvalueIsANumber;
      try
      {
         lvalue = boost::lexical_cast<long>(value);
         lvalueIsANumber = true;
      }
      catch (...)
      {
         lvalueIsANumber = false;
      }

      static auto ADCORead = false;
      static auto OpTarifRead = false;
      static auto ISousRead = false;

      switch (it->second)
      {
      case TELEINFO_TYPE_ADCO:
         if (!ADCORead)
         {
            std::cout << "ADCO" << "=" << value << std::endl;

            CreateDevice(value);

            ADCORead = true;
         }
         break;
      case TELEINFO_TYPE_OPTARIF:
         if (!OpTarifRead)
         {
            std::cout << "OPTARIF" << "=" << value << std::endl;
            OpTarifRead = true;

            switch (value[1])
            {
            case 'A':
               m_optarif = OP_BASE;
               break;
            case 'C':
               m_optarif = OP_CREUSE;
               break;
            case 'J':
               m_optarif = OP_EJP;
               break;
            case 'B':
               m_optarif = OP_TEMPO;
               break;
            default:
               //Erreur normalement
               break;
            }
         }
         break;
      case TELEINFO_TYPE_ISOUSC:
         if (!ISousRead)
         {
            std::cout << "ISOUSC" << "=" << value << std::endl;
            ISousRead = true;
         }
         break;
      case TELEINFO_TYPE_BASE:
         if (lvalueIsANumber && !m_baseUpdated)
         {
            std::cout << "BASE" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("BaseCounter", lvalue);
            m_baseUpdated = true;
         }
         break;
      case TELEINFO_TYPE_HCHC:
         if (lvalueIsANumber && !m_lowCostUpdated)
         {
            std::cout << "HCHC" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("LowCostCounter", lvalue);
            m_lowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_HCHP:
         if (lvalueIsANumber && !m_normalCostUpdated)
         {
            std::cout << "HCHP" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("NormalCostCounter", lvalue);
            m_normalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_EJPHPM:
         if (lvalueIsANumber && !m_eJPPeakPeriodUpdated)
         {
            std::cout << "EJPHPM" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("EJPPeakPeriod", lvalue);
            m_eJPPeakPeriodUpdated = true;
         }
         break;
      case TELEINFO_TYPE_EJPHN:
         if (lvalueIsANumber && !m_eJPNormalPeriodUpdated)
         {
            std::cout << "EJPHPN" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("EJPNormalPeriod", lvalue);
            m_eJPNormalPeriodUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHCJB:
         if (lvalueIsANumber && !m_tempoBlueDaysLowCostUpdated)
         {
            std::cout << "BBRHCJB" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoBlueDaysLowCostPeriod", lvalue);
            m_tempoBlueDaysLowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHPJB:
         if (lvalueIsANumber && !m_tempoBlueDaysNormalCostUpdated)
         {
            std::cout << "BBRHPJB" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoBlueDaysNormalCostPeriod", lvalue);
            m_tempoBlueDaysNormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHCJW:
         if (lvalueIsANumber && !m_tempoWhiteDaysLowCostUpdated)
         {
            std::cout << "BBRHCJW" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoWhiteDaysLowCostPeriod", lvalue);
            m_tempoWhiteDaysLowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHPJW:
         if (lvalueIsANumber && !m_tempoWhiteDaysNormalCostUpdated)
         {
            std::cout << "BBRHPJW" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoWhiteDaysNormalCostPeriod", lvalue);
            m_tempoWhiteDaysNormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHCJR:
         if (lvalueIsANumber && !m_tempoRedDaysLowCostUpdated)
         {
            std::cout << "BBRHCJR" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoRedDaysLowCostPeriod", lvalue);
            m_tempoRedDaysLowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHPJR:
         if (lvalueIsANumber && !m_tempoRedDaysNormalCostUpdated)
         {
            std::cout << "BBRHPJR" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoRedDaysNormalCostPeriod", lvalue);
            m_tempoRedDaysNormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_PTEC:
         if (!m_timePeriodUpdated && m_deviceCreated)
         {
            std::cout << "PTEC" << "=" << value << std::endl;

            m_TimePeriod = boost::make_shared<CRunningPeriod>(m_api, m_DeviceName, "RunningPeriod", m_KeywordDetails);
            std::string temp(value);
            m_TimePeriod->SetValue(temp);
            m_KeywordList.push_back(m_TimePeriod->GetHistorizable());
            m_timePeriodUpdated = true;
         }
         break;
      case TELEINFO_TYPE_IINST:
         if (lvalueIsANumber && !m_instantCurrentUpdated)
         {
            std::cout << "IINST" << "=" << value << std::endl;
            HistorizeTeleInfoData<yApi::historization::CCurrent>("InstantCurrent", lvalue);
            m_instantCurrentUpdated = true;
         }
         break;
      case TELEINFO_TYPE_IMAX: //We do not use this one also
         break;
      case TELEINFO_TYPE_PAPP:
         if (lvalueIsANumber && !m_apparentPowerUpdated)
         {
            std::cout << "PAPP" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CApparentPower>("ApparentPower", lvalue);
            m_apparentPowerUpdated = true;
         }
         break;

      case TELEINFO_TYPE_HHPHC: //No interest ! Used by the distributor.
         std::cout << "HHPHC" << "=" << value << std::endl;
         break;

      case TELEINFO_TYPE_DEMAIN:
         if (!m_forecastPeriodUpdated && m_deviceCreated)
         {
            std::cout << "DEMAIN" << "=" << value << std::endl;

            m_ForecastPeriod = boost::make_shared<CForecastTomorrow>(m_api,
                                                                     m_DeviceName,
                                                                     "ForecastColor",
                                                                     m_KeywordDetails);
            std::string temp(value);
            m_ForecastPeriod->SetValue(temp);
            m_KeywordList.push_back(m_ForecastPeriod->GetHistorizable());
            m_forecastPeriodUpdated = true;
         }
         break;

      case TELEINFO_TYPE_ADPS: // Threshold warning ! If IINST > ISOUSC
         std::cout << "ADPS" << "=" << value << std::endl;
         break;

      case TELEINFO_TYPE_MOTDETAT:
         // This value is for the distributor. It's nevers used
         std::cout << "MOTDETAT" << "=" << value << std::endl;
         break;
      default:
         break;
      }
   }
}
