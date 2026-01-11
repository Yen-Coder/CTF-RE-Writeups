#OST2
# Courses
Big thanks to `OST2`, a brilliant source to learn assemblies / reverse engineering / exploit dev / vulnerability hunting, etc:
- Open-source Online course `OST2`: https://apps.p.ost2.fyi/learning/course/course-v1:OpenSecurityTraining2+Arch1001_x86-64_Asm+2021_v1/home

# Setup for Windows10 VM:
- Check the course materials for links

# Setup for Visual Studio 2019:
- https://apps.p.ost2.fyi/learning/course/course-v1:OpenSecurityTraining2+Dbg1001_VS_IDE+2021_v1/block-v1:OpenSecurityTraining2+Dbg1001_VS_IDE+2021_v1+type@sequential+block@eb40da8217af45f6aa84d09eeb4d5954/

When testing VS2019 by building a `fibber.c`, I noticed there are several `.dll` files linked

![[Screenshot 2026-01-11 195152.png]]
Windows loads those DLLs automatically when launching any native x64 executable, including your compiled C program, as part of the standard PE (Portable Executable) loading process in user-mode processes ([Microsoft Doc about DLL](https://learn.microsoft.com/en-us/troubleshoot/windows-client/setup-upgrade-and-drivers/dynamic-link-library)).

## Core System DLLs
#dll
> Documentation about DLL: ([learn.microsoft / DLL](https://learn.microsoft.com/en-us/troubleshoot/windows-client/setup-upgrade-and-drivers/dynamic-link-library)).​

Windows loads several **core** DLLs into every normal process; the ones in the screenshot are all standard system libraries, not specific to the code.​[](https://learn.microsoft.com/en-us/troubleshoot/windows-client/setup-upgrade-and-drivers/dynamic-link-library)​

## ntdll.dll

- Implements the NT native API and low-level system call stubs used by kernel-mode services.​
- Handles process/thread startup, exceptions, and interaction between user mode and the Windows kernel.​

## kernel32.dll and KernelBase.dll

- `kernel32.dll` exposes high-level `Win32` APIs for processes, threads, memory, files, and synchronisation.
- `KernelBase.dll` holds many of those implementations; newer Windows versions forward a lot of `kernel32` exports into `KernelBase` for internal layering.​


## apphelp.dll

- Provides application compatibility and shim infrastructure so Windows can apply “compatibility fixes” for older programs.​
- Loaded early so the OS can intercept certain API calls and adjust behaviour without changing the application binary.​

## vcruntime140d.dll

- The debug build of the Visual C++ runtime library (VC++ 2015+).​
- Contains C/C++ runtime support such as exception handling, new/delete, and debugging helpers; linked automatically by MSVC for /MDd builds.

## ucrtbase.dll

- The Universal C Runtime shared by modern MSVC toolchains.​
- Implements standard C library functions (printf, malloc, memcpy, etc.) that your C code and the CRT startup use.​
## kernel.appcore.dll

- Provides APIs for application lifecycle and process model used by modern Windows apps.​
- Pulled in as a dependency of higher-level libraries even if your program does not use those APIs directly
## msvcrt.dll

- Older Microsoft C runtime library still present for backward compatibility​
- Many system components and some toolchains continue to depend on it, so it is commonly mapped into processes.​

## rpcrt4.dll

- Implements Microsoft’s RPC (Remote Procedure Call) runtime for client/server and COM/DCOM communication.​
- Loaded because various Win32 services and subsystems your process talks to internally use RPC, even for a simple console app.​

In a Visual Studio debug run, the loader brings these in automatically as part of satisfying the import table and transitive dependencies; their presence is normal for a compiled x64 C program on Windows.​

`x64dbg` displays these during dynamic analysis because it attaches to the process early, capturing the full loader chain before your main() executes [Medium Blog - [Reverse Engineering Tips] — Analyzing a DLL in x64DBG](https://blog.securitybreak.io/reverse-engineering-tip-analyzing-a-dll-in-x64dbg-b3005d516049)


---
#RE-tools
# PE tools
- PEBear​  