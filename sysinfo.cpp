#include <iostream>
#include <windows.h>
#include <iphlpapi.h>
#include <psapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")


#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "ws2_32.lib")


void get_os_info() {
    OSVERSIONINFOEX osvi = { sizeof(OSVERSIONINFOEX) };
    GetVersionEx((LPOSVERSIONINFO)&osvi);
    std::cout << "[+] OS Version: Windows " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << "\n";
}

void get_cpu_info() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::cout << "\n==== CPU INFO ====\n";
    std::cout << "[+] Number of Processors: " << sysInfo.dwNumberOfProcessors << "\n";
    std::cout << "    Architecture: " << sysInfo.wProcessorArchitecture << "\n";
}

void get_ram_info() {
    MEMORYSTATUSEX statex = { sizeof(statex) };
    GlobalMemoryStatusEx(&statex);
    std::cout << "\n[+] Total RAM: " << statex.ullTotalPhys / (1024 * 1024) << " MB\n";
}

void get_disk_info() {
    ULARGE_INTEGER total, free, avail;
    std::cout << "\n==== DISK INFO ====\n";
    char drives[256];
    GetLogicalDriveStringsA(255, drives);
    for (char* drive = drives; *drive; drive += strlen(drive) + 1) {
        if (GetDiskFreeSpaceExA(drive, &avail, &total, &free)) {
            std::cout << "[+] Drive: " << drive << "\n";
            std::cout << "    Total: " << total.QuadPart / (1024 * 1024 * 1024) << " GB\n";
            std::cout << "    Free: " << free.QuadPart / (1024 * 1024 * 1024) << " GB\n";
        }
    }
}

void get_network_info() {
    std::cout << "\n==== NETWORK INFO ====\n";

    IP_ADAPTER_INFO adapter[16];
    DWORD buflen = sizeof(adapter);

    if (GetAdaptersInfo(adapter, &buflen) == NO_ERROR) {
        PIP_ADAPTER_INFO pAdapter = adapter;
        while (pAdapter) {
            std::cout << "[+] Adapter: " << pAdapter->Description << "\n";
            std::cout << "    IP Address: " << pAdapter->IpAddressList.IpAddress.String << "\n";
            std::cout << "    MAC Address: ";
            for (UINT i = 0; i < pAdapter->AddressLength; i++) {
                printf("%02X%s", pAdapter->Address[i], (i < pAdapter->AddressLength - 1) ? "-" : "");
            }
            std::cout << "\n\n";
            pAdapter = pAdapter->Next;
        }
    }

    // Public IP
    HINTERNET net = InternetOpen("PublicIP", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (net) {
        HINTERNET conn = InternetOpenUrlA(net, "http://api.ipify.org", NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (conn) {
            char buffer[64];
            DWORD bytesRead;
            if (InternetReadFile(conn, buffer, sizeof(buffer) - 1, &bytesRead)) {
                buffer[bytesRead] = '\0';
                std::cout << "[+] Public IP: " << buffer << "\n";
            }
            InternetCloseHandle(conn);
        } else {
            std::cout << "[!] Could not open IP URL\n";
    }
    InternetCloseHandle(net);
} else {
    std::cout << "[!] InternetOpen failed\n";
}

}

void get_display_info() {
    std::cout << "\n==== DISPLAY INFO ====\n";
    std::cout << "[+] Screen Width: " << GetSystemMetrics(SM_CXSCREEN) << "\n";
    std::cout << "    Screen Height: " << GetSystemMetrics(SM_CYSCREEN) << "\n";
}

int main() {
    std::cout << "==== SYSTEM INFORMATION TOOL (MinGW Compatible) ====\n";

    get_os_info();
    get_cpu_info();
    get_ram_info();
    get_disk_info();
    get_network_info();
    get_display_info();

    return 0;
}
