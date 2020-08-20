#include <msclr\auto_gcroot.h>
#include "UnmanagedWrapper.h"
#include <msclr\marshal_cppstd.h>

using namespace System::Runtime::InteropServices;
using namespace TCPcsLibrary;

class UnmanagedWrapperPrivate
{
public:
	msclr::auto_gcroot<csLib^> _csLib;
};

/* Utility methods for string conversion */
static const std::string toCPPString(System::String^ inp)
{
	std::string output = msclr::interop::marshal_as<std::string>(inp);
	return output;
}

static System::String^ toCSString(std::string inp)
{
	System::String^ output = gcnew System::String(inp.c_str());
	return output;
}

UnmanagedWrapper::UnmanagedWrapper()
{
	_private = new UnmanagedWrapperPrivate();
	_private->_csLib = gcnew csLib();
}

UnmanagedWrapper::UnmanagedWrapper(int a, int b, int c)
{
	_private = new UnmanagedWrapperPrivate();
	_private->_csLib = gcnew csLib(a, b, c);
}

UnmanagedWrapper::~UnmanagedWrapper()
{
	delete _private;
}

int UnmanagedWrapper::add(int a, int b)
{
	return _private->_csLib->add(a, b);
}

int UnmanagedWrapper::subtract(int a, int b)
{
	return _private->_csLib->subtract(a, b);
}

void UnmanagedWrapper::printMsg()
{
	_private->_csLib->printMsg();
}

void UnmanagedWrapper::printValues()
{
	_private->_csLib->printValues();
}

std::string UnmanagedWrapper::doubleMessage(const char* msg)
{
	System::String^ result = _private->_csLib->doubleMessage(gcnew System::String(msg));
	return toCPPString(result);
}

