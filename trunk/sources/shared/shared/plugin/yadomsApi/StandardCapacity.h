#pragma once
#include <shared/Export.h>
#include <shared/plugin/yadomsApi/KeywordAccessMode.h>
#include <shared/plugin/yadomsApi/KeywordDataType.h>

namespace shared { namespace plugin { namespace yadomsApi {

   //--------------------------------------------------------------
   /// \brief		Inner class which describe a standard capacity
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CStandardCapacity
   {
   public:
      //--------------------------------------------------------------
      /// \brief		Constructor
      /// \param [in] name          the capacity name
      /// \param [in] unit          the capacity unit
      /// \param [in] accessMode    the capacity access mode
      /// \param [in] type          the capacity type
      //--------------------------------------------------------------
      CStandardCapacity(const std::string& name, const std::string& unit, const EKeywordAccessMode& accessMode, const EKeywordDataType& type);

      //--------------------------------------------------------------
      /// \brief		Destructor
      //--------------------------------------------------------------
      virtual ~CStandardCapacity();

      //--------------------------------------------------------------
      /// \brief		get the capacity name
      /// \return 	the capacity name
      //--------------------------------------------------------------
      const std::string & getName() const;
      //--------------------------------------------------------------
      /// \brief		get the capacity unit
      /// \return 	the capacity unit
      //--------------------------------------------------------------
      const std::string & getUnit() const;
      //--------------------------------------------------------------
      /// \brief		get the capacity access mode
      /// \return 	the capacity access mode
      //--------------------------------------------------------------
      const EKeywordAccessMode& getAccessMode() const;
      //--------------------------------------------------------------
      /// \brief		get the capacity type
      /// \return 	the capacity type
      //--------------------------------------------------------------
      const EKeywordDataType& getType() const;

      //--------------------------------------------------------------
      /// \brief		Implicit std::string operator (give the capacity name)
      /// \return 	the capacity name
      //--------------------------------------------------------------
      operator const std::string & () const;

   private:
      //--------------------------------------------------------------
      /// \brief		the capacity name
      //--------------------------------------------------------------
      std::string m_name;
      //--------------------------------------------------------------
      /// \brief		the capacity unit
      //--------------------------------------------------------------
      std::string m_unit;
      //--------------------------------------------------------------
      /// \brief		the capacity access mode
      //--------------------------------------------------------------
      EKeywordAccessMode m_accessMode;
      //--------------------------------------------------------------
      /// \brief		the capacity type
      //--------------------------------------------------------------
      EKeywordDataType m_type;
   };

} } } // namespace shared::plugin::yadomsApi

