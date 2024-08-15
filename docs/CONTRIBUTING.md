# Contributing
If you wish to help the project by decompiling game functions, there are some basic guidelines to adhere to.

## Data types
We use the following aliases (defined in ``common.h``) for primitive data types.
| Data Type          | Alias    |
|--------------------|----------|
| ``unsigned char``  | ``u8``   |
| ``signed char``    | ``s8``*  |
| ``unsigned short`` | ``u16``  |
| ``signed short``   | ``s16``  |
| ``unsigned int``   | ``u32``  |
| ``signed int``     | ``s32``  |

*Strings are still defined with ``char*`` or ``char str[]``.

## Naming conventions

1. Function names are written in Upper Camel Case, e.g. ``LoaderSysResetSystem``.
2. Variable names are written in Camel Case, e.g. ``threadId``.
3. Constant names are written in Upper Snake Case, e.g. ``INTC_HANDLER_LIST``.
4. Functions without a known name use a special naming convention, using the suffix ``__sub_xxxxx``.
- E.g. ``func_01234 -> GuessedName__sub_01234``.

## Style Guidelines
1. Indent with four spaces.
2. Functions are written in Allman Style, .e.g. 
```c
void function()
{
    // code
}
```

## Reversing Functions
When you have followed the setup instructions and run the ``configure.py`` script, an ``asm`` directory will be generated, which contains a ``nonmatchings`` directory. Any assembly functions in any of its subdirectories are ready to be reversed. When you have chosen a function, select and copy the entire contents of the ``.s`` file, 
open https://decomp.me/new, select the ``Shadow of the Colossus`` preset and paste the assembly into ``Target assembly``. When the function is successfully matched, open the corresponding C source file in ``src`` and look for an ``INCLUDE_ASM`` macro. Delete the corresponding line of code, and insert the reversed function in its place.
If the function references rodata, delete the corresponding ``INCLUDE_RODATA`` macro and define the data in the function. Run ``ninja`` to ensure that the generated binary matches the original and submit a pull request.
