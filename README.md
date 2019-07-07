This repository contains a set of bindings for [libtorrent](https://github.com/arvidn/libtorrent) to work with Kotlin/native. It does not bind all of the functionality, but only implements the high-level features of the library that are necessary to build something with it.

## Building

You will need to install a few dependencies...

- `libtorrent-rasterbar` | [AUR](https://aur.archlinux.org/packages/libtorrent-rasterbar-git/) | [Launchpad](https://launchpad.net/ubuntu/+source/libtorrent-rasterbar) | [docs](https://www.rasterbar.com/products/libtorrent/)
- `boost` | [AUR](https://www.archlinux.org/packages/extra/x86_64/boost/) | [docs](https://www.boost.org/)

...after which, you can run `make` to compile the project. This is currently a work in progress, and is only tested on Arch Linux. Feel free to ask for help in the issue tracker, or contact us some other way if you encounter any problems.
