// Generated file, don't modify
#pragma once

class CManufacturers {
public:
   CManufacturers();
   virtual ~CManufacturers();
public:
enum EManufacturerIds {
   kPeha = 0x001,
   kThermokon = 0x002,
   kServodan = 0x003,
   kEchoFlex_Solutions = 0x004,
   kOmnio_AG = 0x005,
   kHardmeier_electronics = 0x006,
   kRegulvar_Inc = 0x007,
   kAd_Hoc_Electronics = 0x008,
   kDistech_Controls = 0x009,
   kKieback__plus__Peter = 0x00A,
   kEnOcean_GmbH = 0x00B,
   kProbare__ViCOS_GmbH = 0x00C,
   kEltako = 0x00D,
   kLeviton = 0x00E,
   kHoneywell = 0x00F,
   kSpartan_Peripheral_Devices = 0x010,
   kSiemens = 0x011,
   kT_Mac = 0x012,
   kReliable_Controls_Corporation = 0x013,
   kElsner_Elektronik_GmbH = 0x014,
   kDiehl_Controls = 0x015,
   kBSC_Computer = 0x016,
   kS_plus_S_Regeltechnik_GmbH = 0x017,
   kZENO_Controls__LLC = 0x018,
   kIntesis_Software_SL = 0x019,
   kViessmann = 0x01A,
   kLutuo_Technology = 0x01B,
   kSchneider_Electric = 0x01C,
   kSauter = 0x01D,
   kBoot_Up = 0x01E,
   kOsram_Sylvania = 0x01F,
   kUnotech = 0x020,
   kDelta_Controls_Inc_ = 0x021,
   kUnitronic_AG = 0x022,
   kNanoSense = 0x023,
   kThe_S4_Group = 0x024,
   kMSR_Solutions_GmbH = 0x025,
   kGE = 0x026,
   kMaico = 0x027,
   kRuskin_Company = 0x028,
   kMagnum_Energy_Solutions = 0x029,
   kKMC_Controls = 0x02A,
   kEcologix_Controls = 0x02B,
   kTrio2Sys = 0x02C,
   kAfriso_Euro_Index = 0x02D,
   kNEC_Access_Technica_Ltd_ = 0x030,
   kITEC_Corporation = 0x031,
   kSIMICX_Co__Ltd_ = 0x032,
   kEUROtronic_Technology_GmbH = 0x034,
   kART_Japan_Co__Ltd_ = 0x035,
   kTIANSU_Automation_Control_System_Co__Ltd_ = 0x036,
   kGruppo_Giordano__Idea_s_p_a_ = 0x038,
   kalphaEOS_AG = 0x039,
   kTAG_technologies__France = 0x03A,
   kWattstopper = 0x03B,
   kPressac_Communications_Ltd_ = 0x03C,
   kGIGA_concept__France = 0x03E,
   kSensortec_AG__Switzerland = 0x03F,
   kJaeger_Direkt__Germany = 0x040,
   kAir_System_Components_Inc___USA = 0x041,
   kERMINE_Corp_ = 0x042,
   kSODA_GmbH = 0x043,
   kEKE_Automation_Oy = 0x044,
   kHolter__Regelarmaturen__GmbH_CO__KG = 0x045,
   kID_RF = 0x046,
   kDEUTA_Controls_GmbH = 0x047,
   kMulti_user_Manufacturer_ID = 0x7FF,
};
   static CManufacturers::EManufacturerIds toManufacturerId(unsigned int id);
   static const std::string& name(unsigned int id);
   static const std::string& name(EManufacturerIds id);
protected:
private:
   static const std::map<unsigned int, std::string> ManufacturersMap;
};

