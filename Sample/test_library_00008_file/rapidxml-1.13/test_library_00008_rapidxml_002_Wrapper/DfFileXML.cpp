/****************************************************************************
* test_library - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-29
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "DfFileXML.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <map>
#include <assert.h>
#include <algorithm>
#include <memory>

#include "rapidxml_ext.hpp"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
using namespace rapidxml;


////////////////////////// DfXMLAttribute /////////////////////
DfXMLAttribute::DfXMLAttribute()
    : m_strName("")
    , m_strValue("")
{

}

DfXMLAttribute::DfXMLAttribute(const String& strName, const String& strValue)
    : m_strName(strName)
    , m_strValue(strValue)
{

}

DfXMLAttribute::~DfXMLAttribute()
{

}

////////////////////////// DfXMLElement ///////////////////////
DfXMLElement::DfXMLElement()
    : m_strName("")
    , m_strValue("")
{

}

DfXMLElement::DfXMLElement(const String& strName)
    : m_strName(strName)
    , m_strValue("")
{

}

DfXMLElement::DfXMLElement(const String& strName, const String& strValue)
    : m_strName(strName)
    , m_strValue(strValue)
{

}

DfXMLElement::~DfXMLElement()
{

}

void DfXMLElement::Destroy()
{
    DeleteElements(m_aElementChildren);
}

DfXMLElement* DfXMLElement::SetElementParent(DfXMLElement* pElementParent)
{
    DfXMLElement* pParent = m_pElementParent;
    m_pElementParent = pElementParent;
    return pParent;
}

int DfXMLElement::GetElementChildrenCount() const
{
    return (int)m_aElementChildren.size();
}

DfXMLElement* DfXMLElement::GetElementChild(int index)
{
    if (index < 0 || index >= (int)m_aElementChildren.size())
        return nullptr;
    return m_aElementChildren[index];
}

DfXMLElement* DfXMLElement::InsertElementChild(DfXMLElement* pElementChild, int index)
{
    if (index < 0 || index >= (int)m_aElementChildren.size())
        return nullptr;
    pElementChild->m_pElementParent = this;
    m_aElementChildren.insert(m_aElementChildren.begin() + index, pElementChild);
    return pElementChild;
}

DfXMLElement* DfXMLElement::AddElementChild(DfXMLElement* pElementChild)
{
    pElementChild->m_pElementParent = this;
    m_aElementChildren.push_back(pElementChild);
    return pElementChild;
}

DfXMLElement* DfXMLElement::FindElementChild(const String& strName)
{
    size_t count = m_aElementChildren.size();
    for (size_t i = 0; i < count; i++)
    {
        DfXMLElement* pChild = m_aElementChildren[i];
        if (pChild->m_strName == strName)
            return pChild;
    }
    return nullptr;
}

void DfXMLElement::FindElementChildren(const String& strName, XMLElementPtrVector& aElementChildren)
{
    size_t count = m_aElementChildren.size();
    for (size_t i = 0; i < count; i++)
    {
        DfXMLElement* pChild = m_aElementChildren[i];
        if (pChild->m_strName == strName)
        {
            aElementChildren.push_back(pChild);
        }
    }
}

void DfXMLElement::RemoveElementChild(DfXMLElement* pElementChild)
{
    size_t count = m_aElementChildren.size();
    for (size_t i = 0; i < count; i++)
    {
        DfXMLElement* pChild = m_aElementChildren[i];
        if (pChild == pElementChild)
        {
            m_aElementChildren.erase(m_aElementChildren.begin() + i);
            return;
        }
    }
}

void DfXMLElement::RemoveElementChildren(const String& strName, XMLElementPtrVector& aElementChildren)
{
    XMLElementPtrVector aLeft;
    size_t count = m_aElementChildren.size();
    for (size_t i = 0; i < count; i++)
    {
        DfXMLElement* pChild = m_aElementChildren[i];
        if (pChild->m_strName == strName)
        {
            aElementChildren.push_back(pChild);
        }
        else
        {
            aLeft.push_back(pChild);
        }
    }
    m_aElementChildren.clear();
    m_aElementChildren = aLeft;
}

void DfXMLElement::DeleteElementChildren(const String& strName)
{
    XMLElementPtrVector aDelete;
    RemoveElementChildren(strName, aDelete);
    DeleteElements(aDelete);
}

void DfXMLElement::DeleteElements(XMLElementPtrVector& aElements)
{
    size_t count = aElements.size();
    for (size_t i = 0; i < count; i++)
    {
        UTIL_DELETE(aElements[i])
    }
    aElements.clear();
}

void DfXMLElement::AddAttribute(const String& strName, const String& strValue)
{
    m_aAttributes.push_back(DfXMLAttribute(strName, strValue));
}

void DfXMLElement::AddAttribute(DfXMLAttribute& attribute)
{
    m_aAttributes.push_back(attribute);
}

void DfXMLElement::AddAttributes(XMLAttributeVector& aAttributes)
{
    size_t count = aAttributes.size();
    for (size_t i = 0; i < count; i++)
    {
        AddAttribute(aAttributes[i]);
    }
}

////////////////////////// DfXMLDocument //////////////////////
DfXMLDocument::DfXMLDocument()
    : m_pElementDeclaration(nullptr) 
    , m_pElementRoot(nullptr)
{

}

DfXMLDocument::~DfXMLDocument()
{
    Destroy();
}

void DfXMLDocument::Destroy()
{
    UTIL_DELETE(m_pElementDeclaration)
    UTIL_DELETE(m_pElementRoot)
}

DfXMLElement* DfXMLDocument::CreateElementDeclaration(const String& strVersion /*= "1.0"*/, const String& strEncoding /*= "utf-8"*/, const String& strStandalone /*= ""*/)
{
    UTIL_DELETE(m_pElementDeclaration)
    m_pElementDeclaration = new DfXMLElement();
    if (!strVersion.empty())
    {
        DfXMLAttribute attrVersion("version", strVersion);
        m_pElementDeclaration->AddAttribute(attrVersion);
    }
    if (!strEncoding.empty())
    {
        DfXMLAttribute attrEncoding("encoding", strEncoding);
        m_pElementDeclaration->AddAttribute(attrEncoding);
    }
    if (!strStandalone.empty())
    {
        DfXMLAttribute attrStandalone("standalone", strStandalone);
        m_pElementDeclaration->AddAttribute(attrStandalone);
    }
    return m_pElementDeclaration;
}

