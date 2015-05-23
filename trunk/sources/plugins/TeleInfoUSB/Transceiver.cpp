#include "stdafx.h"
#include "Transceiver.h"
#include "TransceiverStatus.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "TeleInfoFactory.h"
#include "IncrementSequenceNumber.h"
#include <shared/communication/PortException.hpp>
#include "Keywords/PowerMeter.h"
#include "Keywords/CurrentMeter.h"
#include "Keywords/ApparentPowerMeter.h"
#include "Keywords/CurrentMeter.h"
#include "TeleInfotrxHelpers.h"

//TODO : m_buffer ‡ virer
//TODO : Si Ok, alors RBUF ‡ supprimer !

#define NB_MESSAGES_RECEIVED 20 // Environ toutes les 30s

CTransceiver::CTransceiver()
   :m_seqNumberProvider(new CIncrementSequenceNumber())
{
}

CTransceiver::~CTransceiver()
{
}

void CTransceiver::decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> context, 
	                                     std::string & PluginName,
                                         const shared::communication::CByteBuffer& data) const
{
   const unsigned char *buf = reinterpret_cast<const unsigned char *>(data.begin());

	std::string value;
	std::string checksum;

	ParseData(context, PluginName, buf, data.size());
}

/* Explanation of the checksum computation issued from the official EDF specification

a "checksum" is calculated on the set of characters from the beginning of the label field to the end of the field given character SP included.
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

bool CTransceiver::isCheckSumOk(const unsigned char *m_buffer) const
{
	unsigned int checksum = 0x00;
	int i;

	for (i = 0; i < int(strlen((char*)m_buffer)) - 2; i++)
	{
		checksum += m_buffer[i];
	}
	checksum = (checksum & 0x3F) + 0x20;
	return (checksum == m_buffer[strlen((char*)m_buffer) - 1]);
}

void CTransceiver::ParseData(boost::shared_ptr<yApi::IYPluginApi> context, std::string & PluginName, const unsigned char *pData, int Len) const
{
	int ii = 0;
   	unsigned char m_buffer[readBufferSize];
	int m_bufferpos=0;

	while (ii<Len)
	{
		const unsigned char c = pData[ii];

		if ((c == 0x0d) || (c == 0x00))
		{
			ii++;
			continue;
		}

		m_buffer[m_bufferpos] = c;
		if (c == 0x0a || m_bufferpos == sizeof(m_buffer) - 1)
		{
			// discard newline, close string, parse line and clear it.
			if (m_bufferpos > 0)
				m_buffer[m_bufferpos] = 0;

			//We check the line only if the checksum is ok
			if (isCheckSumOk( m_buffer ))
				MatchLine( m_buffer, context, PluginName );

			m_bufferpos = 0;
		}
		else
		{
			m_bufferpos++;
		}
		ii++;
	}
}

void CTransceiver::MatchLine( const unsigned char *m_buffer, boost::shared_ptr<yApi::IYPluginApi> context, std::string & PluginName ) const
{
	//TODO : Un peu redondant avec la partie suivante
	static const EnumValuesTypes EEnumValuesTypes = boost::assign::map_list_of
		( TE_ADCO   , TELEINFO_TYPE_ADCO )
		( TE_OPTARIF, TELEINFO_TYPE_OPTARIF )
		( TE_ISOUSC , TELEINFO_TYPE_ISOUSC )
		( TE_BASE   , TELEINFO_TYPE_BASE )
		( TE_HCHC   , TELEINFO_TYPE_HCHC )
		( TE_HCHP   , TELEINFO_TYPE_HCHP )
		( TE_EJPHPM , TELEINFO_TYPE_EJPHPM )
		( TE_EJPHN  , TELEINFO_TYPE_EJPHN )
		( TE_BBRHCJB, TELEINFO_TYPE_BBRHCJB )
		( TE_BBRHPJB, TELEINFO_TYPE_BBRHPJB )
		( TE_BBRHCJW, TELEINFO_TYPE_BBRHCJW )
		( TE_BBRHPJW, TELEINFO_TYPE_BBRHPJW )
		( TE_BBRHCJR, TELEINFO_TYPE_BBRHCJR )
		( TE_BBRHPJR, TELEINFO_TYPE_BBRHPJR )
		( TE_PTEC   , TELEINFO_TYPE_PTEC )
		( TE_IINST  , TELEINFO_TYPE_IINST )
		( TE_IMAX   , TELEINFO_TYPE_IMAX )
		( TE_PAPP   , TELEINFO_TYPE_PAPP );

	if ((strlen((const char*)m_buffer)<1) || (m_buffer[0] == 0x0a))
		return;
	
	Match t[18] = {
	{ 12 }, //TELEINFO_TYPE_ADCO
	{  4 }, //TELEINFO_TYPE_OPTARIF
	{  2 }, //TELEINFO_TYPE_ISOUSC
	{  9 }, //TELEINFO_TYPE_BASE
	{  9 }, //TELEINFO_TYPE_HCHC
	{  9 }, //TELEINFO_TYPE_HCHP
	{  9 }, //TELEINFO_TYPE_EJPHPM
	{  9 }, //TELEINFO_TYPE_EJPHN
	{  9 }, //TELEINFO_TYPE_BBRHCJB
	{  9 }, //TELEINFO_TYPE_BBRHPJB
	{  9 }, //TELEINFO_TYPE_BBRHCJW
	{  9 }, //TELEINFO_TYPE_BBRHPJW
	{  9 }, //TELEINFO_TYPE_BBRHCJR
	{  9 }, //TELEINFO_TYPE_BBRHPJR
	{  4 }, //TELEINFO_TYPE_PTEC
	{  3 }, //TELEINFO_TYPE_IINST
	{  3 }, //TELEINFO_TYPE_IMAX
	{  5 }  //TELEINFO_TYPE_PAPP
    };

	char value[20] = "";
	char id[15] = "";

	//We get the id --> This function could be adjust for the following
	unsigned char * pos = (unsigned char *)strchr((char*)m_buffer, ' ');
	if (pos == NULL)
		return;
	int position = int(pos - m_buffer);
	strncpy(id, (char*)m_buffer, position);
	id[position] = 0;

    EnumValuesTypes::const_iterator it = EEnumValuesTypes.find( id );
	if (it != EEnumValuesTypes.end())
	{
		//We get the width car after the space
		pos = (unsigned char *)strchr((char*)m_buffer, ' ');
		if (pos == NULL)
			return;
		position = int(pos - (unsigned char*)m_buffer);
		strncpy(value, (const char*)(&m_buffer[position + 1]), t[it->second].width);
		value[t[it->second].width] = 0;

		unsigned long ulvalue = (unsigned long)atoi(value);

		std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

		boost::shared_ptr<CPowerMeter> Base;
		boost::shared_ptr<CPowerMeter> LowCost;
		boost::shared_ptr<CPowerMeter> NormalCost;

		boost::shared_ptr<CPowerMeter> EJPPeakPeriod;
		boost::shared_ptr<CPowerMeter> EJPNormalPeriod;

		boost::shared_ptr<CPowerMeter> TempoBlueDaysLowCostPeriod;
		boost::shared_ptr<CPowerMeter> TempoBlueDaysNormalCostPeriod;

		boost::shared_ptr<CPowerMeter> TempoWhiteDaysLowCostPeriod;
		boost::shared_ptr<CPowerMeter> TempoWhiteDaysNormalCostPeriod;

		boost::shared_ptr<CPowerMeter> TempoRedDaysLowCostPeriod;
		boost::shared_ptr<CPowerMeter> TempoRedDaysNormalCostPeriod;

		boost::shared_ptr<CCurrentMeter> InstantCurrent;
		boost::shared_ptr<CCurrentMeter> MaxCurrent;
		boost::shared_ptr<CApparentPowerMeter> ApparentPower;

		static unsigned char baseCounter           = 0;
		static unsigned char LowCostCounter        = 0;
		static unsigned char NormalCostCounter     = 0;
		static unsigned char InstantCurrentCounter = 0;
		static unsigned char MaxCurrentCounter     = 0;
		static unsigned char ApparentPowerCounter  = 0;

		static unsigned char EJPPeakPeriodCounter  = 0;
		static unsigned char EJPNormalPeriodCounter= 0;

		static unsigned char TempoBlueDaysLowCostCounter  = 0;
		static unsigned char TempoBlueDaysNormalCostCounter  = 0;

		static unsigned char TempoWhiteDaysLowCostCounter  = 0;
		static unsigned char TempoWhiteDaysNormalCostCounter  = 0;

		static unsigned char TempoRedDaysLowCostCounter  = 0;
		static unsigned char TempoRedDaysNormalCostCounter  = 0;

		switch (it->second)
		{
		case TELEINFO_TYPE_ADCO:
			YADOMS_LOG(debug) << "ADCO" << "=" << value;
			//TODO : Celui-ci n'est qu'un identifiant, il peut Ítre mise qu'une seule fois, ‡ la mise en route / Premier dÈmarrage de Yadoms
			break;
		case TELEINFO_TYPE_OPTARIF:
			YADOMS_LOG(debug) << "OPTARIF" << "=" << value;
			//TODO : Celui-ci n'est qu'un identifiant, il peut Ítre mise qu'une seule fois, ‡ la mise en route / Premier dÈmarrage de Yadoms
			break;
		case TELEINFO_TYPE_ISOUSC:
			YADOMS_LOG(debug) << "ISOUSC" << "=" << value;
			//TODO : Celui-ci n'est qu'un identifiant, il peut Ítre mise qu'une seule fois, ‡ la mise en route / Premier dÈmarrage de Yadoms
			break;
		case TELEINFO_TYPE_BASE:
			if ( ulvalue != 0 && baseCounter >= NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BASE" << "=" << ulvalue;
				Base.reset( new CPowerMeter( context, PluginName, "BaseCounter" ));
				Base->SetValue ( ulvalue );
				KeywordList.push_back ( Base->GetHistorizable() );
				baseCounter = 0;
			}
			++baseCounter;
			break;
		case TELEINFO_TYPE_HCHC:
			if ( ulvalue != 0 && LowCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "HCHC" << "=" << ulvalue;
				LowCost.reset( new CPowerMeter( context, PluginName, "LowCostCounter" ));
				LowCost->SetValue ( ulvalue );
				KeywordList.push_back ( LowCost->GetHistorizable() );
				LowCostCounter = 0;
			}
			++LowCostCounter;
			break;
		case TELEINFO_TYPE_HCHP:
			if ( ulvalue != 0 && NormalCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "HCHP" << "=" << ulvalue;
				NormalCost.reset( new CPowerMeter( context, PluginName, "NormalCostCounter" ));
				NormalCost->SetValue ( ulvalue );
				KeywordList.push_back ( NormalCost->GetHistorizable() );
				NormalCostCounter = 0;
			}
			++NormalCostCounter;
			break;
		case TELEINFO_TYPE_EJPHPM:
			if ( ulvalue != 0 && EJPPeakPeriodCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "EJPHPM" << "=" << ulvalue;
				EJPPeakPeriod.reset( new CPowerMeter( context, PluginName, "EJPPeakPeriod" ));
				EJPPeakPeriod->SetValue ( ulvalue );
				KeywordList.push_back ( EJPPeakPeriod->GetHistorizable() );
				EJPPeakPeriodCounter = 0;
			}
			++EJPPeakPeriodCounter;
			break;
		case TELEINFO_TYPE_EJPHN:
			if ( ulvalue != 0 && EJPNormalPeriodCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "EJPHPN" << "=" << ulvalue;
				EJPNormalPeriod.reset( new CPowerMeter( context, PluginName, "EJPNormalPeriod" ));
				EJPNormalPeriod->SetValue ( ulvalue );
				KeywordList.push_back ( EJPNormalPeriod->GetHistorizable() );
				EJPNormalPeriodCounter = 0;
			}
			++EJPNormalPeriodCounter;
			break;
		case TELEINFO_TYPE_BBRHCJB:
			if ( ulvalue != 0 && TempoBlueDaysLowCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHCJB" << "=" << ulvalue;
				TempoBlueDaysLowCostPeriod.reset( new CPowerMeter( context, PluginName, "TempoBlueDaysLowCostPeriod" ));
				TempoBlueDaysLowCostPeriod->SetValue ( ulvalue );
				KeywordList.push_back ( TempoBlueDaysLowCostPeriod->GetHistorizable() );
				TempoBlueDaysLowCostCounter = 0;
			}
			++TempoBlueDaysLowCostCounter;
			break;
		case TELEINFO_TYPE_BBRHPJB:
			if ( ulvalue != 0 && TempoBlueDaysNormalCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHPJB" << "=" << ulvalue;
				TempoBlueDaysNormalCostPeriod.reset( new CPowerMeter( context, PluginName, "TempoBlueDaysNormalCostPeriod" ));
				TempoBlueDaysNormalCostPeriod->SetValue ( ulvalue );
				KeywordList.push_back ( TempoBlueDaysNormalCostPeriod->GetHistorizable() );
				TempoBlueDaysNormalCostCounter = 0;
			}
			++TempoBlueDaysNormalCostCounter;
			break;
		case TELEINFO_TYPE_BBRHCJW:
			if ( ulvalue != 0 && TempoWhiteDaysLowCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHCJW" << "=" << ulvalue;
				TempoWhiteDaysLowCostPeriod.reset( new CPowerMeter( context, PluginName, "TempoWhiteDaysLowCostPeriod" ));
				TempoWhiteDaysLowCostPeriod->SetValue ( ulvalue );
				KeywordList.push_back ( TempoWhiteDaysLowCostPeriod->GetHistorizable() );
				TempoWhiteDaysLowCostCounter = 0;
			}
			++TempoWhiteDaysLowCostCounter;
			break;
		case TELEINFO_TYPE_BBRHPJW:
			if ( ulvalue != 0 && TempoWhiteDaysNormalCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHPJW" << "=" << ulvalue;
				TempoWhiteDaysNormalCostPeriod.reset( new CPowerMeter( context, PluginName, "TempoWhiteDaysNormalCostPeriod" ));
				TempoWhiteDaysNormalCostPeriod->SetValue ( ulvalue );
				KeywordList.push_back ( TempoWhiteDaysNormalCostPeriod->GetHistorizable() );
				TempoWhiteDaysNormalCostCounter = 0;
			}
			++TempoWhiteDaysNormalCostCounter;
			break;
		case TELEINFO_TYPE_BBRHCJR:
			if ( ulvalue != 0 && TempoRedDaysLowCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHCJR" << "=" << ulvalue;
				TempoRedDaysLowCostPeriod.reset( new CPowerMeter( context, PluginName, "TempoRedDaysLowCostPeriod" ));
				TempoRedDaysLowCostPeriod->SetValue ( ulvalue );
				KeywordList.push_back ( TempoRedDaysLowCostPeriod->GetHistorizable() );
				TempoRedDaysLowCostCounter = 0;
			}
			++TempoRedDaysLowCostCounter;
			break;
		case TELEINFO_TYPE_BBRHPJR:
			if ( ulvalue != 0 && TempoRedDaysNormalCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHPJR" << "=" << ulvalue;
				TempoRedDaysNormalCostPeriod.reset( new CPowerMeter( context, PluginName, "TempoRedDaysNormalCostPeriod" ));
				TempoRedDaysNormalCostPeriod->SetValue ( ulvalue );
				KeywordList.push_back ( TempoRedDaysNormalCostPeriod->GetHistorizable() );
				TempoRedDaysNormalCostCounter = 0;
			}
			++TempoRedDaysNormalCostCounter;
			break;
		case TELEINFO_TYPE_PTEC:
			YADOMS_LOG(debug) << "PTEC" << "=" << value;
			//TODO : A faire ! PÈriode tarifaire en cours !
			break;
		case TELEINFO_TYPE_IINST:
			if ( ulvalue != 0 && InstantCurrentCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "IINST" << "=" << value;
				InstantCurrent.reset( new CCurrentMeter( context, PluginName, "InstantCurrent" ));
				InstantCurrent->SetValue ( ulvalue );
				KeywordList.push_back ( InstantCurrent->GetHistorizable() );
				InstantCurrentCounter = 0;
			}
			++InstantCurrentCounter;
			break;
		case TELEINFO_TYPE_IMAX:
			if (ulvalue != 0 && MaxCurrentCounter>= NB_MESSAGES_RECEIVED)
			{
				YADOMS_LOG(debug) << "IMAX" << "=" << value;
				MaxCurrent.reset( new CCurrentMeter( context, PluginName, "MaxCurrent" ));
				MaxCurrent->SetValue ( ulvalue );
				KeywordList.push_back ( MaxCurrent->GetHistorizable() );
				MaxCurrentCounter = 0;
			}
			++MaxCurrentCounter;
			break;
		case TELEINFO_TYPE_PAPP:
			if ( ulvalue != 0 && ApparentPowerCounter >= NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "PAPP" << "=" << ulvalue;
				ApparentPower.reset( new CApparentPowerMeter( context, PluginName, "ApparentPower" ));
				ApparentPower->SetValue ( ulvalue );
				KeywordList.push_back ( ApparentPower->GetHistorizable() );
				ApparentPowerCounter = 0;
			}
			++ApparentPowerCounter;
			break;

		case TELEINFO_TYPE_HHPHC: //TODO
			YADOMS_LOG(debug) << "HHPHC" << "=" << value;
			break;
		case TELEINFO_TYPE_DEMAIN: //TODO
			YADOMS_LOG(debug) << "DEMAIN" << "=" << value;
			break;
		case TELEINFO_TYPE_ADPS: //TODO
			YADOMS_LOG(debug) << "ADPS" << "=" << value;
			break;
		case TELEINFO_TYPE_MOTDETAT: //TODO
			YADOMS_LOG(debug) << "MOTDETAT" << "=" << value;
			break;
		default:
			break;
		}

		//historizing all information push in the list
		context->historizeData(PluginName, KeywordList);
		return;
	}
}