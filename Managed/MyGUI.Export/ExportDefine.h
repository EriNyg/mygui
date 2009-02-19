/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __EXPORT_DEFINE_H__
#define __EXPORT_DEFINE_H__

#define MYGUIEXPORT extern "C" __declspec( dllexport ) 
#define MYGUICALL __cdecl 
#define MYGUICALLBACK __stdcall

namespace Export
{
	typedef int* IUnknown;
}

#endif // __EXPORT_DEFINE_H__