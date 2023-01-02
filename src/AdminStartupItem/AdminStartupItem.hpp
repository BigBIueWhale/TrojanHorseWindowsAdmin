#pragma once

#include <string>

class AdminStartupItem
{
public:
    virtual void Create(std::wstring executable_path);

    // Future-proofing
    AdminStartupItem();
    virtual ~AdminStartupItem();
    AdminStartupItem(const AdminStartupItem&) = delete;
    AdminStartupItem& operator=(const AdminStartupItem&) = delete;
    AdminStartupItem(AdminStartupItem&&) = delete;
    AdminStartupItem& operator=(AdminStartupItem&&) = delete;
};
