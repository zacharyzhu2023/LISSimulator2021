#include <msclr\auto_gcroot.h>
#include "TCPCSWrapper.h"
#include <msclr\marshal_cppstd.h>

using namespace System::Runtime::InteropServices;
using namespace TCPcsFunctions;

class TCPCSWrapperPrivate
{
	public: msclr::auto_gcroot<TCPcsClass^> funcInstance;
};

TCPCSWrapper::TCPCSWrapper()
{
	_wrapper = new TCPCSWrapperPrivate();
	_wrapper->funcInstance = gcnew TCPcsClass();
}

TCPCSWrapper::~TCPCSWrapper()
{
	delete _wrapper;
}

