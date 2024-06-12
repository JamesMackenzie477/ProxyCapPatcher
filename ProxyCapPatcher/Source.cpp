#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>

#define PROXYCAP_SIR_EVENT L"Global\\PROXYCAP_SIR_EVENT"
#define PROXYCAP_LIO_EVENT L"Global\\PROXYCAP_LIO_EVENT"
#define PROXYCAP_IE_EVENT L"Global\\PROXYCAP_IE_EVENT"
#define PROXYCAP_UIR_EVENT L"Global\\PROXYCAP_UIR_EVENT"

class ProxyCap
{
private:
	HANDLE hSIREvent;
	HANDLE hLIOEvent;
	HANDLE hIEEvent;
	HANDLE hUIREvent;
public:
	ProxyCap()
	{
		// Creates the ProxyCap SIR event (tells the client that the service is running).
		hSIREvent = CreateEventExW(nullptr, PROXYCAP_SIR_EVENT, NULL, SYNCHRONIZE);
		// Creates the ProxyCap LIO event (this event is only present in a licensed version).
		hLIOEvent = CreateEventExW(nullptr, PROXYCAP_LIO_EVENT, NULL, SYNCHRONIZE);
		// Creates the ProxyCap UIR event (makes proxy cap think it's registered).
		hUIREvent = CreateEventExW(nullptr, PROXYCAP_UIR_EVENT, NULL, SYNCHRONIZE);
	}

	~ProxyCap()
	{
		// Closes the events.
		CloseHandle(hSIREvent);
		CloseHandle(hLIOEvent);
		CloseHandle(hUIREvent);
	}

	// Enables the proxy cap service.
	bool Enable()
	{
		// Creates the ProxyCap IE event (enables the service).
		if (hIEEvent = CreateEventExW(nullptr, PROXYCAP_IE_EVENT, NULL, SYNCHRONIZE))
			return true;
		else
			return false;
	}

	// Disables the proxy cap service.
	bool Disable()
	{
		// Closes the ProxyCap IE event (disables the service).
		return CloseHandle(hIEEvent);
	}
};

LSTATUS OpenRegistry(REGSAM samDesired, bool LocalMachine, PHKEY phkResult)
{
	if (LocalMachine)
	{
		std::cout << std::hex << (samDesired | 0x200) << std::endl;
		return RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Proxy Labs\\ProxyCap", 0, NULL, REG_OPTION_NON_VOLATILE, samDesired | KEY_WOW64_32KEY, NULL, phkResult, NULL);
	}
	else
	{
		return RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Proxy Labs\\ProxyCap", 0, NULL, REG_OPTION_NON_VOLATILE, samDesired, NULL, phkResult, NULL);
	}
}

HKEY sub_14004AEB0(REGSAM samDesired)
{
	REGSAM v1; // ebx@1
	HKEY hKey; // [sp+68h] [bp+10h]@1
	HKEY v4; // [sp+70h] [bp+18h]@1

	v1 = samDesired;
	hKey = 0i64;
	OpenRegistry(samDesired, 1, &hKey);
	v4 = 0i64;
	RegCreateKeyExW(hKey, L"Registration", 0, 0i64, 0, v1, 0i64, &v4, 0i64);
	RegCloseKey(hKey);
	return v4;
}

int main()
{
	// Creates the proxy cap class.
	/*ProxyCap Proxy;
	// Enables proxy cap.
	if (Proxy.Enable())
	{
		// Notifies the user.
		std::cout << "ProxyCap enabled." << std::endl;
	}*/
	std::cout << std::hex << sub_14004AEB0(KEY_QUERY_VALUE) << std::endl;

	// Waits to exit.
	system("PAUSE");
}