REM This is a basic file to test the execution of a server w/ multiple clients simultaneously.
REM Should support up to 16 clients
start %~dp0\netcoreapp3.1\CSharpPipeInstance.exe -multiwindow
start %~dp0\TCPServer.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
start %~dp0\TCPClient.exe
