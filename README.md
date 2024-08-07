# Shadow of the Colossus Decompilation Project
We are currently targeting the main ELF (``SCPS_150.97``) of the Preview version.

![Discord Banner 1](https://discord.com/api/guilds/465610776762384394/widget.png?style=banner1)

## Setup
1. ``sudo apt install gcc-mipsel-linux-gnu binutils-mips-linux-gnu ninja-build``
2. ``sudo apt install python3-pip``
3. ``python3 -m pip install -U -r requirements.txt``
4. Create an ``iso`` directory in the root directory
5. From a Shadow of the Colossus Preview Version ISO, extract ``SCPS_150.97`` into the ``iso`` directory
6. ``configure.py``
7. ``ninja``
