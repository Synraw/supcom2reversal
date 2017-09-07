/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once
#include <string>

namespace Moho
{
	/*
		String wrapper used by the engine when keeping small strings inline
		inside of game structures

		Can't find any related info to what its real name might be
	*/
	class MString
	{
	private:
		// Strings small enough to fit within the 16 char buffer will do so
		// Anything else uses part of the buffer as a pointer to the allocated text
		union
		{
			char* m_pText;
			char  m_chBuffer[16];
		};

		int m_szTextLength;
		int m_szBufferSize;

	public:

		inline std::string Get()
		{
			return m_szTextLength < 16 ? m_chBuffer : m_pText;
		}

		inline int Length() { return m_szTextLength; }
		inline int BufferSize() { return m_szBufferSize; }
	};
}

