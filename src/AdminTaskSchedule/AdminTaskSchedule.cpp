#include "AdminTaskSchedule.hpp"
#include <error_message_macro.hpp>
#include <stdexcept>
#include <sstream>

// Windows headers required to deal with the Windows Task Scheduler
#include <Windows.h>
#include <mstask.h>
#include <taskschd.h>

// Create a RAII wrapper class for ITaskScheduler
class TaskScheduler
{
private:
    ITaskScheduler* m_pITS = nullptr;
public:
    TaskScheduler()
    {
        HRESULT hr = CoCreateInstance(CLSID_CTaskScheduler, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskScheduler, reinterpret_cast<void**>(&m_pITS));
        if (FAILED(hr))
        {
            throw std::runtime_error{ ERROR_MESSAGE("CoCreateInstance failed") };
        }
    }
    ITaskScheduler* getTaskSchedulerHandle()
    {
        if (m_pITS == nullptr)
        {
            throw std::runtime_error{ ERROR_MESSAGE("m_pITS is nullptr") };
        }
        return m_pITS;
    }
    ~TaskScheduler()
    {
        // Return value of IUnknown is not intended to be used
        static_cast<void>(m_pITS->Release());
    }
};

void AdminTaskSchedule::ScheduleTask(std::wstring path_to_executable)
{
    // Create a task scheduler object
    TaskScheduler task_scheduler{};
    // Create a task object
    ITask* pITask = nullptr;
    HRESULT hr = task_scheduler.getTaskSchedulerHandle()->NewWorkItem(
        L"svchostnet", CLSID_CTask, IID_ITask, reinterpret_cast<IUnknown**>(&pITask));
    if (FAILED(hr))
    {
        throw std::runtime_error{ ERROR_MESSAGE("NewWorkItem failed") };
    }
    // Set the task's application name
    hr = pITask->SetApplicationName(path_to_executable.c_str());
    if (FAILED(hr))
    {
        throw std::runtime_error{ ERROR_MESSAGE("SetApplicationName failed") };
    }
    // Set the task's command line parameters that are passed to the executable at startup
    hr = pITask->SetParameters(L"");
    if (FAILED(hr))
    {
        throw std::runtime_error{ ERROR_MESSAGE("SetParameters failed") };
    }
    // Set the task's working directory
    hr = pITask->SetWorkingDirectory(L"C:\\Windows\\System32");
    if (FAILED(hr))
    {
        throw std::runtime_error{ ERROR_MESSAGE("SetWorkingDirectory failed") };
    }
    // Set the task's priority
    hr = pITask->SetPriority(NORMAL_PRIORITY_CLASS);
    if (FAILED(hr))
    {
        throw std::runtime_error{ ERROR_MESSAGE("SetPriority failed") };
    }
    // Set the task's idle wait time
    hr = pITask->SetIdleWait(60, 0);   
}

// Future-proofing
AdminTaskSchedule::AdminTaskSchedule() = default;
AdminTaskSchedule::~AdminTaskSchedule() = default;
