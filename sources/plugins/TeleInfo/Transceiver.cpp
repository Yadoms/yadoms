#include "stdafx.h"
#include "Transceiver.h"
#include <shared/DataContainer.h>
#include "IncrementSequenceNumber.h"
#include "TeleInfotrxHelpers.h"

CTransceiver::CTransceiver(boost::shared_ptr<yApi::IYPluginApi> api)
   : m_baseCounter   (boost::make_shared<yApi::historization::CEnergy>("BaseCounter")),
     m_lowCostCounter(boost::make_shared<yApi::historization::CEnergy>("LowCostCounter")),
     m_normalCostCounter(boost::make_shared<yApi::historization::CEnergy>("NormalCostCounter")),
     m_EJPPeakPeriod(boost::make_shared<yApi::historization::CEnergy>("EJPPeakPeriod")),
     m_EJPNormalPeriod(boost::make_shared<yApi::historization::CEnergy>("EJPNormalPeriod")),
     m_tempoBlueDaysLowCostPeriod    (boost::make_shared<yApi::historization::CEnergy>("TempoBlueDaysLowCostPeriod")),
     m_tempoBlueDaysNormalCostPeriod (boost::make_shared<yApi::historization::CEnergy>("TempoBlueDaysNormalCostPeriod")),
     m_tempoRedDaysLowCostPeriod     (boost::make_shared<yApi::historization::CEnergy>("TempoRedDaysLowCostPeriod")),
     m_tempoRedDaysNormalCostPeriod  (boost::make_shared<yApi::historization::CEnergy>("TempoRedDaysNormalCostPeriod")),
     m_tempoWhiteDaysLowCostPeriod   (boost::make_shared<yApi::historization::CEnergy>("TempoWhiteDaysLowCostPeriod")),
     m_tempoWhiteDaysNormalCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoWhiteDaysNormalCostPeriod")),
     m_instantCurrent(boost::make_shared<yApi::historization::CCurrent>("InstantCurrent")),
     m_apparentPower(boost::make_shared<yApi::historization::CApparentPower>("ApparentPower")),
     m_TimePeriod(boost::make_shared<CRunningPeriod>(api, "RunningPeriod")),
     m_ForecastPeriod(boost::make_shared<CForecastTomorrow>(api, "ForecastColor")),
     m_api(api),
     m_isdeveloperMode(false),
     m_deviceCreated(false),
     m_optarif(OP_NOT_DEFINED)
{
   m_isdeveloperMode = api->isDeveloperMode();
   CTransceiver::ResetRefreshTags();
}

CTransceiver::~CTransceiver()
{
}

void CTransceiver::decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                         const shared::communication::CByteBuffer& data)
{
   auto buf = reinterpret_cast<const unsigned char *>(data.begin());
   ParseData(buf, data.size());
}

