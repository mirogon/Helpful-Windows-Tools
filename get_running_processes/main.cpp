//Core
#include <iostream>
#include <string>
#include <stdlib.h>

//Windows
#include <windows.h>
#include <TlHelp32.h>

void print_running_processes() 
{
	HANDLE process_snap;
	PROCESSENTRY32 process_entry;
	process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (process_snap != INVALID_HANDLE_VALUE)
	{
		process_entry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(process_snap, &process_entry)) 
		{ 
			while (Process32Next(process_snap, &process_entry)) 
			{
				std::cout << "PID:" << process_entry.th32ProcessID << " NAME: " << process_entry.szExeFile << std::endl;
			}
			CloseHandle(process_snap);
		}
	}
	else
	{
		std::cout << "Process snapshot could not be made successfully" << std::endl;
		return;
	}
}

int main(int argc, char* argv[])
{
	print_running_processes();

	system("pause");
	return 0;
}