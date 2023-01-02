#include "CombaseLibrary.hpp"

#include <stdexcept>
#include <error_message_macro.hpp>

#include <Windows.h>
#include <combaseapi.h>

CombaseLibrary::CombaseLibrary()
{
    const HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        throw std::runtime_error{ ERROR_MESSAGE("CoInitializeEx failed") };
    }
}

CombaseLibrary::~CombaseLibrary()
{
    CoUninitialize();
}
