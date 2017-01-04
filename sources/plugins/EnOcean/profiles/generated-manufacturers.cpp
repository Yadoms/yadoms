// Generated file, don't modify
#include "stdafx.h"
#include "generated-manufacturers.h"

const std::map<unsigned int, std::string> CManufacturers::ManufacturersMap = boost::assign::map_list_of
   (kPeha, "Peha")
   (kThermokon, "Thermokon")
   (kServodan, "Servodan")
   (kEchoFlex_Solutions, "EchoFlex Solutions")
   (kOmnio_AG, "Omnio AG")
   (kHardmeier_electronics, "Hardmeier electronics")
   (kRegulvar_Inc, "Regulvar Inc")
   (kAd_Hoc_Electronics, "Ad Hoc Electronics")
   (kDistech_Controls, "Distech Controls")
   (kKieback__plus__Peter, "Kieback + Peter")
   (kEnOcean_GmbH, "EnOcean GmbH")
   (kProbare__ViCOS_GmbH, "Probare, ViCOS GmbH")
   (kEltako, "Eltako")
   (kLeviton, "Leviton")
   (kHoneywell, "Honeywell")
   (kSpartan_Peripheral_Devices, "Spartan Peripheral Devices")
   (kSiemens, "Siemens")
   (kT_Mac, "T-Mac")
   (kReliable_Controls_Corporation, "Reliable Controls Corporation")
   (kElsner_Elektronik_GmbH, "Elsner Elektronik GmbH")
   (kDiehl_Controls, "Diehl Controls")
   (kBSC_Computer, "BSC Computer")
   (kS_plus_S_Regeltechnik_GmbH, "S+S Regeltechnik GmbH")
   (kZENO_Controls__LLC, "ZENO Controls, LLC")
   (kIntesis_Software_SL, "Intesis Software SL")
   (kViessmann, "Viessmann")
   (kLutuo_Technology, "Lutuo Technology")
   (kSchneider_Electric, "Schneider Electric")
   (kSauter, "Sauter")
   (kBoot_Up, "Boot-Up")
   (kOsram_Sylvania, "Osram Sylvania")
   (kUnotech, "Unotech")
   (kDelta_Controls_Inc_, "Delta Controls Inc.")
   (kUnitronic_AG, "Unitronic AG")
   (kNanoSense, "NanoSense")
   (kThe_S4_Group, "The S4 Group")
   (kMSR_Solutions_GmbH, "MSR Solutions GmbH")
   (kGE, "GE")
   (kMaico, "Maico")
   (kRuskin_Company, "Ruskin Company")
   (kMagnum_Energy_Solutions, "Magnum Energy Solutions")
   (kKMC_Controls, "KMC Controls")
   (kEcologix_Controls, "Ecologix Controls")
   (kTrio2Sys, "Trio2Sys")
   (kAfriso_Euro_Index, "Afriso-Euro-Index")
   (kNEC_Access_Technica_Ltd_, "NEC Access Technica Ltd.")
   (kITEC_Corporation, "ITEC Corporation")
   (kSIMICX_Co__Ltd_, "SIMICX Co. Ltd.")
   (kEUROtronic_Technology_GmbH, "EUROtronic Technology GmbH")
   (kART_Japan_Co__Ltd_, "ART Japan Co. Ltd.")
   (kTIANSU_Automation_Control_System_Co__Ltd_, "TIANSU Automation Control System Co. Ltd.")
   (kGruppo_Giordano__Idea_s_p_a_, "Gruppo Giordano, Idea s.p.a.")
   (kalphaEOS_AG, "alphaEOS AG")
   (kTAG_technologies__France, "TAG technologies, France")
   (kWattstopper, "Wattstopper")
   (kPressac_Communications_Ltd_, "Pressac Communications Ltd.")
   (kGIGA_concept__France, "GIGA-concept, France")
   (kSensortec_AG__Switzerland, "Sensortec AG, Switzerland")
   (kJaeger_Direkt__Germany, "Jaeger Direkt, Germany")
   (kAir_System_Components_Inc___USA, "Air System Components Inc., USA")
   (kERMINE_Corp_, "ERMINE Corp.")
   (kSODA_GmbH, "SODA GmbH")
   (kEKE_Automation_Oy, "EKE-Automation Oy")
   (kHolter__Regelarmaturen__GmbH_CO__KG, "Holter  Regelarmaturen  GmbH CO. KG")
   (kID_RF, "ID-RF")
   (kDEUTA_Controls_GmbH, "DEUTA Controls GmbH")
   (kMulti_user_Manufacturer_ID, "Multi user Manufacturer ID")
;

CManufacturers::CManufacturers()
{}
CManufacturers::~CManufacturers()
{}
CManufacturers::EManufacturerIds CManufacturers::toManufacturerId(unsigned int id) {
   if (ManufacturersMap.find(id) == ManufacturersMap.end())
      throw std::out_of_range("Unknown manufacturer");
   return static_cast<EManufacturerIds>(id);

}
const std::string& CManufacturers::name(unsigned int id) {
   try {
      return ManufacturersMap.at(id);
   } catch(std::out_of_range&) {
      static const std::string UnknownManufacturer("Unknown manufacturer");
      return UnknownManufacturer;
   }
}
const std::string& CManufacturers::name(EManufacturerIds id) {
   return name(static_cast<unsigned int>(id));
}


