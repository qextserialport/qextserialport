# Introduction #

Default value of WINVER

| | **WINVER** default value| **WINVER** meaning| **Defined in which file?** |
|:|:------------------------|:------------------|:---------------------------|
|MinGW from mingw.org |0x0400 |Windows 95 and Windows NT 4.0 | windef.h  |
|MinGW from mingw-w64|0x0502 |Windows Server 2003 with SP1, Windows XP with SP2| windef.h |
|Qt4 for MinGW | 0x0500 |Windows Server 2000| qt\_windows.h |
|Qt5.0 for MinGW |0x0501 |Windows Server 2003, Windows XP| qt\_windows.h |
|QextSerialPort 1.2 for MinGW |0x0501 |Windows Server 2003, Windows XP| qextserialenumerator\_p.h |

# Reference #

  * http://mingw-users.1079350.n2.nabble.com/DEV-BROADCAST-DEVICEINTERFACE-was-not-declared-in-this-scope-td3552762.html
  * http://msdn.microsoft.com/en-us/library/6sehtctf.aspx