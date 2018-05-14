#pragma once
#include "rapidjson/document.h"     // rapidjson
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
typedef rapidjson::GenericDocument<rapidjson::UTF16<> > JSON_Document;
typedef rapidjson::GenericValue<rapidjson::UTF16<> > JSON_Value;


typedef rapidjson::GenericStringBuffer<rapidjson::UTF16<> > JSON_StringBuffer;
typedef rapidjson::Writer<JSON_StringBuffer, rapidjson::UTF16<wchar_t>, rapidjson::UTF16<wchar_t>> JSON_Writer;
typedef rapidjson::MemoryPoolAllocator<> JSON_Allocator;

class Serializable
{
public:
	virtual bool Deserialize(const JSON_Value& value) = 0;
	virtual bool Serialize(JSON_Value& value, JSON_Allocator& allocator) = 0;
};