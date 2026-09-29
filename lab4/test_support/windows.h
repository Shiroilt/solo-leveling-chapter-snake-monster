#ifndef LAB4_TEST_WINDOWS_H
#define LAB4_TEST_WINDOWS_H
using HANDLE = int;
using COORD = struct { short X; short Y; };
#define STD_OUTPUT_HANDLE 0
#define CP_UTF8 65001
#define MB_OK 0
#define MB_ICONASTERISK 0
#define MB_ICONHAND 0
inline HANDLE GetStdHandle(int) { return 0; }
inline int SetConsoleCursorPosition(HANDLE, COORD) { return 1; }
inline int SetConsoleOutputCP(unsigned int) { return 1; }
inline int SetConsoleCP(unsigned int) { return 1; }
inline int MessageBeep(unsigned int) { return 1; }
inline void Sleep(unsigned long) {}
#endif
