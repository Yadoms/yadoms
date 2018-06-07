#pragma once

#include "IGPIOManager.h"
#include "ITeleInfoConfiguration.h"
#include <vector>
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------
/// \brief	A always null sequence number
//--------------------------------------------------------------
class CGPIOManager : public IGPIOManager
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   explicit CGPIOManager(boost::shared_ptr<ITeleInfoConfiguration> configuration) : m_currentPosition(0)
   {
      m_GPIO.clear();

      if (configuration->getEquipmentType() == TwoInputs)
      {
         if (configuration->getInputsActivated() == AllInputsActivated)
         {
            m_GPIO.push_back(1);
            m_GPIO.push_back(2);
         }
         else if (configuration->getInputsActivated() == Input1Activated)
         {
            m_GPIO.push_back(1);
         }
         else if (configuration->getInputsActivated() == Input2Activated)
         {
            m_GPIO.push_back(2);
         }
      }
      else
      {
         m_GPIO.push_back(1);
      }
   }

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CGPIOManager()
   {}

   // IGPIOManager implementation
   void front() override
   {
      YADOMS_LOG(debug) << "return to the front of the list";
      m_currentPosition = 0;
   }

   void next() override
   {
      YADOMS_LOG(debug) << "next equipement";
      ++m_currentPosition;
   }

   int getGPIO() override
   {
      if (m_currentPosition <= m_GPIO.size()-1)
         return m_GPIO[m_currentPosition];
      return INT32_MAX;
   }

   bool isLast() const override
   {
      return (m_currentPosition == m_GPIO.size() - 1);
   }

   // [END] ISequenceNumber implementation
private:
   unsigned char    m_currentPosition;
   std::vector<int> m_GPIO;
};