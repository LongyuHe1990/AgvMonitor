#pragma once
#include <sstream>
#include <chrono>
#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib,"DbgHelp.lib")
#define DUMP_FOLDER "../dump/"


namespace utility
{

    class ExceptionHandler
    {
    public:
        static void init()
        {
            SetUnhandledExceptionFilter(&ExceptionHandler::exceptionCallBack);
            PreventSetUnhandledExceptionFilter();
        }

    private:
        static std::string GetDumpFileName()
        {
            std::stringstream fn;
            std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            fn << DUMP_FOLDER << "SysCrash_" << std::put_time(std::localtime(&tt), "%Y-%m-%d-%H.%M.%S") << ".dmp";
            return fn.str();
        }

        static bool createDumpFile(std::string filePath, PEXCEPTION_POINTERS pExcept)
        {
            MINIDUMP_EXCEPTION_INFORMATION exceptInfo;
            exceptInfo.ExceptionPointers = pExcept;
            exceptInfo.ThreadId = GetCurrentThreadId();
            exceptInfo.ClientPointers = TRUE;

            HANDLE dumpFile = ::CreateFile((LPCSTR)filePath.c_str(), GENERIC_WRITE, 0, NULL,
                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

            if (dumpFile != INVALID_HANDLE_VALUE)
            {
                MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, MiniDumpNormal, &exceptInfo, NULL, NULL);
                CloseHandle(dumpFile);
                return true;
            }
            return false;
        }

        static long _stdcall exceptionCallBack(PEXCEPTION_POINTERS excp)
        {
            broker::Logger log_module("UnhandledException");
            LOGGER_FATAL((&log_module)) << "Unexcepted Exception";
            auto dump_file_name = GetDumpFileName();
            if (createDumpFile(dump_file_name, excp))
            {
                LOGGER_INFO((&log_module)) << "create dump file sucess," << dump_file_name;
            }
            else
            {
                LOGGER_INFO((&log_module)) << "create dump file failed," << dump_file_name;
            }
            return   EXCEPTION_EXECUTE_HANDLER;
        }


        static LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
        {
            return NULL;
        }

        static BOOL PreventSetUnhandledExceptionFilter()
        {
            HMODULE hKernel32 = LoadLibrary("kernel32.dll");
            if (hKernel32 == NULL) return FALSE;
            void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
            if (pOrgEntry == NULL) return FALSE;
            unsigned char newJump[100];
            DWORD dwOrgEntryAddr = (DWORD)pOrgEntry;
            dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far
            void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
            DWORD dwNewEntryAddr = (DWORD)pNewFunc;
            DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;

            newJump[0] = 0xE9;  // JMP absolute
            memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));
            SIZE_T bytesWritten;
            BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
                pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
            return bRet;
        }     
    };
}