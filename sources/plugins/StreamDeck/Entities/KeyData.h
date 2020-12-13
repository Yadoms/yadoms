#pragma once

class CKeyData
{
public:

   CKeyData() = default;

   const std::string& getImg() const;
   void setImg(const std::string& img);
   const std::string& getCustomText() const;
   void setCustomText(const std::string& customText);
   bool isIsAlreadyPressed() const;
   void setIsAlreadyPressed(bool isAlreadyPressed);
private:
	std::string m_img;
	std::string m_customText;
	bool m_isAlreadyPressed;
};
