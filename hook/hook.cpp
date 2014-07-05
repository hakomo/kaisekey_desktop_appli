#include<Windows.h>
#include<algorithm>

using namespace std;

#pragma data_seg(".share")
HHOOK  hh = 0;
#pragma data_seg()
#pragma comment(linker, "/Section:.share,rws")

HINSTANCE hi;
long long ct[256 << 4] = {};

int modifiers() {
    return (GetKeyState(VK_MENU) < 0) | (GetKeyState(VK_CONTROL) < 0 ? 2 : 0) |
        (GetKeyState(VK_SHIFT) < 0 ? 4 : 0) |
        (GetKeyState(VK_LWIN) < 0 || GetKeyState(VK_RWIN) < 0 ? 8 : 0);
}

LRESULT CALLBACK llkp(int cd, WPARAM wp, LPARAM lp) {
    if(cd < 0 || wp == WM_KEYDOWN || wp == WM_SYSKEYDOWN)
        return CallNextHookEx(hh, cd, wp, lp);
    ++ct[modifiers() * 256 + ((KBDLLHOOKSTRUCT*)lp)->vkCode];
    return CallNextHookEx(hh, cd, wp, lp);
}

extern "C" __declspec(dllexport) void hook() {
    hh = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)llkp, hi, 0);
}

extern "C" __declspec(dllexport) void set(long long* c) {
    copy_n(ct, 256 << 4, c);
    fill_n(ct, 256 << 4, 0);
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD, LPVOID) {
    hi = (HINSTANCE)hModule;
    return TRUE;
}
