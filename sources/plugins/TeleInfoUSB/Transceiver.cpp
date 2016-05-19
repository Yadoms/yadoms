#include "stdafx.h"
#include "Transceiver.h"
#include <shared/DataContainer.h>
#include "IncrementSequenceNumber.h"
#include "TeleInfotrxHelpers.h"

CTransceiver::CTransceiver()
   : m_seqNumberProvider(new CIncrementSequenceNumber()),
     m_deviceCreated(false),
     Optarif(OP_NOT_DEFINED)
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

La "checksum" est calcul»e sur l'ensemble des caractÀres allant du d»but du champ »tiquette á la fin du champ
donn»e, caractÀre SP inclus. On fait tout d'abord la somme des codes ASCII de tous ces caractÀres. Pour »viter
d'introduire des fonctions ASCII (00 á 1F en hexad»cimal), on ne conserve que les six bits de poids faible du
r»sultat obtenu (cette op»ration se traduit par un ET logique entre la somme pr»c»demment calcul»e et 03Fh).
Enfin, on ajoute 20 en hexad»cimal. Le r»sultat sera donc toujours un caractÀre ASCII imprimable (signe, chiffre,
lettre majuscule) allant de 20 á 5F en hexad»cimal.
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

void CTransceiver::ParseData(const unsigned char* pData, int Len)
{
   int ii = 0;
   unsigned char buffer[readBufferSize];
   int m_bufferpos = 0;

   while (ii < Len)
   {
      const unsigned char c = pData[ii];

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
void CTransceiver::HistorizeTeleInfoData(std::string KeywordName, long Value)
{
   if (m_deviceCreated)
   {
      boost::shared_ptr<T> m_keyword;

      m_keyword.reset(new T(KeywordName));

      if (!m_api->keywordExists(m_DeviceName, m_keyword))
         m_api->declareKeyword(m_DeviceName, m_keyword, m_KeywordDetails);

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
                           std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >(),
                           m_DeviceDetails);

   //Set the counter id for each keyword
   m_KeywordDetails.set("id", CounterId);

   m_deviceCreated = true;
}

void CTransceiver::ResetRefreshTags()
{
   baseUpdated = false;
   LowCostUpdated = false;
   NormalCostUpdated = false;
   InstantCurrentUpdated = false;
   ApparentPowerUpdated = false;

   EJPPeakPeriodUpdated = false;
   EJPNormalPeriodUpdated = false;

   TempoBlueDaysLowCostUpdated = false;
   TempoBlueDaysNormalCostUpdated = false;

   TempoWhiteDaysLowCostUpdated = false;
   TempoWhiteDaysNormalCostUpdated = false;

   TempoRedDaysLowCostUpdated = false;
   TempoRedDaysNormalCostUpdated = false;

   TimePeriodUpdated = false;
   ForecastPeriodUpdated = false;
}

bool CTransceiver::IsInformationUpdated()
{
   // We do not take account about the apparent power. Some counters may not have this tag
   if ((((Optarif == OP_BASE) && (baseUpdated)) ||
         ((Optarif == OP_CREUSE) && (LowCostUpdated) && (NormalCostUpdated)) ||
         ((Optarif == OP_EJP) && (EJPPeakPeriodUpdated) && (EJPNormalPeriodUpdated)) ||
         ((Optarif == OP_TEMPO) && (TempoBlueDaysLowCostUpdated) && (TempoBlueDaysNormalCostUpdated) && (TempoWhiteDaysLowCostUpdated) && (TempoWhiteDaysNormalCostUpdated) && (TempoRedDaysLowCostUpdated) && (TempoRedDaysNormalCostUpdated) && (ForecastPeriodUpdated))
      )
      && (TimePeriodUpdated)
      && (InstantCurrentUpdated))
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

   if ((strlen((const char*)buffer) < 1) || (buffer[0] == 0x0a))
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
   unsigned char* pos = reinterpret_cast<unsigned char *>(strchr((char*)buffer, ' '));
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
               Optarif = OP_BASE;
               break;
            case 'C':
               Optarif = OP_CREUSE;
               break;
            case 'J':
               Optarif = OP_EJP;
               break;
            case 'B':
               Optarif = OP_TEMPO;
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
         if (lvalueIsANumber && !baseUpdated)
         {
            std::cout << "BASE" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("BaseCounter", lvalue);
            baseUpdated = true;
         }
         break;
      case TELEINFO_TYPE_HCHC:
         if (lvalueIsANumber && !LowCostUpdated)
         {
            std::cout << "HCHC" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("LowCostCounter", lvalue);
            LowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_HCHP:
         if (lvalueIsANumber && !NormalCostUpdated)
         {
            std::cout << "HCHP" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("NormalCostCounter", lvalue);
            NormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_EJPHPM:
         if (lvalueIsANumber && !EJPPeakPeriodUpdated)
         {
            std::cout << "EJPHPM" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("EJPPeakPeriod", lvalue);
            EJPPeakPeriodUpdated = true;
         }
         break;
      case TELEINFO_TYPE_EJPHN:
         if (lvalueIsANumber && !EJPNormalPeriodUpdated)
         {
            std::cout << "EJPHPN" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("EJPNormalPeriod", lvalue);
            EJPNormalPeriodUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHCJB:
         if (lvalueIsANumber && !TempoBlueDaysLowCostUpdated)
         {
            std::cout << "BBRHCJB" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoBlueDaysLowCostPeriod", lvalue);
            TempoBlueDaysLowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHPJB:
         if (lvalueIsANumber && !TempoBlueDaysNormalCostUpdated)
         {
            std::cout << "BBRHPJB" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoBlueDaysNormalCostPeriod", lvalue);
            TempoBlueDaysNormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHCJW:
         if (lvalueIsANumber && !TempoWhiteDaysLowCostUpdated)
         {
            std::cout << "BBRHCJW" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoWhiteDaysLowCostPeriod", lvalue);
            TempoWhiteDaysLowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHPJW:
         if (lvalueIsANumber && !TempoWhiteDaysNormalCostUpdated)
         {
            std::cout << "BBRHPJW" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoWhiteDaysNormalCostPeriod", lvalue);
            TempoWhiteDaysNormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHCJR:
         if (lvalueIsANumber && !TempoRedDaysLowCostUpdated)
         {
            std::cout << "BBRHCJR" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoRedDaysLowCostPeriod", lvalue);
            TempoRedDaysLowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHPJR:
         if (lvalueIsANumber && !TempoRedDaysNormalCostUpdated)
         {
            std::cout << "BBRHPJR" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CEnergy>("TempoRedDaysNormalCostPeriod", lvalue);
            TempoRedDaysNormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_PTEC:
         if (!TimePeriodUpdated && m_deviceCreated)
         {
            std::cout << "PTEC" << "=" << value << std::endl;

            m_TimePeriod.reset(new CRunningPeriod(m_api, m_DeviceName, "RunningPeriod", m_KeywordDetails));
            std::string temp(value);
            m_TimePeriod->SetValue(temp);
            m_KeywordList.push_back(m_TimePeriod->GetHistorizable());
            TimePeriodUpdated = true;
         }
         break;
      case TELEINFO_TYPE_IINST:
         if (lvalueIsANumber && !InstantCurrentUpdated)
         {
            std::cout << "IINST" << "=" << value << std::endl;
            HistorizeTeleInfoData<yApi::historization::CCurrent>("InstantCurrent", lvalue);
            InstantCurrentUpdated = true;
         }
         break;
      case TELEINFO_TYPE_IMAX: //We do not use this one also
         break;
      case TELEINFO_TYPE_PAPP:
         if (lvalueIsANumber && !ApparentPowerUpdated)
         {
            std::cout << "PAPP" << "=" << lvalue << std::endl;
            HistorizeTeleInfoData<yApi::historization::CApparentPower>("ApparentPower", lvalue);
            ApparentPowerUpdated = true;
         }
         break;

      case TELEINFO_TYPE_HHPHC: //No interest ! Used by the distributor.
         std::cout << "HHPHC" << "=" << value << std::endl;
         break;

      case TELEINFO_TYPE_DEMAIN:
         if (!ForecastPeriodUpdated && m_deviceCreated)
         {
            std::cout << "DEMAIN" << "=" << value << std::endl;

            m_ForecastPeriod.reset(new CForecastTomorrow(m_api, m_DeviceName, "ForecastColor", m_KeywordDetails));
            std::string temp(value);
            m_ForecastPeriod->SetValue(temp);
            m_KeywordList.push_back(m_ForecastPeriod->GetHistorizable());
            ForecastPeriodUpdated = true;
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
      return;
   }
}

