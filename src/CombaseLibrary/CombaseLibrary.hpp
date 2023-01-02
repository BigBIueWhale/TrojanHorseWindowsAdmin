#pragma once

class CombaseLibrary
{
public:
    CombaseLibrary();
    virtual ~CombaseLibrary();
    CombaseLibrary(const CombaseLibrary&) = delete;
    CombaseLibrary& operator=(const CombaseLibrary&) = delete;
    CombaseLibrary(CombaseLibrary&&) = delete;
    CombaseLibrary& operator=(CombaseLibrary&&) = delete;
};
