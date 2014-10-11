#pragma once

#include "HistorizableType.h"

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief A RainRate historizable object
   //-----------------------------------------------------
   class CRainRate : public CHistorizableType<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CRainRate(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CRainRate();

   };



} } } // namespace xplrules::rfxLanXpl::data

