//
// CHardwarePluginQualifier.h
//
// Yadoms-plugin qualifier
//
#pragma once

#include "IHardwarePluginQualifier.h"

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

   private:
      const std::string m_name;
      const std::string m_version;
      const IHardwarePluginInformation::EReleaseType m_releaseType;

      friend CPluginIdentityCompare;
   };

   //--------------------------------------------------------------
   /// \brief	Compare operator (used in map)
   //--------------------------------------------------------------
   class CPluginIdentityCompare// TODO à déplacer dans un autre fichier ?
   {
   public:
      bool operator() (const CPluginIdentity& lhs, const CPluginIdentity& rhs)
      {
         if (lhs.m_name != rhs.m_name)
            return lhs.m_name < rhs.m_name;
         if (lhs.m_version != rhs.m_version)
            return lhs.m_version < rhs.m_version;
         return lhs.m_releaseType < rhs.m_releaseType;
      }
   };

public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CHardwarePluginQualifier();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginQualifier();

   // IHardwarePluginQualifier implementation
   virtual void signalLoad(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   virtual void signalUnload(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   virtual void signalCrash(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation, const std::string& exceptionName);
   virtual bool isSafe(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   virtual int getQualityLevel(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   // [END] IHardwarePluginQualifier implementation

private:
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
   QualityIndicatorsCache m_qualityIndicatorsCache;//TODO : protect by mutex
};
