#pragma once

class AdminCheck
{
public:
    virtual bool IsAdmin();

    // Make future-proof by avoiding assumptions that the user might make
    AdminCheck();
    virtual ~AdminCheck();
    AdminCheck(const AdminCheck&) = delete;
    AdminCheck& operator=(const AdminCheck&) = delete;
    AdminCheck(AdminCheck&&) = delete;
    AdminCheck& operator=(AdminCheck&&) = delete;
};
