#pragma once
#include <shared/StringExtension.h>
#include "Common.h"

namespace owfs { namespace io {

   //--------------------------------------------------------------
   /// \brief	Single read-only data io accessor interface
   /// \template Interface Base interface
   /// \template T Value type, must correspond to Interface value type
   //--------------------------------------------------------------
   template<class Interface, typename T>
   class CSingleReadOnlyData : public Interface
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	devicePath Device path
      /// \param[in]	valueSubPath OWFS value sub-path
      //--------------------------------------------------------------
      CSingleReadOnlyData(const boost::filesystem::path& devicePath, const boost::filesystem::path& valueSubPath)
         :m_valuePath(devicePath / valueSubPath)
      {
      }

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSingleReadOnlyData()
      {
      }

   protected:
      // Interface implementation
      virtual T read() const
      {
         std::string readValue = CCommon::read(m_valuePath);

         if (readValue.empty())
            return 0.0;
         return shared::CStringExtension::parse<T>(readValue.c_str());
      }
      // [END] Interface implementation

   private:
      //--------------------------------------------------------------
      /// \brief	OWFS device value path
      //--------------------------------------------------------------
      const boost::filesystem::path m_valuePath;
   };

} } // namespace owfs::io