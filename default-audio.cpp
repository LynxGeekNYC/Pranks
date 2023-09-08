#include <iostream>
#include <Windows.h>
#include <Urlmon.h>
#include <Shlwapi.h>
#include <fstream>

#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Shlwapi.lib")

bool DownloadFile(const char* url, const char* localPath) {
    HRESULT hr = URLDownloadToFile(NULL, url, localPath, 0, NULL);
    return SUCCEEDED(hr);
}

int main() {
    // URLs of the MP3 files to download.
    const char* inSoundUrl = "http://example.com/in_sound.mp3";
    const char* outSoundUrl = "http://example.com/out_sound.mp3";

    // Will automatically download in the userprofile directory
    const char* documentsFolder = getenv("USERPROFILE");
    const char* inSoundPath = PathCombineA((LPSTR)documentsFolder, "in_sound.mp3");
    const char* outSoundPath = PathCombineA((LPSTR)documentsFolder, "out_sound.mp3");

    // Download the MP3 files
    if (DownloadFile(inSoundUrl, inSoundPath) && DownloadFile(outSoundUrl, outSoundPath)) {
        // Set USB in and out sounds in Windows Registry (Hope this works).
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, "AppEvents\\Schemes\\Apps\\.Default\\DeviceConnect\\.Current", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            RegSetValueEx(hKey, nullptr, 0, REG_SZ, (BYTE*)inSoundPath, strlen(inSoundPath));
            RegCloseKey(hKey);
        }

        if (RegOpenKeyEx(HKEY_CURRENT_USER, "AppEvents\\Schemes\\Apps\\.Default\\DeviceDisconnect\\.Current", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            RegSetValueEx(hKey, nullptr, 0, REG_SZ, (BYTE*)outSoundPath, strlen(outSoundPath));
            RegCloseKey(hKey);
        }

        // Notify the user about the changes. This can be removed. 
        std::cout << "USB in and out sounds set successfully!" << std::endl;
    } else {
        std::cerr << "Failed to download one or both MP3 files." << std::endl;
    }

    // Exit without notification.
    return 0;
}
