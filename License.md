<font color='blue'>Since QextSerialPort 1.2-beta , this project uses the MIT license. </font>



## License ##

```
Copyright (c) 2000-2003 Wayne Roth
Copyright (c) 2004-2007 Stefan Sander
Copyright (c) 2007 Michal Policht
Copyright (c) 2008 Brandon Fosdick
Copyright (c) 2009-2010 Liam Staskawicz
Copyright (c) 2011 Debao Zhang

Web: http://code.google.com/p/qextserialport/

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

## Why license needed? ##

> Many users complains that, without a proper licence they can not use this library.

  * http://groups.google.com/group/qextserialport/browse_thread/thread/0e8756920b01da82

<pre>
Hi,<br>
we are considering using a modified version of QExtSerialPort in one of our<br>
projects (Qt Creator, http://qt.gitorious.org/qt-creator).<br>
Would it be possible to add license header information or a license file to the<br>
QExtSerialPort code base? - This would make re-use of the code base easier.<br>
If that is not  possible, could we redistribute the source code with BSD-<br>
license headers manually added?<br>
</pre>

And

<pre>
I am also considering packaging the software for Debian, but I<br>
couldn't do it yet just because of the license.<br>
</pre>

  * http://code.google.com/p/qextserialport/issues/detail?id=8

<pre>
Questions:<br>
Can I use qextserialport in a commercial product?<br>
If yes, how?<br>
Compile it in? I guess no.<br>
If I can use it as a library, how should the README be formulated?<br>
Is the "MIT license" from 2008 appropriate?<br>
</pre>

## Why can we use MIT? ##

**Form the history of [qt-interest mail list](http://lists.trolltech.com/qt-interest/2004-12/msg01022.html)**
  * Wayne Roth, the original author of the project, had said that:
<pre>
the code is in the public domain. Do whatever you like with it. Right<br>
now I have too many other things to do to put any serious time into<br>
fixing it.  Trolltech should be aware of this already; they asked<br>
about a license when they offered to host the tarball.<br>
</pre>
  * Stefan Sander, the maintainer of qextserialport on sourceforge, said that
<pre>
Hello,<br>
My project registration at !SourceForge have been approved.<br>
http://www.sf.net/projects/qextserialport<br>
I thought an initial licence of Public Domain would be best solution.<br>
Someone wrote: - Because its public domain, some could fork it under different licenses -<br>
</pre>

**And from [this thread](http://groups.google.com/group/qextserialport/browse_thread/thread/fbcddbfb4a0b5a51?pli=1) on qesp mail list, we can see that, current maintainers and users agree with a MIT licence.**
  * Brandon Fosdick,
<pre>
I would vote for BSD or MIT :)<br>
</pre>
  * Liam Staskawicz,
<pre>
That works for me - let's call it MIT and go for it :)<br>
</pre>