#pragma once

#include <string>

class PathToCurrentExecutable
{
public:
    virtual std::wstring GetPath();

    // Future-proofing
    PathToCurrentExecutable();
    virtual ~PathToCurrentExecutable();
    PathToCurrentExecutable(const PathToCurrentExecutable&) = delete;
    PathToCurrentExecutable& operator=(const PathToCurrentExecutable&) = delete;
    PathToCurrentExecutable(PathToCurrentExecutable&&) = delete;
    PathToCurrentExecutable& operator=(PathToCurrentExecutable&&) = delete;
};
