//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <string>
#include "WinApiTypes.hpp"

namespace WinApi
{

class Clipboard
{
public:
	class String
	{
		friend class Clipboard;
	public:
		/**
		* @throws LastErrorException
		* allocated global memoey and copied string to allocated memory
		*/
		explicit String(const std::string& p_value);
		/**
		* @throws LastErrorException
		* copies string from given global memory
		*/
		String(GlobalHandle p_handle);

		String(const String& p_other);

		operator std::string () const;
	private:
		GlobalHandle handle;
		std::string value;

		class NoAlloc {};
		// creates string; no global memory allocation
		String(const std::string& p_value, NoAlloc);
	};

	/**
	* @throws LastErrorException
	*/
	static void set(const String& p_value);

	/**
	* @throws LastErrorException
	*/
	static String getText();

private:
	class Lock;
};

}
