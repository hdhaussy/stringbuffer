# sringbuffer
A simple, easy to use string library for C.

## stringbuffer literal
You can initialize a stringbuffer literal very easily using the macros SB or SB_EMPTY.
```C
stringbuffer_t s1 = SB("Hello, World");
stringbuffer_t s2 = SB_EMPTY;

```