DfXMLElement* DfXMLDocument::CreateElementRoot(const String& strName)
{
    UTIL_DELETE(m_pElementRoot)
    m_pElementRoot = new DfXMLElement(strName);
    return m_pElementRoot;
}

DfXMLElement* DfXMLDocument::CreateElement(DfXMLElement* pElementParent)
{
    DfXMLElement* pElement = new DfXMLElement();
    pElement->m_pElementParent = pElementParent;
    return pElement;
}

////////////////////////// DfFileXML //////////////////////////
DfFileXML::DfFileXML()
    : m_pXMLDocument(nullptr)
{

}

DfFileXML::~DfFileXML()
{
    Destroy();
}

void DfFileXML::Destroy()
{
    UTIL_DELETE(m_pXMLDocument)
}

void DfFileXML::CreateXML()
{
    Destroy();
    m_pXMLDocument = new DfXMLDocument();
}
bool DfFileXML::CreateXML(const String& strContent)
{
    return CreateXML(strContent.c_str());
}
bool DfFileXML::CreateXML(const char* szContent)
{
    CreateXML();
    return parseXML(szContent);
}

bool DfFileXML::LoadXML(const String& strPath)
{
    rapidxml::file<> file(strPath.c_str());
    if (file.data() == nullptr)
    {
        return false;
    }   
    CreateXML();
    return parseXML(file.data());
}

static void s_ParseAttributes(rapidxml::xml_node<>* pNode, DfXMLElement* pElement)
{
    rapidxml::xml_attribute<>* pAttribute = pNode->first_attribute();
    while (pAttribute != nullptr)
    {
        pElement->AddAttribute(String(pAttribute->name(), pAttribute->name_size()), 
                               String(pAttribute->value(), pAttribute->value_size()));
        pAttribute = pAttribute->next_attribute();
    }
}
static void s_ParseNode(rapidxml::xml_node<>* pNode, DfXMLElement* pElement)
{
    s_ParseAttributes(pNode, pElement);

    rapidxml::xml_node<>* pNodeChild = pNode->first_node();
    while (pNodeChild != nullptr)
    {
        DfXMLElement* pElementChild = pElement->AddElementChild(new DfXMLElement(String(pNodeChild->name(), pNodeChild->name_size())));
        s_ParseNode(pNodeChild, pElementChild);

        pNodeChild = pNodeChild->next_sibling();
    }
}

