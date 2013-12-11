#pragma once

class CConfiguration
{
public:
   CConfiguration()
   {
   }

   virtual ~CConfiguration()
   {
   }

public:
   const std::string getSection() const {return m_section; }
   void setSection(const std::string & newSection) { m_section = newSection; }

   const std::string getName() const { return m_name; }
   void setName(const std::string newName) { m_name = newName; }

   const std::string getValue() const {return m_value; }
   void setValue(const std::string newValue) { m_value = newValue; }

   const std::string getDefaultValue() const {return m_defaultValue; }
   void setDefaultValue(const std::string newDefaultValue) { m_defaultValue = newDefaultValue; }

   const std::string getDescription() const {return m_description; }
   void setDescription(const std::string newDescription) { m_description = newDescription; }

   const boost::gregorian::date getLastModificationDate() const {return m_lastModificationDate; }
   void setLastModificationDate(const boost::gregorian::date newLastModificationDate) { m_lastModificationDate = newLastModificationDate; }

private:
   std::string m_section;
   std::string m_name;
   std::string m_value;
   std::string m_defaultValue;
   std::string m_description;
   boost::gregorian::date m_lastModificationDate;
};