#pragma once

#include <string>

class AdminTaskSchedule
{
public:
    virtual void ScheduleTask(std::wstring path_to_executable);

    // Future-proofing
    AdminTaskSchedule();
    virtual ~AdminTaskSchedule();
    AdminTaskSchedule(const AdminTaskSchedule&) = delete;
    AdminTaskSchedule& operator=(const AdminTaskSchedule&) = delete;
    AdminTaskSchedule(AdminTaskSchedule&&) = delete;
    AdminTaskSchedule& operator=(AdminTaskSchedule&&) = delete;
};
