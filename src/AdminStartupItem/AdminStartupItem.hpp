#pragma once

#include <string_view>

class AdminStartupItem
{
public:
    virtual void Create(std::wstring_view executable_path, std::wstring_view task_name);
    virtual bool Exists(std::wstring_view task_name);

    // Future-proofing
    AdminStartupItem();
    virtual ~AdminStartupItem();
    AdminStartupItem(const AdminStartupItem&) = delete;
    AdminStartupItem& operator=(const AdminStartupItem&) = delete;
    AdminStartupItem(AdminStartupItem&&) = delete;
    AdminStartupItem& operator=(AdminStartupItem&&) = delete;
};
