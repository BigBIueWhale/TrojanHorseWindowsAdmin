#include "AdminStartupItem.hpp"

#include <error_message_macro.hpp>
#include <Win32ErrorMessage.hpp>
#include <Windows.h>
#include <shobjidl_core.h>
#include <filesystem>
#include <sstream>
#include <stdexcept>

// Create IShellLink wrapper RAII class to create shortcut files
class ShellLink
{
private:
    IShellLink* m_shell_link = nullptr;
public:
    ShellLink() {
        const HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_shell_link));
        if (hr != S_OK || m_shell_link == nullptr) {
            switch (hr) {
                case REGDB_E_CLASSNOTREG:
                    throw std::runtime_error{ ERROR_MESSAGE("CoCreateInstance failed with REGDB_E_CLASSNOTREG") };
                case CLASS_E_NOAGGREGATION:
                    throw std::runtime_error{ ERROR_MESSAGE("CoCreateInstance failed with CLASS_E_NOAGGREGATION") };
                case E_NOINTERFACE:
                    throw std::runtime_error{ ERROR_MESSAGE("CoCreateInstance failed with E_NOINTERFACE") };
                case E_POINTER:
                    throw std::runtime_error{ ERROR_MESSAGE("CoCreateInstance failed with E_POINTER") };
                case E_ACCESSDENIED:
                    throw std::runtime_error{ ERROR_MESSAGE("CoCreateInstance failed with E_ACCESSDENIED") };
                default:
                {
                    std::ostringstream err_msg;
                    err_msg << "CoCreateInstance failed with HRESULT: " << std::to_string(hr)
                        << "Did you remember to call CoInitializeEx?";
                    throw std::runtime_error{ ERROR_MESSAGE(err_msg.str()) };
                }
            }
        }
    }
    IShellLink* getShellLinkHandle() {
        if (m_shell_link == nullptr) {
            throw std::runtime_error{ ERROR_MESSAGE("m_shell_link is nullptr") };
        }
        return m_shell_link;
    }
    ~ShellLink() {
        if (m_shell_link) {
            static_cast<void>(m_shell_link->Release());
        }
    }
};

// Create a wrapper RAII class for IPersistFile
class PersistFile
{
private:
    IPersistFile* m_persist_file = nullptr;
public:
    PersistFile(IShellLink* shell_link) {
        const HRESULT hr = shell_link->QueryInterface(IID_PPV_ARGS(&m_persist_file));
        if (FAILED(hr)) {
            throw std::runtime_error{ ERROR_MESSAGE("IShellLinkW::QueryInterface failed") };
        }
    }
    IPersistFile* getPersistFileHandle() {
        if (m_persist_file == nullptr) {
            throw std::runtime_error{ ERROR_MESSAGE("m_persist_file is nullptr") };
        }
        return m_persist_file;
    }
    ~PersistFile() {
        if (m_persist_file) {
            static_cast<void>(m_persist_file->Release());
        }
    }
};

void AdminStartupItem::Create(std::wstring executable_path)
{
    // Create a shortcut file for the executable in:
    // C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup

    // Use the Win32API to create a shortcut of the executable

    const std::filesystem::path executable_path_fs{ executable_path };
    const std::filesystem::path startup_folder_path = L"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup";

    const std::filesystem::path shortcut_file_path = startup_folder_path / (executable_path_fs.stem().wstring() + L".lnk");

    // Create a shortcut file for the executable in: shortcut_file_path
    ShellLink shell_link{};
    const HRESULT hr = shell_link.getShellLinkHandle()->SetPath(executable_path.c_str());
    if (FAILED(hr)) {
        throw std::runtime_error{ ERROR_MESSAGE("IShellLinkW::SetPath failed") };
    }
    PersistFile persist_file{ shell_link.getShellLinkHandle() };
    const HRESULT hr2 = persist_file.getPersistFileHandle()->Save(shortcut_file_path.c_str(), TRUE);
    if (FAILED(hr2)) {
        throw std::runtime_error{ ERROR_MESSAGE("IPersistFile::Save failed") };
    }
}

AdminStartupItem::AdminStartupItem() = default;
AdminStartupItem::~AdminStartupItem() = default;
