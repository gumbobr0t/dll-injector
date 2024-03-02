#include <Windows.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    DWORD proc_id = std::stoul(argv[1]);

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, proc_id);

    if (!process) {
        std::cout << "Failed to open proc\n";
    }

    const char dll_path[MAX_PATH] = "C:\\Users\\gumbo\\Desktop\\dll\\python_code_injection.dll";

    void* memory = VirtualAllocEx(process, 0, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    
    WriteProcessMemory(process, memory, dll_path, strlen(dll_path) + 1, 0);

    HANDLE thread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, memory, 0, 0);

    CloseHandle(process);

    VirtualFreeEx(process, memory, strlen(dll_path) + 1, MEM_RELEASE);

    std::system_category().message(GetLastError());

    return 0;
}