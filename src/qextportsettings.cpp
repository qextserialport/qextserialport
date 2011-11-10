/****************************************************************************
** Copyright (c) 2000-2007 Stefan Sander
** Copyright (c) 2007 Michal Policht
** Copyright (c) 2008 Brandon Fosdick
** Copyright (c) 2009-2010 Liam Staskawicz
** Copyright (c) 2011 Debao Zhang
** All right reserved.
** Web: http://code.google.com/p/qextserialport/
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#include "qextportsettings.h"


/*!
    \enum BaudRateType
*/

/*!
    \enum DataBitsType
*/

/*!
    \enum ParityType
*/

/*!
    \enum StopBitsType

    \value STOP_1
    \value STOP_1_5
    \value STOP_2
*/

/*!
    \enum FlowType
*/

/*! \class PortSettings

    \brief The PortSettings class contain port settings

    structure to contain port settings. This new code should using
    QextPortSettings.
*/


/*!
  \class QextPortSettings

    \brief The PortSettings class contain port settings

    structure to contain port settings.
*/

QextPortSettings::QextPortSettings(BaudRateType b, DataBitsType d, ParityType p
                                   ,StopBitsType s, FlowType f, long timeout, int customBaudRate)
    :BaudRate(b), DataBits(d), Parity(p), StopBits(s), FlowControl(f)
    , Timeout_Millisec(timeout), CustomBaudRate(customBaudRate)
{
    if (BaudRate==BAUDCustom && customBaudRate ==-1) {
        QESP_WARNING("QextPortSettings: Wrong custom BaudRate");
        BaudRate = BAUD9600;
    }
}

QextPortSettings::QextPortSettings(const PortSettings &s)
    :BaudRate(s.BaudRate), DataBits(s.DataBits), Parity(s.Parity), StopBits(s.StopBits)
    , FlowControl(s.FlowControl), Timeout_Millisec(s.Timeout_Millisec), CustomBaudRate(-1)
{

}
