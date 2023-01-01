#include "AdminCheck.hpp"
#include <error_message_macro.hpp>
#include <Win32ErrorMessage.hpp>
#include <stdexcept>
#include <sstream>
#include <Windows.h>

// Create wrapper RAII class for AllocateAndInitializeSid and FreeSid
class Sid
{
private:
    PSID m_pSid = nullptr;
public:
    Sid(SID_IDENTIFIER_AUTHORITY pIdentifierAuthority, BYTE nSubAuthorityCount,
        DWORD nSubAuthority0, DWORD nSubAuthority1, DWORD nSubAuthority2, DWORD nSubAuthority3,
        DWORD nSubAuthority4, DWORD nSubAuthority5, DWORD nSubAuthority6, DWORD nSubAuthority7)
    {
        if (!AllocateAndInitializeSid(&pIdentifierAuthority, nSubAuthorityCount,
                                        nSubAuthority0, nSubAuthority1, nSubAuthority2, nSubAuthority3,
                                        nSubAuthority4, nSubAuthority5, nSubAuthority6, nSubAuthority7,
                                        &m_pSid))
        {
            DWORD dwError = GetLastError();
            std::stringstream ss;
            ss << "AllocateAndInitializeSid failed with error " << dwError;
            throw std::runtime_error{ ERROR_MESSAGE(ss.str()) };
        }
    }
    PSID getSidHandle()
    {
        if (m_pSid == nullptr)
        {
            throw std::runtime_error{ ERROR_MESSAGE("m_pSid is nullptr") };
        }
        return m_pSid;
    }
    PSID_IDENTIFIER_AUTHORITY getIdentifierAuthorityHandle()
    {
        PSID_IDENTIFIER_AUTHORITY identifierAuthority = GetSidIdentifierAuthority(getSidHandle());
        if (identifierAuthority == nullptr)
        {
            DWORD dwError = GetLastError();
            std::stringstream ss;
            ss << "GetSidIdentifierAuthority failed with error " << dwError;
            throw std::runtime_error{ ERROR_MESSAGE(ss.str()) };
        }
    }
    ~Sid()
    {
        FreeSid(m_pSid);
    }
};

AdminCheck::AdminCheck() = default;

bool AdminCheck::IsAdmin()
{
    Sid sid{ SECURITY_NT_AUTHORITY, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0 };
    BOOL bIsMember = FALSE;
    if (!CheckTokenMembership(NULL, sid.getSidHandle(), &bIsMember))
    {
        DWORD dwError = GetLastError();
        std::stringstream ss;
        ss << "CheckTokenMembership failed with error " << dwError;
        throw std::runtime_error{ ERROR_MESSAGE(ss.str()) };
    }
    return bIsMember;
}

AdminCheck::~AdminCheck() = default;
