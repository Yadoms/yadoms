//
// CHardwarePluginQualifier.h
//
// Yadoms-plugin qualifier
//
#pragma once

#include "IHardwarePluginQualifier.h"
#include "database/IHardwareEventLoggerRequester.h"

//--------------------------------------------------------------
/// \brief	this class is used to qualify a plugin
//--------------------------------------------------------------
class CHardwarePluginQualifier : public IHardwarePluginQualifier
{
private:
   //--------------------------------------------------------------
   /// \brief	Internal plugin identity
   //--------------------------------------------------------------
   class CPluginIdentityCompare;
   class CPluginIdentity   // TODO à déplacer dans un autre fichier ?
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CPluginIdentity(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation)
         :m_name(pluginInformation->getName()), m_version(pluginInformation->getVersion()), m_releaseType(pluginInformation->getReleaseType())
      {
      }

      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CPluginIdentity(const std::string& name, const std::string& version, IHardwarePluginInformation::EReleaseType releaseType)
         :m_name(name), m_version(version), m_releaseType(releaseType)
      {
      }

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CPluginIdentity()
      {
      }

      //--------------------------------------------------------------
      /// \brief	   Name getter
      /// \return    Plugin name
      //--------------------------------------------------------------
      const std::string& getName() const
      {
         return m_name;
      }

      //--------------------------------------------------------------
      /// \brief	   Version getter
      /// \return    Plugin version
      //--------------------------------------------------------------
      const std::string& getVersion() const
      {
         return m_version;
      }

      //--------------------------------------------------------------
      /// \brief	   Release type getter
      /// \return    Plugin release type
      //--------------------------------------------------------------
      const IHardwarePluginInformation::EReleaseType getReleaseType() const
      {
         return m_releaseType;
      }

   private:
      const std::string m_name;
      const std::string m_version;
      const IHardwarePluginInformation::EReleaseType m_releaseType;
   };

   //--------------------------------------------------------------
   /// \brief	Compare operator (used in map)
   //--------------------------------------------------------------
   class CPluginIdentityCompare// TODO à déplacer dans un autre fichier ?
   {
   public:
      bool operator() (const CPluginIdentity& lhs, const CPluginIdentity& rhs)
      {
         if (lhs.getName() != rhs.getName())
            return lhs.getName() < rhs.getName();
         if (lhs.getVersion() != rhs.getVersion())
            return lhs.getVersion() < rhs.getVersion();
         return lhs.getReleaseType() < rhs.getReleaseType();
      }
   };

public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] eventLoggerDatabase    Event logger database
   //--------------------------------------------------------------
   CHardwarePluginQualifier(boost::shared_ptr<IHardwareEventLoggerRequester> eventLoggerDatabase);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginQualifier();

   // IHardwarePluginQualifier implementation
   virtual void signalLoad(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   virtual void signalUnload(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   virtual void signalCrash(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation, const std::string& reason);
   virtual bool isSafe(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   virtual int getQualityLevel(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   // [END] IHardwarePluginQualifier implementation

private:
   //--------------------------------------------------------------
   /// \brief	               Add an event to database
   /// \param[in]  pluginInformation     Plugin information (name, version...)
   /// \param[in]  eventType  Event type (load, unload, crash...)
   /// \param[in]  reason     Crash cause (exception...)
   //--------------------------------------------------------------
   void AddEventToDatabase(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation,
      CHardwareEventLogger::EEventType eventType, const std::string& reason = CStringExtension::EmptyString);

   //--------------------------------------------------------------
   /// \brief	               Make corresponding quality indicator obsolete in cache
   /// \param[in] identity    Plugin identity
   //--------------------------------------------------------------
   void obsoleteQualityIndicatorCache(const CPluginIdentity& identity);

   //--------------------------------------------------------------
   /// \brief	               compute quality indicator of a plugin
   /// \param[in] identity    Plugin identity
   /// \return                Computed quality, from 0(lower) to 100(best)
   //--------------------------------------------------------------
   int computeQuality(const CPluginIdentity& identity) const;

private:
   //--------------------------------------------------------------
   /// \brief	Safety threshold : a plugin with a quality upper or equal to
   ///         this threshold, is considered as safe.
   //--------------------------------------------------------------
   static const int m_SafetyThreshold;

   //--------------------------------------------------------------
   /// \brief	Cache of quality indicators
   ///         Key is plugin identity
   ///         Value is quality indicator
   ///         Need specific comparator
   //--------------------------------------------------------------
   typedef std::map<CPluginIdentity, int, CPluginIdentityCompare> QualityIndicatorsCache;
   QualityIndicatorsCache m_qualityIndicatorsCache;

   //--------------------------------------------------------------
   /// \brief	Quality indicators Cache mutex
   //--------------------------------------------------------------
   boost::mutex m_qualityIndicatorsCacheMutex;

   //--------------------------------------------------------------
   /// \brief	database access
   //--------------------------------------------------------------
   boost::shared_ptr<IHardwareEventLoggerRequester> m_eventLoggerDatabase;
};
