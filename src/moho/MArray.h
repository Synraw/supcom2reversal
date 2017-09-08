/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

namespace Moho
{
	/*
		These kinds of arrays seem to be used all over the engine
		Uses 2 pointers(one to the start and one to the end)
	*/
	template<class T> class MArray
	{
	private:
		T* m_pArrayStart;
		T* m_pArrayEnd;

	public:
		T* begin()
		{
			return m_pArrayStart;
		}

		T* end()
		{
			return m_pArrayEnd;
		}

		size_t size()
		{
			return (reinterpret_cast<unsigned int>(m_pArrayEnd) - reinterpret_cast<unsigned int>(m_pArrayStart)) / sizeof(T);
		}

		T& operator[](int index)
		{
			return m_pArrayStart[index];
		}
	};
}