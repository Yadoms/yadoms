#pragma once
#include <string>

struct HueInformations
{
   std::string ip;
   std::string friendlyName;
   std::string modelName;
   std::string modelNumber;
};

struct HueLightInformations
{
   struct STATE
   {
      bool on;
      int bri;
      int hue;
      int sat;
      std::string effect;
      struct XY
      {
         int x;
         int y;
      };
      XY xy;

      int ct;
      std::string alert;
      std::string colormode;
      std::string mode;
      bool reachable;
   };
   STATE state;

   struct SWUPDATE
   {
      std::string state;
      std::string lastinstall;
   };
   SWUPDATE swupdate;

   std::string type;
   std::string name;
   std::string modelid;
   std::string manufacturername;
   std::string productname;

   struct CAPABILITIES
   {
      bool certified;
      struct CONTROL
      {
         int mindimlevel;
         int maxlumen;
         std::string colorgamuttype;
         struct COLORGAMUT
         {
            
         };
         COLORGAMUT colorgamut;

         struct CT
         {
            int min;
            int max;
         };
         CT ct;
      };
      CONTROL control;
      struct STREAMING
      {
         bool renderer;
         bool proxy;
      };
      STREAMING streaming;

   };
   CAPABILITIES capabilities;

   struct CONFIG
   {
      std::string archetype;
      std::string function;
      std::string direction;
   };
   CONFIG config;

   std::string uniqueid;
   std::string swversion;
};