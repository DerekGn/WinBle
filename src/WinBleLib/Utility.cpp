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

#include "Utility.h"
#include "BleException.h"

#include <Windows.h>
#include <codecvt>
#include <sstream>

string Utility::getLastErrorString(DWORD lastError)
{

	if (lastError == 0)
		return string();

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);

	string message(messageBuffer, size);

	LocalFree(messageBuffer);

	return message.replace(message.end() - 3, message.end(), "");
}

string Utility::guidToString(GUID uuid)
{
	string guid;
	RPC_CSTR szUuid = nullptr;
	if (::UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
	{
		guid = (char*)szUuid;
		::RpcStringFreeA(&szUuid);
	}

	return guid;
}

string Utility::convertToString(const wstring& value)
{
	using convert_type = codecvt_utf8<wchar_t>;
	wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(value);
}

void Utility::handleMallocFailure(unsigned long size)
{
	stringstream msg;
	msg << "Unable to allocate [" << size << "] bytes";

	throw BleException(msg.str());
}

void Utility::throwLastErrorException(const string& message)
{
	stringstream stream;
	DWORD lasterror = GetLastError();
	stream << message <<
		" LastError: " << std::hex << lasterror <<
		" Reason: [" << getLastErrorString(lasterror) << "]";

	throw BleException(stream.str());
}

void Utility::throwHResultException(const string& message, const HRESULT result)
{
	stringstream stream;
	stream << message <<
		" HRESULT: " << std::hex << result <<
		" Reason: [" << getLastErrorString(result) << "]";

	throw BleException(stream.str());
}
