# Shadow of the Colossus Decompilation Project
![Endpoint Badge](https://img.shields.io/endpoint?url=https%3A%2F%2Fprogress.deco.mp%2Fdata%2Fsotc%2Fpreview%2Fdefault%2F%3Fmode%3Dshield%26measure%3Dos&style=flat&label=code)

<!-- Contributors shield -->
[contributors-url]: https://github.com/Fantaskink/SOTC/graphs/contributors
[contributors-badge]: https://img.shields.io/github/contributors/Fantaskink/SOTC

We are currently targeting the main ELF (``SCPS_150.97``) of the Preview version.

<a href="https://discord.gg/3nmDn6p7" target="_blank">
  <img src="https://discord.com/api/guilds/465610776762384394/widget.png?style=banner2" alt="Discord Banner">
</a>


## Setup
Disclaimer: Linux or WSL2 required
1. ``sudo apt install gcc-mipsel-linux-gnu binutils-mips-linux-gnu ninja-build``
2. ``sudo apt install python3-pip``
3. ``python3 -m pip install -U -r requirements.txt``
4. Create an ``iso`` directory in the root directory
5. From a Shadow of the Colossus Preview Version ISO, extract ``SCPS_150.97`` into the ``iso`` directory
6. ``configure.py``
7. ``ninja``
