#pragma once

#include <string_view>

class HiddenFileCreate
{
public:
    virtual void Create(std::wstring_view path);
    HiddenFileCreate();
    virtual ~HiddenFileCreate();
};
