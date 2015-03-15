When attaching your virtual serial device to a Windows machine, you need to specify a .inf file that describes the device and provides some info about it.  Usually, you plug the device in, the hardware manager asks for a driver and then you can point it to a directory containing this .inf file.

## Sample ##
Here's the general template that we've found to work well.  This should go in a .inf file named whatever you like, such as **my\_product.inf**.

There are a few parts you need to replace with information specific to your product:
  * **PROVIDER** should be your company name
  * **DESCRIPTION** should be your product name/description
  * lines in the form of **USB\VID\_EB03&PID\_0920** should specify your vendor and product IDs

If you have any modifications or updates, please file an issue.

```
; --------------------------------------------
; USB CDC ACM Setup File
; --------------------------------------------

[Version] 
Signature="$Windows NT$" 
Class=Ports
ClassGuid={4D36E978-E325-11CE-BFC1-08002BE10318} 
Provider=%PROVIDER% 
LayoutFile=layout.inf
DriverVer=11/10/2009,5.2.0.0

[Manufacturer] 
%DEVICE_DESCRIPTION%=DeviceList, NTx86, NTia64, NTamd64

[DeviceList.NTx86] 
%DEVICE_DESCRIPTION%=DriverInstall, USB\VID_EB03&PID_0920

[DeviceList.NTia64] 
%DEVICE_DESCRIPTION%=DriverInstall, USB\VID_EB03&PID_0920

[DeviceList.NTamd64] 
%DEVICE_DESCRIPTION%=DriverInstall, USB\VID_EB03&PID_0920

[DriverCopyFiles]
usbser.sys,,,0x20
serenum.sys,,,0x20

;------------------------------------------------------------------------------
;  Installation Configuration
;------------------------------------------------------------------------------

[DriverInstall]
include=mdmcpq.inf 
CopyFiles=DriverCopyFiles
AddReg=DriverInstall.AddReg 

[DriverInstall.HW] 
AddReg=DriverInstall.HW.AddReg 

[DriverInstall.AddReg]
HKR,,EnumPropPages32,,"MsPorts.dll,SerialPortPropPageProvider"

[DriverInstall.HW.AddReg]
HKR,,NTMPDriver,,*ntkern
HKR,,NTMPDriver,,usbser.sys
HKR,,PortSubClass,1,01
HKR,,"UpperFilters",0x00010000,"serenum" 

[DriverInstall.Services] 
AddService=usbser, 0x00000002, UsbserService
AddService=Serenum,,SerenumService

;------------------------------------------------------------------------------
;  Usbser Service
;------------------------------------------------------------------------------

[UsbserService] 
DisplayName=%USBSER.DESC% 
ServiceType=1
StartType=3
ErrorControl=1
ServiceBinary=%12%\usbser.sys 
LoadOrderGroup = Base

;------------------------------------------------------------------------------
;  Serenum Service
;------------------------------------------------------------------------------

[SerenumService] 
DisplayName    = %SERENUM.DESC%
ServiceType    = 1               ; SERVICE_KERNEL_DRIVER
StartType      = 3               ; SERVICE_DEMAND_START
ErrorControl   = 1               ; SERVICE_ERROR_NORMAL
ServiceBinary  = %12%\serenum.sys
LoadOrderGroup = PNP Filter

;------------------------------------------------------------------------------
;  String Definitions
;------------------------------------------------------------------------------

[Strings] 
PROVIDER="My Organization"
DEVICE_DESCRIPTION ="My Friendly Device Name"
USBSER.DESC = "USB Serial Driver"
SERENUM.DESC = "Serenum Filter Driver"

```