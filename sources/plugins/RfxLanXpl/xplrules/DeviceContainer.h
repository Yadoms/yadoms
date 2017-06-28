#pragma once
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include "DeviceIdentifier.h"

namespace xplrules
{
   //------------------------------------
   ///\brief Class container for a device with its keywords.
   //------------------------------------
   class CDeviceContainer
   {
   public:
      //------------------------------------
      ///\brief Constructor
      ///\param [in] deviceIdentifier  The device identifier
      //------------------------------------
      explicit CDeviceContainer(const CDeviceIdentifier& deviceIdentifier);

      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~CDeviceContainer();

      //------------------------------------
      ///\brief Add a keyword to the device
      ///\param [in] keyword  The keyword to add
      //------------------------------------
      void addKeyword(boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> keyword);

      //------------------------------------
      ///\brief Get the keywords
      ///\return the keywords
      //------------------------------------
      const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >& getKeywords() const;

      //------------------------------------
      ///\brief Get the device identifier
      ///\return the device identifier
      //------------------------------------
      const CDeviceIdentifier& getDeviceIdentifier() const;

   private:
      //------------------------------------
      ///\brief The device identifier
      //------------------------------------
      CDeviceIdentifier m_deviceIdentifier;

      //------------------------------------
      ///\brief The device keywords
      //------------------------------------
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> > m_keywords;
   };
} //namespace xplrules


