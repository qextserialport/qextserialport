Informations, directions and strategies for developers.


# Doctrine #

  * Utilize 100% of language features; if something makes sense, then do not worry about braking OOP canons or other theoretical rules.

  * Write simple and intuitive code, so that other programmers won't be confused when they meet function called "theGreatReturn". Take an effort to find good names for classes or functions (in respect to existing naming standard).

  * Avoid repetition of code.

  * "Premature optimization is the root of all evil". At first attempt, value clean and secure code over fast and dirty code.

  * Integrate as much as possible with Qt.

  * Use English.

# Versioning #

As of version 1.2.0 library will use [semantic versioning](#References.md)<sup>1 2</sup> scheme. Version parts are provided in major.minor.patch[.qualifier] format, where:
  * major - increases when functions are removed or other revolutionary changes have been made, so that backward compatibility is broken.
  * minor - increases when new features have been added, but API and ABI are backward compatible.
  * patch (sometimes also called "micro" or "release") - increases with each release, which is forward and backward compatible. Changes typically include bug fixes.
  * qualifier - not mandatory string. Does not contain compatibility information and can be used to denote platform specific or unofficial releases.

## References ##

<a>1.</a>http://semver.org/

<a>2.</a>http://www.osgi.org/wiki/uploads/Links/SemanticVersioning.pdf