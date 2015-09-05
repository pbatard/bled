Bled - Base Library for Easy Decompression
=============================================

## What's this then?

This is _Bled_, the <b>B</b>ase <b>L</b>ibrary for <b>E</b>asy <b>D</b>ecompression. It
is based almost entirely on the native decompression code found in [BusyBox](http://www.busybox.net/).

## What's it for?

It's a library, that can be used in applications to handle the decompression of
__.Z__, __.gz__, __.bz2__, __.lzma__, __.xz__, __.zip__ and (_planned_) __.7z__ 
compressed files and/or archives.

## Why are you doing that?

Because I need this stuff for [Rufus](http://rufus.akeo.ie), mostly to handle compressed
disk images, and nobody outside of [BusyBox](http://www.busybox.net/) seems to have put
much effort providing a compact, one-size-fits-all Open Source library, for decompressing
the most common formats.

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
This is possible because the BusyBox sources we used were all explictly tagged GPLv2 or
later and any source that wasn't (`libbb.h`, `crc32.c`, `decompress_unxz.c`) has been
recreated/replaced with versions that are. Also, since we're not using any part of bzlib
there's no additional license notice required. So this really is a _pure_ GPLv2
__or later__ decompression library, for the most common compression formats.

## What about compressed tar archives?

I'm not planning to handle these, because I have no need for tar extraction in Rufus.
However, since tar archive handling is present in the BusyBox sources, I may accept a
patch if you feel like adding support for it.

## What about .rar?

Not planning to add support for RAR, unless a truly Free Source (GPL) RAR decompression
comes along.

## Are there any limitations?

* Only Windows is supported for now (MinGW/gcc and MSVC). That's because:
  * Windows is the only platform I need for Rufus
  * The original `libbb.h` was GPLv2 __only__, so I had to recreate my own from scratch
    that is GPLv2 __or later__. This was a bit of a pain, so I'm don't want to bother
    recreating a GPLv2+ libbb.h for Linux or other platforms.
* Can't query the uncompressed file size. This is mostly due to the limitation of the
  compression formats being used, as most of them are stream formats and do not store
  that information anywhere.
* .7z archive extraction is not currently supported. But I may add this feature if needed.
* No multithreading and not optimized for speed __at all__!
* There probably exist limitations with regards to advanced compression and/or newer
  compression formats.

## Alright I'm sold. What do I need?

Either:

* Visual Studio 2013 (or later)
* MinGW

Then use the `.sln` file or run `configure` + `make`.

## Is the name a reference to _Bled, Slovenia_?

It is. You should visit Slovenia too, if you ever have a chance.
