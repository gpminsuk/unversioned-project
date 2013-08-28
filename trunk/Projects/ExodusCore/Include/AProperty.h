#pragma once

enum EPropertyType {
	Property_Array,
	Property_Object,
	Property_Integer,
	Property_String,
};

class AProperty {
public:
    AProperty(EPropertyType InPropertyType);
    virtual ~AProperty();

	EPropertyType PropertyType;
};
