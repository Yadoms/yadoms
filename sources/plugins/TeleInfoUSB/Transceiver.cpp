#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "TeleInfoFactory.h"
#include "IncrementSequenceNumber.h"
#include <shared/communication/PortException.hpp>
#include "TeleInfotrxHelpers.h"

CTransceiver::CTransceiver( )
   :m_seqNumberProvider(new CIncrementSequenceNumber()),
    m_deviceCreated ( false ),
    Optarif ( OP_NOT_DEFINED )
{
	ResetRefreshTags ();
}

CTransceiver::~CTransceiver()
{}

void CTransceiver::decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> context,
                                         const shared::communication::CByteBuffer& data)
{
   const unsigned char *buf = reinterpret_cast<const unsigned char *>(data.begin());

   m_context = context;

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
	m_context->historizeData(m_DeviceName, m_KeywordList);

	//Clear the vector
	m_KeywordList.clear();
}

template <class T>
void CTransceiver::HistorizeTeleInfoData ( std::string KeywordName, long Value )
{
	if (m_deviceCreated)
	{
		boost::shared_ptr<T> m_keyword;

		m_keyword.reset ( new T( KeywordName) );

		if (!m_context->keywordExists( m_DeviceName, m_keyword->getKeyword()))
			m_context->declareKeyword ( m_DeviceName, *m_keyword, m_KeywordDetails );

		m_keyword->set( Value );
		YADOMS_LOG(debug) << m_keyword->getKeyword() << "=" << m_keyword->get();
		m_KeywordList.push_back ( m_keyword );
	}
}

void CTransceiver::CreateDevice ( std::string CounterId )
{
	m_DeviceName = CounterId;

	if (!m_context->deviceExists(CounterId))
		m_context->declareDevice(CounterId, "TeleInfoUSB : Id = " + CounterId, m_DeviceDetails);

	//Set the counter id for each keyword
	m_KeywordDetails.set("id", CounterId );

	m_deviceCreated = true;
}

void CTransceiver::ResetRefreshTags ( void )
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

