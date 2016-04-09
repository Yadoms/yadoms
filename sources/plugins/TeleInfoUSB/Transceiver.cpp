#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "TeleInfoFactory.h"
#include "IncrementSequenceNumber.h"
#include <shared/communication/PortException.hpp>
#include "TeleInfotrxHelpers.h"

#define NB_MESSAGES_RECEIVED 20 // Nearly 30 seconds

CTransceiver::CTransceiver()
   :m_seqNumberProvider(new CIncrementSequenceNumber())
{
}

CTransceiver::~CTransceiver()
{
}

void CTransceiver::decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> context,
	                                     std::string & PluginName,
                                         const shared::communication::CByteBuffer& data)
{
   const unsigned char *buf = reinterpret_cast<const unsigned char *>(data.begin());

   m_context = context;
   m_PluginName = PluginName;

	ParseData( buf, data.size() );
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

void CTransceiver::ParseData( const unsigned char *pData, int Len )
{
	int ii = 0;
   	unsigned char m_buffer[readBufferSize];
	int m_bufferpos=0;

	while (ii<Len)
	{
		const unsigned char c = pData[ii];

		if ((c == 0x0a) || (c == 0x00))
		{
			ii++;
			m_bufferpos = 0;
			continue;
		}

		m_buffer[m_bufferpos] = c;
		if (c == 0x0d || m_bufferpos == sizeof(m_buffer) - 1)
		{
			// discard newline, close string, parse line and clear it.
			if (m_bufferpos > 0)
				m_buffer[m_bufferpos] = 0;

			//We check the line only if the checksum is ok
			if (isCheckSumOk( m_buffer ))
				MatchLine( m_buffer );

			m_bufferpos = 0;
		}
		else
		{
			m_bufferpos++;
		}
		ii++;
	}
	//historizing all information push in the list
	m_context->historizeData(m_PluginName, m_KeywordList);

	//Clear the vector
	m_KeywordList.clear();
}

template <class T>
void CTransceiver::HistorizeTeleInfoData ( std::string KeywordName, long Value )
{
	boost::shared_ptr<T> m_energy;

	m_energy.reset ( new T( KeywordName) );

	if (!m_context->keywordExists( m_PluginName, m_energy->getKeyword()))
		m_context->declareKeyword ( m_PluginName, *m_energy );

	m_energy->set( Value );
	YADOMS_LOG(debug) << m_energy->getKeyword() << "=" << m_energy->get();
	m_KeywordList.push_back ( m_energy );
}

void CTransceiver::MatchLine( const unsigned char *m_buffer )
{
	static const EnumValuesTypes EEnumValuesTypes = boost::assign::map_list_of
		( TE_ADCO    , TELEINFO_TYPE_ADCO )
		( TE_OPTARIF , TELEINFO_TYPE_OPTARIF )
		( TE_ISOUSC  , TELEINFO_TYPE_ISOUSC )
		( TE_BASE    , TELEINFO_TYPE_BASE )
		( TE_HCHC    , TELEINFO_TYPE_HCHC )
		( TE_HCHP    , TELEINFO_TYPE_HCHP )
		( TE_EJPHPM  , TELEINFO_TYPE_EJPHPM )
		( TE_EJPHN   , TELEINFO_TYPE_EJPHN )
		( TE_BBRHCJB , TELEINFO_TYPE_BBRHCJB )
		( TE_BBRHPJB , TELEINFO_TYPE_BBRHPJB )
		( TE_BBRHCJW , TELEINFO_TYPE_BBRHCJW )
		( TE_BBRHPJW , TELEINFO_TYPE_BBRHPJW )
		( TE_BBRHCJR , TELEINFO_TYPE_BBRHCJR )
		( TE_BBRHPJR , TELEINFO_TYPE_BBRHPJR )
		( TE_PTEC    , TELEINFO_TYPE_PTEC )
		( TE_IINST   , TELEINFO_TYPE_IINST )
		( TE_IMAX    , TELEINFO_TYPE_IMAX )
		( TE_PAPP    , TELEINFO_TYPE_PAPP )
		( TE_DEMAIN  , TELEINFO_TYPE_DEMAIN) 
		( TE_HHPHC   , TELEINFO_TYPE_HHPHC )
		( TE_ADPS    , TELEINFO_TYPE_ADPS )
		( TE_MOTDETAT, TELEINFO_TYPE_MOTDETAT)
                ;

	if ((strlen((const char*)m_buffer)<1) || (m_buffer[0] == 0x0a))
		return;

	Match t[22] = {
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
	{  5 }, //TELEINFO_TYPE_PAPP
	{  4 }, //TELEINFO_TYPE_DEMAIN 
	{  1 }, //TELEINFO_TYPE_HHPHC
	{  3 }, //TELEINFO_TYPE_ADPS
	{  6 }  //TELEINFO_TYPE_MOTDETAT
    };

	//We get the id --> This function could be adjust for the following
	unsigned char * pos = (unsigned char *)strchr((char*)m_buffer, ' ');
	if (pos == NULL)
		return;
	int position = int(pos - m_buffer);
   char id[15] = "";
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
      char value[20] = "";
		strncpy(value, (const char*)(&m_buffer[position + 1]), t[it->second].width);
		value[t[it->second].width] = 0;             

      long lvalue = 0;
      bool lvalueIsANumber;
      try
		 {
          lvalue = boost::lexical_cast<long>(value);
		    lvalueIsANumber = true;
		 }
		 catch(...)
		 {
		    lvalueIsANumber = false;
		 }

		static unsigned char baseCounter           = NB_MESSAGES_RECEIVED;
		static unsigned char LowCostCounter        = NB_MESSAGES_RECEIVED;
		static unsigned char NormalCostCounter     = NB_MESSAGES_RECEIVED;
		static unsigned char InstantCurrentCounter = NB_MESSAGES_RECEIVED;
		static unsigned char MaxCurrentCounter     = NB_MESSAGES_RECEIVED;
		static unsigned char ApparentPowerCounter  = NB_MESSAGES_RECEIVED;

		static unsigned char EJPPeakPeriodCounter  = NB_MESSAGES_RECEIVED;
		static unsigned char EJPNormalPeriodCounter= NB_MESSAGES_RECEIVED;

		static unsigned char TempoBlueDaysLowCostCounter     = NB_MESSAGES_RECEIVED;
		static unsigned char TempoBlueDaysNormalCostCounter  = NB_MESSAGES_RECEIVED;

		static unsigned char TempoWhiteDaysLowCostCounter    = NB_MESSAGES_RECEIVED;
		static unsigned char TempoWhiteDaysNormalCostCounter = NB_MESSAGES_RECEIVED;

		static unsigned char TempoRedDaysLowCostCounter      = NB_MESSAGES_RECEIVED;
		static unsigned char TempoRedDaysNormalCostCounter   = NB_MESSAGES_RECEIVED;

		static unsigned char TimePeriodCounter  = NB_MESSAGES_RECEIVED;

		static unsigned char ForecastPeriodCounter  = NB_MESSAGES_RECEIVED;

		static bool ADCORead      = false;
		static bool OpTarifRead   = false;
		static bool ISousRead     = false;

		switch (it->second)
		{
		case TELEINFO_TYPE_ADCO:
			if (!ADCORead)
			{
				YADOMS_LOG(information) << "ADCO" << "=" << value;
				ADCORead = true;
			}
			break;
		case TELEINFO_TYPE_OPTARIF:
			if (!OpTarifRead)
			{
				YADOMS_LOG(information) << "OPTARIF" << "=" << value;
				OpTarifRead = true;
			}
			break;
		case TELEINFO_TYPE_ISOUSC:
			if (!ISousRead)
			{
				YADOMS_LOG(information) << "ISOUSC" << "=" << value;
				ISousRead = true;
			}
			break;
		case TELEINFO_TYPE_BASE:
			if ( lvalueIsANumber && baseCounter >= NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BASE" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "BaseCounter", lvalue );
				baseCounter = 0;
			}
			++baseCounter;
			break;
		case TELEINFO_TYPE_HCHC:
			if ( lvalueIsANumber && LowCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "HCHC" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "LowCostCounter", lvalue );
				LowCostCounter = 0;
			}
			++LowCostCounter;
			break;
		case TELEINFO_TYPE_HCHP:
			if ( lvalueIsANumber && NormalCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "HCHP" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "NormalCostCounter", lvalue );
				NormalCostCounter = 0;
			}
			++NormalCostCounter;
			break;
		case TELEINFO_TYPE_EJPHPM:
			if ( lvalueIsANumber && EJPPeakPeriodCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "EJPHPM" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "EJPPeakPeriod", lvalue );
				EJPPeakPeriodCounter = 0;
			}
			++EJPPeakPeriodCounter;
			break;
		case TELEINFO_TYPE_EJPHN:
			if ( lvalueIsANumber && EJPNormalPeriodCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "EJPHPN" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "EJPNormalPeriod", lvalue );
				EJPNormalPeriodCounter = 0;
			}
			++EJPNormalPeriodCounter;
			break;
		case TELEINFO_TYPE_BBRHCJB:
			if ( lvalueIsANumber && TempoBlueDaysLowCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHCJB" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoBlueDaysLowCostPeriod", lvalue );
				TempoBlueDaysLowCostCounter = 0;
			}
			++TempoBlueDaysLowCostCounter;
			break;
		case TELEINFO_TYPE_BBRHPJB:
			if ( lvalueIsANumber && TempoBlueDaysNormalCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHPJB" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoBlueDaysNormalCostPeriod", lvalue );
				TempoBlueDaysNormalCostCounter = 0;
			}
			++TempoBlueDaysNormalCostCounter;
			break;
		case TELEINFO_TYPE_BBRHCJW:
			if ( lvalueIsANumber && TempoWhiteDaysLowCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHCJW" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoWhiteDaysLowCostPeriod", lvalue );
				TempoWhiteDaysLowCostCounter = 0;
			}
			++TempoWhiteDaysLowCostCounter;
			break;
		case TELEINFO_TYPE_BBRHPJW:
			if ( lvalueIsANumber && TempoWhiteDaysNormalCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHPJW" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoWhiteDaysNormalCostPeriod", lvalue );
				TempoWhiteDaysNormalCostCounter = 0;
			}
			++TempoWhiteDaysNormalCostCounter;
			break;
		case TELEINFO_TYPE_BBRHCJR:
			if ( lvalueIsANumber && TempoRedDaysLowCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHCJR" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoRedDaysLowCostPeriod", lvalue );
				TempoRedDaysLowCostCounter = 0;
			}
			++TempoRedDaysLowCostCounter;
			break;
		case TELEINFO_TYPE_BBRHPJR:
			if ( lvalueIsANumber && TempoRedDaysNormalCostCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "BBRHPJR" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoRedDaysNormalCostPeriod", lvalue );
				TempoRedDaysNormalCostCounter = 0;
			}
			++TempoRedDaysNormalCostCounter;
			break;
		case TELEINFO_TYPE_PTEC:
			if ( TimePeriodCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "PTEC" << "=" << value;
				m_TimePeriod.reset( new CRunningPeriod( m_context, m_PluginName, "RunningPeriod" ));
				std::string temp(value);
				m_TimePeriod->SetValue ( temp );
				m_KeywordList.push_back ( m_TimePeriod->GetHistorizable() );
				TimePeriodCounter = 0;
			}
			++TimePeriodCounter;
			break;
		case TELEINFO_TYPE_IINST:
			if ( lvalueIsANumber && InstantCurrentCounter>=NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "IINST" << "=" << value;
				HistorizeTeleInfoData<yApi::historization::CCurrent> ( "InstantCurrent", lvalue );
				InstantCurrentCounter = 0;
			}
			++InstantCurrentCounter;
			break;
		case TELEINFO_TYPE_IMAX:
			if ( lvalueIsANumber && MaxCurrentCounter>= NB_MESSAGES_RECEIVED)
			{
				YADOMS_LOG(debug) << "IMAX" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CCurrent> ( "MaxCurrent", lvalue );
				MaxCurrentCounter = 0;
			}
			++MaxCurrentCounter;
			break;
		case TELEINFO_TYPE_PAPP:
			if ( lvalueIsANumber && ApparentPowerCounter >= NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "PAPP" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CApparentPower> ( "ApparentPower", lvalue );
				ApparentPowerCounter = 0;
			}
			++ApparentPowerCounter;
			break;

		case TELEINFO_TYPE_HHPHC: //No interest ! Used by the distributor.
			YADOMS_LOG(debug) << "HHPHC" << "=" << value;
			break;

		case TELEINFO_TYPE_DEMAIN:
			if ( ForecastPeriodCounter >= NB_MESSAGES_RECEIVED )
			{
				YADOMS_LOG(debug) << "DEMAIN" << "=" << value;
				
				m_ForecastPeriod.reset( new CForecastTomorrow( m_context, m_PluginName, "ForecastColor" ));
				std::string temp(value);
				m_ForecastPeriod->SetValue ( temp );
				m_KeywordList.push_back ( m_ForecastPeriod->GetHistorizable() );
				ForecastPeriodCounter = 0;
			}
			++ForecastPeriodCounter;
			break;

		case TELEINFO_TYPE_ADPS: // Threshold warning ! If IINST > ISOUSC
			YADOMS_LOG(information) << "ADPS" << "=" << value;
			break;

		case TELEINFO_TYPE_MOTDETAT:
			// This value is for the distributor. It's nevers used
			YADOMS_LOG(debug) << "MOTDETAT" << "=" << value;
			break;
		default:
			break;
		}
		return;
	}
}
