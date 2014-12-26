Bled - BusyBox Library for Easy Decompression
=============================================

## What's this then?

This is _Bled_, the __B__usyBox __L__ibrary for __E__asy __D__ecompression.

## What's it for?

It's a library, that can be used in your applications to handle the decompression of
__.Z__, __.gz__, __.bz2__, __.lzma__, __.xz__, __.zip__ (_planned_) and __.7z__ (_planned_)
compressed files and/or archives.

## Why are you doing that?

Because I need this stuff for [Rufus](http://rufus.akeo.ie), mostly to handle compressed
disk images, and nobody outside of [BusyBox](http://www.busybox.net/) seems to have put
much effort providing a compact, one-size-fits-all Open Source library, for decompressing
the most common compression formats.

As I am very conscious of size in Rufus, and Busybox is targeted at embedded systems,
its decompression library seemed like a natural choice. Sure, the [7-Zip LZMA SDK](http://www.7-zip.org/sdk.html)
is nice, and compiles nicely on Windows, but it only supports lzma/lzma2/xz (+.7z archives),
so you'll need to add stuff like [miniz](https://code.google.com/p/miniz/), and then some
more, and soon enough you're dealing with multiple APIs and a lot of duplicated code.
Plus these libraries also provide compression support, which we don't need, so the whole
thing becomes quite large.

## What's the license?

GPLv2 __or later__.

While the BusyBox project as a whole is GPLv2 __only__, this library is GPLv2
__or later__, which means it can be used indiscriminately in GPLv2 or GPLv3 projects.
That's because the BusyBox sources we used were all explictly tagged GPLv2 or later and
 the
ones we needed that weren't (`libbb.h`, `crc32.c`, `decompress_unxz.c`) have been replaced
with versions that are. Also, since we're not using any part of bzlib there's no
additional license notices required. This really is a __pure__ GPLv2 or later
decompression library for the most common compression formats.

## What about compressed tar archives?

I'm not planning to handle these, because I don't need tar extraction for Rufus.
However, tar archive handling is present in the BusyBox source, and I'll gladly accept
a patch.

## What about rar?

Not planning to add support for that either.

## Are there any limitations?

* Only Windows is supported for now (MinGW/gcc and MSVC). That's because:
  * Windows is the only platform I need for Rufus
  * The original libbb.h was GPLv2 __only__, so I had to recreate my own from scratch
    that is GPLv2 __or later__. This was a bit of a pain, so I'm not gonna bother
    recreating a GPLv2+ libbb.h for Linux or other platforms.
* Only uncompressing to file is supported for now. But I'm planning to add uncompression
  to a buffer, at least for lzma and xz, since I'm gonna need that.
* Can't query the uncompressed size for now, but I'll add that.
* .zip and .7z archive extraction to a directory is not currently supported. But I'll
   probably need that eventaully, so I'm planning to add it then. .zip archive support
   shouldn't be much of problem, since BusyBox already had it. .7z will be a different
   story, as it looks like this will need to be created from scratch.
* No multithreading or progress support. But once extraction to a buffer is implemented
  progress can be figured out by keeping track of how much data is being processed.
* Probably some limitation on some of the advanced compression features that can be
  added to an archive and/or newer compression formats

## Alright I'm sold. What do I need?

Either:

* Visual Studio 2013 (or later)
* MinGW

Then either use the `.sln` file or run `configure` + `make`.


## Is the name a reference to _Bled, Slovenia_?

It is. You should visit Slovenia too, if you ever have a chance.
