REM This is a basic file to test the execution of a server w/ multiple clients simultaneously.
REM Currently supporting 16 clients
start %~dp0\TCPServerV5.exe -multiwindow
start %~dp0\TCPClientV5.exe
start %~dp0\TCPClientV5.exe
start %~dp0\TCPClientV5.exe
start %~dp0\TCPClientV5.exe
start %~dp0\TCPClientV5.exe