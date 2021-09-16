/*
MIT License
Copyright(c) Derek Goslin 2019
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

#ifndef BLEEXCEPTION_H
#define BLEEXCEPTION_H

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

/// <summary>
/// An exception class throw when errors occur interacting with the ble device classes
/// </summary>
class BleException : public runtime_error
{
public:
	/// <summary>
	/// Construct an instance of a <see cref="BleException"/> class
	/// </summary>
	/// <param name="_Message">The exception message</param>
	explicit BleException(const string & _Message);
};

#endif
