#pragma once

#include <string_view>

class ExecutableRun
{
public:
    virtual void Run(const std::wstring_view path_to_executable);
    ExecutableRun();
    virtual ~ExecutableRun();
};
