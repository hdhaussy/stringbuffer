# stringbuffer
A simple, easy to use string library for C.

## Description
Stringbuffer is a lightweight C library for dynamic string manipulation. It provides an efficient way to build and modify strings with automatic memory management.

## Features
- Dynamic string building with automatic reallocation
- Efficient append, insert, replace operations
- Substring extraction and search
- Case conversion (upper/lower)
- Formatted string appending (printf-style)
- Comparison functions
- Memory-safe operations

## Usage

### Initialization
You can initialize a stringbuffer using the `SB` macro:
```C
#include "stringbuffer.h"

stringbuffer_t sb = SB("Hello, World");
```

For an empty stringbuffer:
```C
stringbuffer_t sb = SB("");
```

**Note:** The `SB` macro does not allocate memory; it creates a stringbuffer that points to the string literal. Memory is only allocated when the stringbuffer is modified (e.g., via append, insert, etc.).

### Basic Operations
```C
// Append strings
sb_append_str(&sb, " more text");

// Append characters
sb_append_char(&sb, '!');

// Get the C string
const char* str = sb_str(&sb);

// Get length and capacity
size_t len = sb_length(&sb);
size_t cap = sb_capacity(&sb);
```

### Advanced Operations
```C
// Insert at position
sb_insert_str(&sb, 5, &SB(" inserted "));

// Replace substring
sb_replace(&sb, 0, 5, &SB("Hi"));

// Find substring
size_t pos = sb_find(&sb, &SB("World"));

// Extract substring
stringbuffer_t sub = sb_substr(&sb, 0, 5);
sb_release(&sub); // Don't forget to release

// Case conversion
sb_upper(&sb);
sb_lower(&sb);
```

### Memory Management
Always release the stringbuffer when done:
```C
sb_release(&sb);
```

## API Reference

### Core Functions
- `void sb_alloc(stringbuffer_t* sb, size_t size)`: Allocate/reallocate buffer
- `void sb_release(stringbuffer_t* sb)`: Free buffer memory
- `const char* sb_str(stringbuffer_t* sb)`: Get null-terminated C string

### Modification Functions
- `void sb_append(stringbuffer_t* dest, const stringbuffer_t* src)`: Append stringbuffer
- `void sb_append_str(stringbuffer_t* dest, const char* src)`: Append C string
- `void sb_append_char(stringbuffer_t* dest, char c)`: Append character
- `void sb_insert(stringbuffer_t* dest, size_t pos, const stringbuffer_t* src)`: Insert at position
- `void sb_insert_char(stringbuffer_t* dest, size_t pos, char c)`: Insert character
- `void sb_replace(stringbuffer_t* dest, size_t pos, size_t len, const stringbuffer_t* src)`: Replace substring
- `void sb_printf(stringbuffer_t* sb, const char* fmt, ...)`: Append formatted string

### Utility Functions
- `stringbuffer_t sb_substr(stringbuffer_t* sb, size_t start, size_t len)`: Extract substring
- `size_t sb_find(stringbuffer_t* sb, stringbuffer_t* substr)`: Find substring position
- `int sb_cmp(stringbuffer_t* sb1, stringbuffer_t* sb2)`: Compare stringbuffers
- `void sb_upper(stringbuffer_t* sb)`: Convert to uppercase
- `void sb_lower(stringbuffer_t* sb)`: Convert to lowercase

### Accessor Functions
- `char* sb_buffer(stringbuffer_t* sb)`: Get buffer pointer
- `size_t sb_length(stringbuffer_t* sb)`: Get string length
- `size_t sb_capacity(stringbuffer_t* sb)`: Get buffer capacity
- `size_t sb_available(stringbuffer_t* sb)`: Get available space

## Building and Testing

### Compilation
```bash
make test
```

### Running Tests
```bash
./test
```

### Memory Leak Check
```bash
valgrind --leak-check=full ./test
```

## License
This project is licensed under the MIT License - see the LICENSE file for details.