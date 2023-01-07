/****************************************************************************
* test_library - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-29
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _DF_FILE_XML_H_
#define _DF_FILE_XML_H_

#include <vector>
#include <string>

typedef std::string String;
#define UTIL_DELETE(p) {if(p){delete p; p=nullptr;}}

class DfXMLAttribute;
class DfXMLElement;
class DfXMLDocument;

typedef std::vector<DfXMLAttribute> XMLAttributeVector;
typedef std::vector<DfXMLElement*> XMLElementPtrVector;


////////////////////////// DfXMLAttribute /////////////////////
class DfXMLAttribute
{
    friend class DfXMLElement;

public:
    DfXMLAttribute();
    DfXMLAttribute(const String& strName, const String& strValue);
    virtual ~DfXMLAttribute();

public:
protected:
    String m_strName;
    String m_strValue;

public:
    const String& GetName() const { return m_strName; }
    void SetName(const String& strName) { m_strName = strName; }
    const String& GetValue() const { return m_strValue; }
    void SetValue(const String& strValue) { m_strValue = strValue; }

};

////////////////////////// DfXMLElement ///////////////////////
class DfXMLElement
{
    friend class DfXMLDocument;

public:
    DfXMLElement();
    DfXMLElement(const String& strName);
    DfXMLElement(const String& strName, const String& strValue);
    virtual ~DfXMLElement();

public:
protected:
    String m_strName;
    String m_strValue;
    XMLAttributeVector m_aAttributes;

    DfXMLElement* m_pElementParent;
    XMLElementPtrVector m_aElementChildren;

public:
    const String& GetName() const { return m_strName; }
    void SetName(const String& strName) { m_strName = strName; }
    const String& GetValue() const { return m_strValue; }
    void SetValue(const String& strValue) { m_strValue = strValue; }
    const XMLAttributeVector& GetAttributes() const { return m_aAttributes; }
    XMLAttributeVector& GetAttributes() { return m_aAttributes; }
    DfXMLElement* GetElementParent() const { return m_pElementParent; }
    const XMLElementPtrVector& GetElementChildren() const { return m_aElementChildren; }
    XMLElementPtrVector& GetElementChildren() { return m_aElementChildren; }

public:
    void Destroy();

    DfXMLElement* SetElementParent(DfXMLElement* pElementParent);

    int GetElementChildrenCount() const;
    DfXMLElement* GetElementChild(int index);

    DfXMLElement* InsertElementChild(DfXMLElement* pElementChild, int index);
    DfXMLElement* AddElementChild(DfXMLElement* pElementChild);

    DfXMLElement* FindElementChild(const String& strName);
    void FindElementChildren(const String& strName, XMLElementPtrVector& aElementChildren);

    void RemoveElementChild(DfXMLElement* pElementChild);
    void RemoveElementChildren(const String& strName, XMLElementPtrVector& aElementChildren);
    void DeleteElementChildren(const String& strName);

    void DeleteElements(XMLElementPtrVector& aElements);

    void AddAttribute(const String& strName, const String& strValue);
    void AddAttribute(DfXMLAttribute& attribute);
    void AddAttributes(XMLAttributeVector& aAttributes);
};

////////////////////////// DfXMLDocument //////////////////////
class DfXMLDocument
{
public:
    DfXMLDocument();
    virtual ~DfXMLDocument();

public:
protected:
    DfXMLElement* m_pElementDeclaration;
    DfXMLElement* m_pElementRoot;

public:
    DfXMLElement* GetElementDeclaration() const { return m_pElementDeclaration; }
    DfXMLElement* GetElementRoot() const { return m_pElementRoot; }

public:
    void Destroy();
    
    DfXMLElement* CreateElementDeclaration(const String& strVersion = "1.0", const String& strEncoding = "utf-8", const String& strStandalone = "");
    DfXMLElement* CreateElementRoot(const String& strName);

    DfXMLElement* CreateElement(DfXMLElement* pElementParent);

};

////////////////////////// DfFileXML //////////////////////////
class DfFileXML
{
public:
    DfFileXML();
    virtual ~DfFileXML();

public:
protected:
    DfXMLDocument* m_pXMLDocument;

public:
    DfXMLDocument* GetXMLDocument() const { return m_pXMLDocument; }

public:
    void Destroy();

    void CreateXML();
    bool CreateXML(const String& strContent);
    bool CreateXML(const char* szContent);
    bool LoadXML(const String& strPath);
    bool SaveXML(const String& strPath);

private:
    bool parseXML(const char* szContent);
};

#endif