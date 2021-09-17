/*
MIT License

Copyright(c) Derek Goslin 2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef UTILITY_H
#define UTILITY_H

#include <Windows.h>
#include <string>

using namespace std;

/// <summary>
/// A utility class of helper methods
/// </summary>
class Utility final
{
private:
	Utility() = delete;
public:
	
	~Utility() = default;

	/// <summary>
	/// Gets the last win32 error as a string
	/// </summary>
	static string getLastErrorString(DWORD lastError);

	/// <summary>
	/// Converts a GUID to a string
	/// </summary>
	/// <param name="uuid">The guid to convert</param>
	static string guidToString(GUID uuid);

	/// <summary>
	/// Converts a wide string to a narrow string
	/// </summary>
	/// <param name="value">The string to convert</param>
	static string convertToString(const wstring& value);

	/// <summary>
	/// Handles memory allocation failure
	/// </summary>
	/// <param name="size">The size of bytes that failed to allocate</param>
	static void handleMallocFailure(unsigned long size);

	/// <summary>
	/// Throws a ble exception with a last error code details
	/// </summary>
	/// <param name="message">The exception message</param>
	static void throwLastErrorException(const string& message);

	/// <summary>
	/// Throws a ble exception
	/// </summary>
	/// <param name="message">The exception message</param>
	/// <param name="result">The HRESULT value</param>
	static void throwHResultException(const string& message, const HRESULT result);
};

#endif
