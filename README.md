# stringbuffer
A simple, easy to use string library for C.

## Description
Stringbuffer is a lightweight C library for dynamic string manipulation. It provides an efficient way to build and modify strings with automatic memory management.

## Data Structure
The core data structure is `stringbuffer_t`:
```C
typedef struct stringbuffer {
    size_t capacity;  // Total buffer size in bytes
    size_t length;    // Current string length in bytes
    char* buffer;     // Pointer to the character buffer
} stringbuffer_t;
```
- `capacity`: The allocated buffer size. 0 for string literals (no allocation).
- `length`: The number of characters in the string (excluding null terminator).
- `buffer`: Pointer to the character array. May point to a string literal or dynamically allocated memory.

## Design Choices
- **Lazy Allocation**: The `SB` macro initializes stringbuffers without allocating memory, pointing to string literals. Memory is only allocated when the string is modified, reducing overhead for read-only operations.
- **Exponential Growth**: Buffer capacity doubles when reallocation is needed, balancing memory usage and reallocation frequency.
- **C-Style API**: Functions take pointers to stringbuffer_t structures, following C conventions and allowing efficient pass-by-reference.
- **Inline Functions**: Accessor functions like `sb_length()` are inline for performance, avoiding function call overhead.
- **Lazy Null-Termination**: The buffer is not null-terminated during operations; the null character is only added when `sb_str()` is called, optimizing performance for internal manipulations.
- **No Exceptions**: Pure C implementation with return codes or direct error handling, suitable for systems without exception support.
- **Minimal Dependencies**: Only depends on standard C libraries (stdlib.h, string.h), making it portable.

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
sb_insert(&sb, 5, &SB(" inserted "));

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
- `stringbuffer_t sb_substr(const stringbuffer_t* sb, size_t start, size_t len)`: Extract substring
- `stringbuffer_t sb_view(const stringbuffer_t* sb, size_t start, size_t len)`: Create a view into the stringbuffer without copying
- `size_t sb_find(const stringbuffer_t* sb, const stringbuffer_t* substr)`: Find substring position
- `int sb_cmp(const stringbuffer_t* sb1, const stringbuffer_t* sb2)`: Compare stringbuffers
- `void sb_upper(stringbuffer_t* sb)`: Convert to uppercase
- `void sb_lower(stringbuffer_t* sb)`: Convert to lowercase

### Accessor Functions
- `char* sb_buffer(stringbuffer_t* sb)`: Get buffer pointer
- `size_t sb_length(const stringbuffer_t* sb)`: Get string length
- `size_t sb_capacity(const stringbuffer_t* sb)`: Get buffer capacity
- `size_t sb_available(const stringbuffer_t* sb)`: Get available space

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

### Running Benchmarks
```bash
make benchmark
./benchmark
```

#### Benchmark Results (example run)
- Append 1,000,000 characters: stringbuffer is ~14x faster than std::string
- Insert 100,000 characters: stringbuffer is ~21% faster than std::string
- Insert strings 10,000 times: stringbuffer is ~41% faster than std::string

## License
This project is licensed under the MIT License - see the LICENSE file for details.