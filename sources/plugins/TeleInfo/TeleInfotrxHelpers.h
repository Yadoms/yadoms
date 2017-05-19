#pragma once

#define TELEINFO_BAUD_RATE         1200
#define TELEINFO_PARITY            boost::asio::serial_port_base::parity::even
#define TELEINFO_CARACTER_SIZE     7
#define TELEINFO_FLOW_CONTROL      boost::asio::serial_port_base::flow_control::none
#define TELEINFO_STOP_BITS         boost::asio::serial_port_base::stop_bits::one

//Teleinfo for EDF power meter. Only "Base" and "Heures creuses" are suported

//Teleinfo official specification :
//http://www.planete-domotique.com/notices/ERDF-NOI-CPT_O2E.pdf

//Example of data received by EDF power meter
//ADCO 271028237723 C
//OPTARIF HC.. <
//ISOUSC 45 ?
//HCHC 013149843 '
//HCHP 013016759 3
//PTEC HP..
//IINST 002 Y
//IMAX 049 L
//PAPP 00450 *
//HHPHC D /
//MOTDETAT 000000 B

#define TE_ADCO "ADCO" //meter id
#define TE_OPTARIF "OPTARIF"//pricing option
#define TE_ISOUSC "ISOUSC"//current power subscribe   //A
#define TE_BASE "BASE"//total power usage normal tariff in base option
#define TE_HCHC "HCHC"// total power usage low tariff in HC option
#define TE_HCHP "HCHP"// total power usage normal tariff in HC option
#define TE_EJPHPM "EJPHPM"// total power usage normal tariff in PM option
#define TE_EJPHN "EJPHN"// total power usage low tariff in HN option
#define TE_BBRHCJB "BBRHCJB"// total power usage low tariff in HC option tempo blue
#define TE_BBRHPJB "BBRHPJB"// total power usage normal tariff in HC option tempo blue
#define TE_BBRHCJW "BBRHCJW"// total power usage low tariff in HC option tempo white
#define TE_BBRHPJW "BBRHPJW"// total power usage normal tariff in HC option tempo white
#define TE_BBRHCJR "BBRHCJR"// total power usage low tariff in HC option tempo red
#define TE_BBRHPJR "BBRHPJR"// total power usage normal tariff in HC option tempo red
#define TE_PTEC   "PTEC"//current tariff period
#define TE_IINST "IINST"//instant current power usage
#define TE_IMAX "IMAX"//maximal current power usage
#define TE_PAPP "PAPP"//apparent power
#define TE_HHPHC "HHPHC" // Change between LowCost to normal cost period 1 car (LowCost or TEMPO)
#define TE_DEMAIN "DEMAIN" // Color of the next day
#define TE_ADPS "ADPS" // Warning of threshold overrun Ampère
#define TE_MOTDETAT "MOTDETAT" //MOTDETAT


typedef enum {
	TELEINFO_TYPE_ADCO,
	TELEINFO_TYPE_OPTARIF,
	TELEINFO_TYPE_ISOUSC,
	TELEINFO_TYPE_BASE,
	TELEINFO_TYPE_HCHC,
	TELEINFO_TYPE_HCHP,
	TELEINFO_TYPE_EJPHN,
	TELEINFO_TYPE_EJPHPM,
	TELEINFO_TYPE_BBRHCJB,
	TELEINFO_TYPE_BBRHPJB,
	TELEINFO_TYPE_BBRHCJW,
	TELEINFO_TYPE_BBRHPJW,
	TELEINFO_TYPE_BBRHCJR,
	TELEINFO_TYPE_BBRHPJR,
	TELEINFO_TYPE_PTEC,
	TELEINFO_TYPE_IINST,
	TELEINFO_TYPE_IMAX,
	TELEINFO_TYPE_PAPP,
    TELEINFO_TYPE_DEMAIN,
    TELEINFO_TYPE_HHPHC,
    TELEINFO_TYPE_ADPS,
    TELEINFO_TYPE_MOTDETAT
} Type;

typedef struct _tMatch 
{
	int width;
} Match;

typedef std::map<std::string, unsigned int> EnumValuesTypes;

static const int readBufferSize = 1028;
