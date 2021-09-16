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

#pragma once

#include "CallbackContext.h"

// CallbackScope sets invokes CallbackContext.SetInCallback in its constructor
// and invokes CallbackContext.UnsetInCallback in its destructor

class CallbackScope
{
public:
	CallbackScope(CallbackScope&&) = delete;
	CallbackScope(const CallbackScope&) = delete;
	CallbackScope& operator=(const CallbackScope&) = delete;
	CallbackScope& operator=(CallbackScope&&) = delete;

	explicit CallbackScope(CallbackContext& context) : _context(context)
	{
		_context.SetInCallback();
	}

	~CallbackScope()
	{
		_context.UnsetInCallback();
	}

private:
	CallbackContext& _context;
};
