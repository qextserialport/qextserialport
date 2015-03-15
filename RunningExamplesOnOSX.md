As with any application on OS X, in order to load dynamic libraries those libraries need to either be in a location the dynamic linker knows to look for them.  There are some default spots on your system: **$(HOME)/lib:/usr/local/lib:/lib:/usr/lib** but if you don't want to install a library there, you need to specify where to look.

You can do this by adding to the **DYLD\_LIBRARY\_PATH** environment variable.  If you're running in Qt Creator, you can add this as an entry into the runtime settings for your app (within the Projects tab), or any of the example apps: DYLD\_LIBRARY\_PATH should be set to $(qextserialport)/src/build.

You can also export this symbol to your environment if you're running the app from the command line:
```
export DYLD_LIBRARY_PATH+=/path/to/qesp
```

See http://developer.apple.com/mac/library/documentation/Darwin/Reference/ManPages/man1/dyld.1.html for more info on dyld.