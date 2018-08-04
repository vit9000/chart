/*
#ifndef SOLUTION_TYPE_H_IDN393B83VJ
#define SOLUTION_TYPE_H_IDN393B83VJ

#include "DataContainer.h"
#include "comutil.h"

#define MAXIMUM_ENUM_VARIANT_TYPE VT_RESERVED

namespace SoData
{
    
enum _TypeData
{
    VARIANT = MAXIMUM_ENUM_VARIANT_TYPE+1,
    DATACONTAINER,
    COUNT_TYPES,
};

typedef unsigned long int TYPE;

class ITypedData
{
public:
    virtual ~ITypedData();
    
    virtual bool IsType(TYPE type) const;
    
private:
    friend class CursorData;
    
    ITypedData();
    ITypedData(const ITypedData & data);
};

class VariantData : public ITypedData, public PSObject
{
public:
    typedef _variant_t TVARIANT;
    
    virtual ~VariantData();
    
    virtual bool IsType(TYPE type) const;
    
    const TVARIANT & GetValue() const;
    void SetValue(const TVARIANT & value);
    
private:
    TVARIANT m_value;
};

};//namespace SoData

#endif //SOLUTION_TYPE_H_IDN393B83VJ
*/
