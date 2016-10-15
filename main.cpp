#include "keyco.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")

HHOOK hHook;
KeyCo *k;

void UpdateKeySatate(BYTE *keystate, int keycode);
LRESULT CALLBACK LowLevelKeyBoaardProc(const int nCode, const WPARAM wParam, const LPARAM lParam);

int main(int argc, char *argv[])
{
  int result;
  QApplication a(argc, argv);
  k = new KeyCo();
  hHook = NULL;
  hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyBoaardProc, NULL, 0);

  if (hHook == NULL) {
    k->notify("KeyBoard hook falied!");
    delete k;
    return -1;
  } else
    k->notify("KeyCo start ok!");

  result = a.exec();
  delete k;
  return result;
}

void UpdateKeySatate(BYTE *keystate, int keycode)
{
  keystate[keycode] = (BYTE) GetKeyState(keycode);
}

LRESULT CALLBACK LowLevelKeyBoaardProc(const int nCode, const WPARAM wParam, const LPARAM lParam)
{
//  if (wParam != WM_KEYDOWN )//&& wParam != WM_SYSKEYDOWN)
  if (wParam != WM_KEYUP && wParam != WM_SYSKEYUP)
    return CallNextHookEx(hHook, nCode, wParam, lParam);

  // Get the key information
  KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);
  wchar_t buffer[5];

  // Get the keyboard stat
  BYTE keyboard_state[256];
  GetKeyboardState(keyboard_state);
  UpdateKeySatate(keyboard_state, VK_SHIFT);
  UpdateKeySatate(keyboard_state, VK_CAPITAL);
  UpdateKeySatate(keyboard_state, VK_CONTROL);
  UpdateKeySatate(keyboard_state, VK_MENU);

  // Get keyboard layout
  HKL keyboard_layout = GetKeyboardLayout(0);

  // Get the name
  char lpszName[0x100] = { 0 };

  DWORD dwMsg = 1;
  dwMsg += cKey.scanCode << 16;
  dwMsg += cKey.flags << 24;

  int i = GetKeyNameText(dwMsg, (LPTSTR)lpszName, 255);

  // Try convert the key info
  int result = ToUnicodeEx(cKey.vkCode, cKey.scanCode, keyboard_state, buffer, 4, 0, keyboard_layout);
  buffer[4] = L'\0';

  // Print the output
  //  cout << "Key: " << cKey.vkCode << " \"" << (cKey.vkCode != 13 ? (char*) buffer : "\\n") << "\" \"" << lpszName << "\"" << endl;
  QString name = (cKey.vkCode != 161) ? QString::fromUtf16((ushort *)lpszName) : "Shift Right";
  // qDebug() << "Key: " << cKey.vkCode << " " << QString::fromUtf16((ushort *)buffer) << " " << name;

  k->keyDown(cKey.vkCode, buffer);

  return CallNextHookEx(hHook, nCode, wParam, lParam);
  //return 0;
}