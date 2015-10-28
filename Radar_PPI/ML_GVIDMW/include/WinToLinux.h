#ifndef _WINTOLINUX_
#define _WINTOLINUX_


#if (defined(_WIN32)) //windows
#define WIN_API  extern "C"__declspec(dllimport)
typedef  unsigned __int64 UINT64;
#elif defined(__linux__) || defined(__APPLE__) //linux
typedef     unsigned int    DWORD;
typedef     unsigned short  WORD;
typedef     unsigned short  USHORT;
typedef     int             LONG;
typedef     unsigned char	BYTE ;
#define     BOOL int
typedef     unsigned int   	UINT;
typedef     void* 	   	LPVOID;
typedef     void* 		HANDLE;
typedef     unsigned int*  LPDWORD; 
typedef     unsigned long long UINT64;

#ifndef    TRUE
#define    TRUE	1
#endif
#ifndef    FALSE
#define	   FALSE 0
#endif
#ifndef    NULL
#define	   NULL 0
#endif

#define __stdcall 
#define CALLBACK  

#define WIN_API extern "C"

#ifndef __HWND_defined
#define __HWND_defined
	#if defined(__linux__)
		typedef unsigned int HWND;
	#else
		typedef void* HWND;
	#endif
#endif

#define WIN_API extern "C"

#ifndef __HDC_defined
#define __HDC_defined
    #if defined(__linux__)
    typedef struct __DC
    {
            void*   surface;        //SDL Surface
            HWND    hWnd;           // HDC window handle
    }DC;
    typedef DC* HDC;
    #else
    typedef void* HDC;
    #endif
#endif