bool DfFileXML::parseXML(const char* szContent)
{
    rapidxml::xml_document<> doc;
    doc.parse<0>(const_cast<char*>(szContent));

    auto pNode = doc.first_node();
    while (pNode != nullptr)
    {
        switch (pNode->type())
        {
        case rapidxml::node_type::node_declaration:
            {
                DfXMLElement* pElementDeclaration = m_pXMLDocument->CreateElementDeclaration("", "", "");
                pElementDeclaration->SetName(String(pNode->name(), pNode->name_size()));
                s_ParseNode(pNode, pElementDeclaration);
            }
            break;
        case rapidxml::node_type::node_element:
            {
                DfXMLElement* pElementRoot = m_pXMLDocument->CreateElementRoot(String(pNode->name(), pNode->name_size()));
                s_ParseNode(pNode, pElementRoot);
            }
            break;
        default:
            break;
        }

        pNode = pNode->next_sibling();
    }
    return true;
}


static void s_AddAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* pNode, DfXMLElement* pElement)
{
    XMLAttributeVector& atrributes = pElement->GetAttributes();
    size_t count = atrributes.size();
    for (size_t i = 0; i < count; i++)
    {
        DfXMLAttribute& attribute = atrributes[i];
        pNode->append_attribute(doc.allocate_attribute(attribute.GetName().c_str(), attribute.GetValue().c_str()));
    }
}
static rapidxml::xml_node<>* s_AddNode(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* pNodeParent, DfXMLElement* pElement, rapidxml::node_type type = rapidxml::node_element)
{
    rapidxml::xml_node<>* pChild = doc.allocate_node(type, pElement->GetName().c_str());
    pNodeParent->append_node(pChild);
    s_AddAttributes(doc, pChild, pElement);
    XMLElementPtrVector& aElements = pElement->GetElementChildren();
    size_t count = aElements.size();
    for (size_t i = 0; i < count; i++)
    {
        DfXMLElement* pCC = aElements[i];
        s_AddNode(doc, pChild, pCC);
    }
    return pChild;
}
static rapidxml::xml_node<>* s_AddRootNode(rapidxml::xml_document<>& doc, DfXMLElement* pElement, rapidxml::node_type type)
{
    rapidxml::xml_node<>* pNode = doc.allocate_node(type, pElement->GetName().c_str());
    doc.append_node(pNode);
    s_AddAttributes(doc, pNode, pElement);
    XMLElementPtrVector& aElements = pElement->GetElementChildren();
    size_t count = aElements.size();
    for (size_t i = 0; i < count; i++)
    {
        DfXMLElement* pChild = aElements[i];
        s_AddNode(doc, pNode, pChild);
    }
    return pNode;
}


bool DfFileXML::SaveXML(const String& strPath)
{
    if (m_pXMLDocument == nullptr)
    {
        return false;
    }
    
    DfXMLElement* pElementDeclaration = m_pXMLDocument->GetElementDeclaration();
    DfXMLElement* pElementRoot = m_pXMLDocument->GetElementRoot();
    if (pElementDeclaration == nullptr ||
        pElementRoot == nullptr)
    {
        return false;
    }

    //1> Doc
    rapidxml::xml_document<> doc;
    //2> Declaration
    rapidxml::xml_node<>* declaration = s_AddRootNode(doc, pElementDeclaration, rapidxml::node_declaration);
    //3> Node
    rapidxml::xml_node<>* root = s_AddRootNode(doc, pElementRoot, rapidxml::node_element);
    //4> Save file
    std::ofstream outFile(strPath.c_str(), std::ios::out);
    if (outFile)
    {
        outFile << doc;
        outFile.close();
        return true;
    }
    
    return false;    
}