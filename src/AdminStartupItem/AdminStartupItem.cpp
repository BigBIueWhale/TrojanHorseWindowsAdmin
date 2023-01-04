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
    // Schedule a task to run on login of any user.
    // The task will run with the highest privilages.
    // The task will run whether the user is logged-in or not (will not store credentials).
    // Configure the task for Windows 10
    // Make the task hidden

    // Create the task
    ITaskService* pService = nullptr;
    ITaskFolder* pRootFolder = nullptr;
    ITaskDefinition* pTask = nullptr;
    IRegistrationInfo* pRegInfo = nullptr;
    ITaskSettings* pSettings = nullptr;
    IPrincipal* pPrincipal = nullptr;
    IActionCollection* pActionCollection = nullptr;
    IAction* pAction = nullptr;
    IExecAction* pExecAction = nullptr;
    ITriggerCollection* pTriggerCollection = nullptr;
    ITrigger* pTrigger = nullptr;
    ILogonTrigger* pLogonTrigger = nullptr;
    IRegisteredTask* pRegisteredTask = nullptr;
    HRESULT hr = S_OK;

    // Assuming the the COMbase library has been initialized

    // Get the task service
    hr = CoCreateInstance(CLSID_TaskScheduler, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskService, reinterpret_cast<void**>(&pService));
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to create the task service. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Connect to the task service
    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to connect to the task service. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Get the root folder
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to get the root folder. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Get the task definition
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to get the task definition. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Get the registration info
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to get the registration info. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Set the registration info
    hr = pRegInfo->put_Author(_bstr_t(L""));
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to set the registration info. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Get the task settings
    hr = pTask->get_Settings(&pSettings);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to get the task settings. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Set the task settings
    hr = pSettings->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to set the task settings. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // TODO:
    // // Hide task
    // hr = pSettings->put_Hidden(VARIANT_TRUE);

    // Get the principal
    hr = pTask->get_Principal(&pPrincipal);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to get the principal. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Set IPrincipal::put_GroupId to administrators
    // This will allow the task to run with the highest privileges
    // The special string is a well-known SID used for the administrators group
    hr = pPrincipal->put_GroupId(_bstr_t(L"S-1-5-32-544"));

    // Set the principal
    hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "pPrincipal->put_RunLevel failed. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Don't store credentials
    hr = pPrincipal->put_LogonType(TASK_LOGON_NONE);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "pPrincipal->put_LogonType failed. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }


    // Get the action collection
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to get the action collection. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Create the action
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to create the action. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Get the exec action
    hr = pAction->QueryInterface(IID_IExecAction, reinterpret_cast<void**>(&pExecAction));
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to get the exec action. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Set the exec action to run executable_path
    hr = pExecAction->put_Path(_bstr_t(executable_path.c_str()));
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to set the exec action. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Get the trigger collection
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to get the trigger collection. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Set the trigger to run at login of any user
    hr = pTriggerCollection->Create(TASK_TRIGGER_LOGON, &pTrigger);
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to set the trigger. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Get the logon trigger
    hr = pTrigger->QueryInterface(IID_ILogonTrigger, reinterpret_cast<void**>(&pLogonTrigger));
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to get the logon trigger. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Set the logon trigger to run at login of any user
    hr = pLogonTrigger->put_Id(_bstr_t(L"Trigger1"));
    if (FAILED(hr))
    {
        std::ostringstream oss;
        oss << "Failed to set the logon trigger. Error code: " << hr;
        throw std::runtime_error{ ERROR_MESSAGE(oss.str()) };
    }

    // Release the task
    pTask->Release();
    pTask = nullptr;

    // Release the service
    pService->Release();
    pService = nullptr;
}

AdminStartupItem::AdminStartupItem() = default;
AdminStartupItem::~AdminStartupItem() = default;
