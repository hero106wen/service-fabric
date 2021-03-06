// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace std;
using namespace Common;
using namespace ServiceModel;

DriverOptionDescription::DriverOptionDescription()
    : Name(),
    Value(),
    IsEncrypted(L"false")
{
}

bool DriverOptionDescription::operator== (DriverOptionDescription const & other) const
{
    return (StringUtility::AreEqualCaseInsensitive(Name, other.Name) &&
        StringUtility::AreEqualCaseInsensitive(Value, other.Value) &&
        StringUtility::AreEqualCaseInsensitive(IsEncrypted, other.IsEncrypted));
}

bool DriverOptionDescription::operator!= (DriverOptionDescription const & other) const
{
    return !(*this == other);
}

void DriverOptionDescription::WriteTo(TextWriter & w, FormatOptions const &) const
{
    w.Write("DriverOptionDescription { ");
    w.Write("Name = {0}, ", Name);
    w.Write("Value = {0}, ", Value);
    w.Write("IsEncrypted = {0}, ", IsEncrypted);
    w.Write("}");
}

void DriverOptionDescription::ReadFromXml(
    XmlReaderUPtr const & xmlReader)
{
    xmlReader->StartElement(
        *SchemaNames::Element_DriverOption,
        *SchemaNames::Namespace);

    this->Name = xmlReader->ReadAttributeValue(*SchemaNames::Attribute_Name);
    this->Value = xmlReader->ReadAttributeValue(*SchemaNames::Attribute_Value);

    if (xmlReader->HasAttribute(*SchemaNames::Attribute_IsEncrypted))
    {
        this->IsEncrypted = xmlReader->ReadAttributeValue(*SchemaNames::Attribute_IsEncrypted);
    }

    // Read the rest of the empty element
    xmlReader->ReadElement();
}

Common::ErrorCode DriverOptionDescription::WriteToXml(XmlWriterUPtr const & xmlWriter)
{	//<DriverOption>
    ErrorCode er = xmlWriter->WriteStartElement(*SchemaNames::Element_DriverOption, L"", *SchemaNames::Namespace);
    if (!er.IsSuccess())
    {
        return er;
    }
    er = xmlWriter->WriteAttribute(*SchemaNames::Attribute_Name, this->Name);
    if (!er.IsSuccess())
    {
        return er;
    }
    er = xmlWriter->WriteAttribute(*SchemaNames::Attribute_Value, this->Value);
    if (!er.IsSuccess())
    {
        return er;
    }
    er = xmlWriter->WriteAttribute(*SchemaNames::Attribute_IsEncrypted, this->IsEncrypted);
    if(!er.IsSuccess())
    {
        return er;
    }
    //</DriverOption>
    return xmlWriter->WriteEndElement();
}

void DriverOptionDescription::clear()
{
    this->Name.clear();
    this->Value.clear();
    this->IsEncrypted.clear();
}

ErrorCode DriverOptionDescription::ToPublicApi(
    __in ScopedHeap & heap,
    __out FABRIC_CONTAINER_DRIVER_OPTION_DESCRIPTION & fabricDriverOptionDesc) const
{
    fabricDriverOptionDesc.Name = heap.AddString(this->Name);
    fabricDriverOptionDesc.Value = heap.AddString(this->Value);

    if (StringUtility::AreEqualCaseInsensitive(this->IsEncrypted, L"true"))
    {
        fabricDriverOptionDesc.IsEncrypted = true;
    }
    else
    {
        fabricDriverOptionDesc.IsEncrypted = false;
    }

    fabricDriverOptionDesc.Reserved = nullptr;

    return ErrorCode::Success();
}