bool CTransceiver::IsInformationUpdated ( void )
{
	// We do not take account about the apparent power. Some counters may not have this tag
	if ((
		( (Optarif == OP_BASE)   && ( baseUpdated ) ) ||
		( (Optarif == OP_CREUSE) && ( LowCostUpdated ) && ( NormalCostUpdated ) ) ||
		( (Optarif == OP_EJP)    && ( EJPPeakPeriodUpdated ) && ( EJPNormalPeriodUpdated ) ) ||
		( (Optarif == OP_TEMPO)  && ( TempoBlueDaysLowCostUpdated ) && ( TempoBlueDaysNormalCostUpdated ) && ( TempoWhiteDaysLowCostUpdated ) && ( TempoWhiteDaysNormalCostUpdated ) && ( TempoRedDaysLowCostUpdated ) && ( TempoRedDaysNormalCostUpdated ) && ( ForecastPeriodUpdated ) )
	   )
		&& ( TimePeriodUpdated )
		&& ( InstantCurrentUpdated )
	   )
	   return true;
	else
		return false;
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

		static bool ADCORead      = false;
		static bool OpTarifRead   = false;
		static bool ISousRead     = false;

		switch (it->second)
		{
		case TELEINFO_TYPE_ADCO:
			if (!ADCORead)
			{
				YADOMS_LOG(information) << "ADCO" << "=" << value;
				
				CreateDevice ( value );

				ADCORead = true;
			}
			break;
		case TELEINFO_TYPE_OPTARIF:
			if (!OpTarifRead)
			{
				YADOMS_LOG(information) << "OPTARIF" << "=" << value;
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
				YADOMS_LOG(information) << "ISOUSC" << "=" << value;
				ISousRead = true;
			}
			break;
		case TELEINFO_TYPE_BASE:
			if ( lvalueIsANumber && !baseUpdated )
			{
				YADOMS_LOG(debug) << "BASE" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "BaseCounter", lvalue );
				baseUpdated = true;
			}
			break;
		case TELEINFO_TYPE_HCHC:
			if ( lvalueIsANumber && !LowCostUpdated )
			{
				YADOMS_LOG(debug) << "HCHC" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "LowCostCounter", lvalue );
				LowCostUpdated = true;
			}
			break;
		case TELEINFO_TYPE_HCHP:
			if ( lvalueIsANumber && !NormalCostUpdated )
			{
				YADOMS_LOG(debug) << "HCHP" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "NormalCostCounter", lvalue );
				NormalCostUpdated = true;
			}
			break;
		case TELEINFO_TYPE_EJPHPM:
			if ( lvalueIsANumber && !EJPPeakPeriodUpdated )
			{
				YADOMS_LOG(debug) << "EJPHPM" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "EJPPeakPeriod", lvalue );
				EJPPeakPeriodUpdated = true;
			}
			break;
		case TELEINFO_TYPE_EJPHN:
			if ( lvalueIsANumber && !EJPNormalPeriodUpdated )
			{
				YADOMS_LOG(debug) << "EJPHPN" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "EJPNormalPeriod", lvalue );
				EJPNormalPeriodUpdated = true;
			}
			break;
		case TELEINFO_TYPE_BBRHCJB:
			if ( lvalueIsANumber && !TempoBlueDaysLowCostUpdated )
			{
				YADOMS_LOG(debug) << "BBRHCJB" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoBlueDaysLowCostPeriod", lvalue );
				TempoBlueDaysLowCostUpdated = true;
			}
			break;
		case TELEINFO_TYPE_BBRHPJB:
			if ( lvalueIsANumber && !TempoBlueDaysNormalCostUpdated )
			{
				YADOMS_LOG(debug) << "BBRHPJB" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoBlueDaysNormalCostPeriod", lvalue );
				TempoBlueDaysNormalCostUpdated = true;
			}
			break;
		case TELEINFO_TYPE_BBRHCJW:
			if ( lvalueIsANumber && !TempoWhiteDaysLowCostUpdated )
			{
				YADOMS_LOG(debug) << "BBRHCJW" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoWhiteDaysLowCostPeriod", lvalue );
				TempoWhiteDaysLowCostUpdated = true;
			}
			break;
		case TELEINFO_TYPE_BBRHPJW:
			if ( lvalueIsANumber && !TempoWhiteDaysNormalCostUpdated )
			{
				YADOMS_LOG(debug) << "BBRHPJW" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoWhiteDaysNormalCostPeriod", lvalue );
				TempoWhiteDaysNormalCostUpdated = true;
			}
			break;
		case TELEINFO_TYPE_BBRHCJR:
			if ( lvalueIsANumber && !TempoRedDaysLowCostUpdated )
			{
				YADOMS_LOG(debug) << "BBRHCJR" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoRedDaysLowCostPeriod", lvalue );
				TempoRedDaysLowCostUpdated = true;
			}
			break;
		case TELEINFO_TYPE_BBRHPJR:
			if ( lvalueIsANumber && !TempoRedDaysNormalCostUpdated )
			{
				YADOMS_LOG(debug) << "BBRHPJR" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CEnergy> ( "TempoRedDaysNormalCostPeriod", lvalue );
				TempoRedDaysNormalCostUpdated = true;
			}
			break;
		case TELEINFO_TYPE_PTEC:
			if ( !TimePeriodUpdated && m_deviceCreated)
			{
				YADOMS_LOG(debug) << "PTEC" << "=" << value;

				m_TimePeriod.reset( new CRunningPeriod( m_context, m_DeviceName, "RunningPeriod", m_KeywordDetails ));
				std::string temp(value);
				m_TimePeriod->SetValue ( temp );
				m_KeywordList.push_back ( m_TimePeriod->GetHistorizable() );
				TimePeriodUpdated = true;
			}
			break;
		case TELEINFO_TYPE_IINST:
			if ( lvalueIsANumber && !InstantCurrentUpdated )
			{
				YADOMS_LOG(debug) << "IINST" << "=" << value;
				HistorizeTeleInfoData<yApi::historization::CCurrent> ( "InstantCurrent", lvalue );
				InstantCurrentUpdated = true;
			}
			break;
		case TELEINFO_TYPE_IMAX: //We do not use this one also
			break;
		case TELEINFO_TYPE_PAPP:
			if ( lvalueIsANumber && !ApparentPowerUpdated )
			{
				YADOMS_LOG(debug) << "PAPP" << "=" << lvalue;
				HistorizeTeleInfoData<yApi::historization::CApparentPower> ( "ApparentPower", lvalue );
				ApparentPowerUpdated = true;
			}
			break;

		case TELEINFO_TYPE_HHPHC: //No interest ! Used by the distributor.
			YADOMS_LOG(debug) << "HHPHC" << "=" << value;
			break;

		case TELEINFO_TYPE_DEMAIN:
			if ( !ForecastPeriodUpdated && m_deviceCreated)
			{
				YADOMS_LOG(debug) << "DEMAIN" << "=" << value;
				
				m_ForecastPeriod.reset( new CForecastTomorrow( m_context, m_DeviceName, "ForecastColor", m_KeywordDetails ));
				std::string temp(value);
				m_ForecastPeriod->SetValue ( temp );
				m_KeywordList.push_back ( m_ForecastPeriod->GetHistorizable() );
				ForecastPeriodUpdated = true;
			}
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