/* Explanation of the checksum computation issued from the official EDF specification

A "checksum" is calculated on the set of characters from the beginning of the label field to the end of the field given character SP included.
We first make ??the sum of all ASCII codes of all characters.
to avoid introduce ASCII (00 to 1F hex) functions, it retains only the six least significant bits of
result (this translates into a logical AND between the amount previously calculated and 03Fh).
Finally, we added 20 hexadecimal. The result will always be a printable ASCII character (sign, digit,
capital letter) of from 0x20 to hexadecimal 0x5F

La "checksum" est calculé sur l'ensemble des caractères allant du début du champ étiquette à la fin du champ
donnée, caractère SP inclus. On fait tout d'abord la somme des codes ASCII de tous ces caractères. Pour éviter
d'introduire des fonctions ASCII (00 à 1F en hexadécimal), on ne conserve que les six bits de poids faible du
résultat obtenu (cette opération se traduit par un ET logique entre la somme précédemment calculée et 03Fh).
Enfin, on ajoute 20 en hexadécimal. Le résultat sera donc toujours un caractère ASCII imprimable (signe, chiffre,
lettre majuscule) allant de 20 à 5F en hexadécimal.
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

   // creation of the device
   if (!m_deviceCreated) createDeviceAndKeywords();

   // historizing all information pushed in the list
   if (m_deviceCreated && isInformationUpdated()) m_api->historizeData(m_DeviceName, m_keywords);
}

void CTransceiver::createDeviceAndKeywords( void )
{
   //Add the apparent power if this frame is detected. This frame is not automatically generated by all counters
   if (m_apparentPowerUpdated)
      m_keywords.push_back(m_apparentPower);

   if (m_isdeveloperMode) std::cout << "Nb keywords : " << "=" << m_keywords.size() << std::endl;

   if (!isERDFCounterDesactivated())
   {
      m_api->declareDevice(m_DeviceName,
                           "TeleInfoUSB : Id = " + m_DeviceName,
                           m_keywords,
                           m_DeviceDetails);
   }
   
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

bool CTransceiver::isInformationUpdated() const
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

bool CTransceiver::isERDFCounterDesactivated() const
{
   // if only the ADCO is update, the teleInformation is not activated into the ERDF counter
   if (m_adcoUpdated && !m_baseUpdated && m_optarif== OP_NOT_DEFINED && !m_lowCostUpdated && !m_normalCostUpdated && !m_eJPPeakPeriodUpdated && !m_eJPNormalPeriodUpdated
       && !m_tempoBlueDaysLowCostUpdated && !m_tempoBlueDaysNormalCostUpdated && !m_tempoWhiteDaysLowCostUpdated && !m_tempoWhiteDaysNormalCostUpdated && !m_tempoRedDaysLowCostUpdated && !m_tempoRedDaysNormalCostUpdated && !m_forecastPeriodUpdated)
      return true;
   else
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

      static auto OpTarifRead = false;
      static auto ISousRead = false;

      switch (it->second)
      {
      case TELEINFO_TYPE_ADCO:   //TODO_V2 : Detect the change of the counter in live
         if (!m_adcoUpdated)
         {
            if (m_isdeveloperMode) std::cout << "ADCO" << "=" << value << std::endl;

            m_DeviceName = value;
            m_adcoUpdated = true;
         }
         break;
      case TELEINFO_TYPE_OPTARIF:
         if (!OpTarifRead)
         {
            if (m_isdeveloperMode) std::cout << "OPTARIF" << "=" << value << std::endl;

            OpTarifRead = true;

            switch (value[1])
            {
               case 'A':
               {
                  m_optarif = OP_BASE;

                  m_keywords.clear();
                  m_keywords.push_back(m_baseCounter);
                  m_keywords.push_back(m_instantCurrent);
                  m_keywords.push_back(m_TimePeriod->GetHistorizable());
                  break;
               }
               case 'C':
               {
                  m_optarif = OP_CREUSE;

                  m_keywords.clear();
                  m_keywords.push_back(m_lowCostCounter);
                  m_keywords.push_back(m_normalCostCounter);
                  m_keywords.push_back(m_instantCurrent);
                  m_keywords.push_back(m_TimePeriod->GetHistorizable());
                  break;
               }
               case 'J':
               {
                  m_optarif = OP_EJP;

                  m_keywords.clear();
                  m_keywords.push_back(m_EJPPeakPeriod);
                  m_keywords.push_back(m_EJPNormalPeriod);
                  m_keywords.push_back(m_instantCurrent);
                  m_keywords.push_back(m_TimePeriod->GetHistorizable());
                  break;
               }
               case 'B':
               {
                  m_optarif = OP_TEMPO;

                  m_keywords.clear();
                  m_keywords.push_back(m_tempoBlueDaysLowCostPeriod);
                  m_keywords.push_back(m_tempoBlueDaysNormalCostPeriod);
                  m_keywords.push_back(m_tempoRedDaysLowCostPeriod);
                  m_keywords.push_back(m_tempoRedDaysNormalCostPeriod);
                  m_keywords.push_back(m_tempoWhiteDaysLowCostPeriod);
                  m_keywords.push_back(m_tempoWhiteDaysNormalCostPeriod);
                  m_keywords.push_back(m_instantCurrent);
                  m_keywords.push_back(m_TimePeriod->GetHistorizable());
                  m_keywords.push_back(m_ForecastPeriod->GetHistorizable());
                  break;
               }
               default:
                  //Erreur normalement
                  break;
            }
         }
         break;
      case TELEINFO_TYPE_ISOUSC:
         if (!ISousRead)
         {
            if (m_isdeveloperMode) std::cout << "ISOUSC" << "=" << value << std::endl;

            ISousRead = true;
         }
         break;
      case TELEINFO_TYPE_BASE:
         if (lvalueIsANumber && !m_baseUpdated)
         {
            if (m_isdeveloperMode) std::cout << "BASE" << "=" << lvalue << std::endl;

            m_baseCounter->set(lvalue);
            m_baseUpdated = true;
         }
         break;
      case TELEINFO_TYPE_HCHC:
         if (lvalueIsANumber && !m_lowCostUpdated)
         {
            if (m_isdeveloperMode) std::cout << "HCHC" << "=" << lvalue << std::endl;

            m_lowCostCounter->set(lvalue);
            m_lowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_HCHP:
         if (lvalueIsANumber && !m_normalCostUpdated)
         {
            if (m_isdeveloperMode) std::cout << "HCHP" << "=" << lvalue << std::endl;

            m_normalCostCounter->set(lvalue);
            m_normalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_EJPHPM:
         if (lvalueIsANumber && !m_eJPPeakPeriodUpdated)
         {
            if (m_isdeveloperMode) std::cout << "EJPHPM" << "=" << lvalue << std::endl;

            m_EJPPeakPeriod->set(lvalue);
            m_eJPPeakPeriodUpdated = true;
         }
         break;
      case TELEINFO_TYPE_EJPHN:
         if (lvalueIsANumber && !m_eJPNormalPeriodUpdated)
         {
            if (m_isdeveloperMode) std::cout << "EJPHPN" << "=" << lvalue << std::endl;

            m_EJPNormalPeriod->set(lvalue);
            m_eJPNormalPeriodUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHCJB:
         if (lvalueIsANumber && !m_tempoBlueDaysLowCostUpdated)
         {
            if (m_isdeveloperMode) std::cout << "BBRHCJB" << "=" << lvalue << std::endl;

            m_tempoBlueDaysLowCostPeriod->set(lvalue);
            m_tempoBlueDaysLowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHPJB:
         if (lvalueIsANumber && !m_tempoBlueDaysNormalCostUpdated)
         {
            if (m_isdeveloperMode) std::cout << "BBRHPJB" << "=" << lvalue << std::endl;

            m_tempoBlueDaysNormalCostPeriod->set(lvalue);
            m_tempoBlueDaysNormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHCJW:
         if (lvalueIsANumber && !m_tempoWhiteDaysLowCostUpdated)
         {
            if (m_isdeveloperMode) std::cout << "BBRHCJW" << "=" << lvalue << std::endl;

            m_tempoWhiteDaysLowCostPeriod->set(lvalue);
            m_tempoWhiteDaysLowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHPJW:
         if (lvalueIsANumber && !m_tempoWhiteDaysNormalCostUpdated)
         {
            if (m_isdeveloperMode) std::cout << "BBRHPJW" << "=" << lvalue << std::endl;

            m_tempoWhiteDaysNormalCostPeriod->set(lvalue);
            m_tempoWhiteDaysNormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHCJR:
         if (lvalueIsANumber && !m_tempoRedDaysLowCostUpdated)
         {
            if (m_isdeveloperMode) std::cout << "BBRHCJR" << "=" << lvalue << std::endl;

            m_tempoRedDaysLowCostPeriod->set(lvalue);
            m_tempoRedDaysLowCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_BBRHPJR:
         if (lvalueIsANumber && !m_tempoRedDaysNormalCostUpdated)
         {
            if (m_isdeveloperMode) std::cout << "BBRHPJR" << "=" << lvalue << std::endl;

            m_tempoRedDaysNormalCostPeriod->set(lvalue);
            m_tempoRedDaysNormalCostUpdated = true;
         }
         break;
      case TELEINFO_TYPE_PTEC:
         if (!m_timePeriodUpdated && m_deviceCreated)
         {
            if (m_isdeveloperMode) std::cout << "PTEC" << "=" << value << std::endl;

            std::string temp(value);
            m_TimePeriod->set(temp);
            m_timePeriodUpdated = true;
         }
         break;
      case TELEINFO_TYPE_IINST:
         if (lvalueIsANumber && !m_instantCurrentUpdated)
         {
            if (m_isdeveloperMode) std::cout << "IINST" << "=" << value << std::endl;

            m_instantCurrent->set(lvalue);
            m_instantCurrentUpdated = true;
         }
         break;
      case TELEINFO_TYPE_IMAX: //We do not use this one also
         break;
      case TELEINFO_TYPE_PAPP:
         if (lvalueIsANumber && !m_apparentPowerUpdated)
         {
            if (m_isdeveloperMode) std::cout << "PAPP" << "=" << lvalue << std::endl;

            m_apparentPower->set(lvalue);
            m_apparentPowerUpdated = true;
         }
         break;

      case TELEINFO_TYPE_HHPHC: //No interest ! Used by the distributor.
         break;

      case TELEINFO_TYPE_DEMAIN:
         if (!m_forecastPeriodUpdated && m_deviceCreated)
         {
            if (m_isdeveloperMode) std::cout << "DEMAIN" << "=" << value << std::endl;

            std::string temp(value);
            m_ForecastPeriod->set(temp);
            m_forecastPeriodUpdated = true;
         }
         break;

      case TELEINFO_TYPE_ADPS: // Threshold warning ! If IINST > ISOUSC
         if (m_isdeveloperMode) std::cout << "ADPS" << "=" << value << std::endl;
         break;

      case TELEINFO_TYPE_MOTDETAT:
         // This value is for the distributor. It's nevers used
         if (m_isdeveloperMode) std::cout << "MOTDETAT" << "=" << value << std::endl;
         break;
      default:
         break;
      }
   }
}