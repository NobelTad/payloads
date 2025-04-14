#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <direct.h>

std::string get_timestamp_filename() {
    time_t now = time(0);
    tm* local = localtime(&now);

    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", local);
    return std::string(buffer) + ".bmp"; // BMP is easier to save in native C++
}

bool capture_screen(const std::string& full_path) {
    // Get screen dimensions
    int screen_x = GetSystemMetrics(SM_CXSCREEN);
    int screen_y = GetSystemMetrics(SM_CYSCREEN);

    // Create device contexts
    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screen_x, screen_y);
    SelectObject(hDC, hBitmap);

    // Copy screen to bitmap
    BitBlt(hDC, 0, 0, screen_x, screen_y, hScreen, 0, 0, SRCCOPY);

    // Save to file
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = screen_x;
    bi.biHeight = -screen_y; // Negative so origin is top-left
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((screen_x * bi.biBitCount + 31) / 32) * 4 * screen_y;
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char* lpbitmap = (char*)GlobalLock(hDIB);

    GetDIBits(hDC, hBitmap, 0, (UINT)screen_y, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    std::ofstream out(full_path, std::ios::out | std::ios::binary);
    bmfHeader.bfType = 0x4D42;
    bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmpSize;
    bmfHeader.bfReserved1 = 0;
    bmfHeader.bfReserved2 = 0;
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    out.write((char*)&bmfHeader, sizeof(bmfHeader));
    out.write((char*)&bi, sizeof(bi));
    out.write(lpbitmap, dwBmpSize);
    out.close();

    // Cleanup
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);
    DeleteObject(hBitmap);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);

    return true;
}

int main() {
    _mkdir("imgdat");  // Create folder

    std::string filename = get_timestamp_filename();
    std::string fullpath = "imgdat\\" + filename;

    if (capture_screen(fullpath)) {
        std::cout << fullpath << std::endl;
    } else {
        std::cerr << "Screen capture failed." << std::endl;
    }

    return 0;
}
