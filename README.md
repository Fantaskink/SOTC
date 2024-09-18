# Shadow of the Colossus Decompilation Project
[code-progress-badge]: https://img.shields.io/endpoint?url=https%3A%2F%2Fprogress.deco.mp%2Fdata%2Fsotc%2Fpreview%2Fdefault%2F%3Fmode%3Dshield%26measure%3Dos&style=flat&label=SCPS_150.97

<!-- Contributors shield -->
[contributors-url]: https://github.com/Fantaskink/SOTC/graphs/contributors
[contributors-badge]: https://img.shields.io/github/contributors/Fantaskink/SOTC?color=green

<!-- Shields -->
![code-progress-badge] [![Contributors][contributors-badge]][contributors-url]

We are currently targeting the Preview Version's main ELF (``SCPS_150.97``, sha1: ``c4d5576d1cae3721c411a746c7845f5c6f026dbb``).

<a href="https://discord.gg/WGSnPQFtHU" target="_blank">
  <img src="https://discord.com/api/guilds/465610776762384394/widget.png?style=banner2" alt="Discord Banner">
</a>

## Getting Started
### Requirements
**Disclaimer**: Linux or WSL2 required
1. ``sudo apt install gcc-mipsel-linux-gnu binutils-mips-linux-gnu ninja-build``
2. ``sudo apt install python3-pip``
3. ``python3 -m pip install -U -r requirements.txt``
4. ``scripts/setup_compiler.sh``

### Setup
1. Create an ``iso`` directory in the root directory
2. From a Shadow of the Colossus Preview Version ISO, extract ``SCPS_150.97`` into the ``iso`` directory
3. ``configure.py`` (optionally pass ``-c`` to do a clean split)
4. ``ninja``

## Contributing
Use our [contribution guide](docs/CONTRIBUTING.md).
