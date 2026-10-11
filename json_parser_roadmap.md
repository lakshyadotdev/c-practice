# Building a JSON Parser in C — Prerequisites & Learning Roadmap

## Overview: How All the Pieces Fit Together

A JSON parser takes a raw string of text — like `{"name": "Alice", "age": 30}` — and transforms it into a structured, in-memory C representation you can work with programmatically.

This happens in two distinct stages:

1. **Tokenization (Lexing):** A component called the *lexer* reads the raw text character by character and groups characters into meaningful *tokens* — e.g., `{`, `"name"`, `:`, `"Alice"`, `,`, `"age"`, `30`, `}`. It doesn't understand structure, only vocabulary.

2. **Parsing:** A component called the *parser* consumes the token stream produced by the lexer and builds a tree of C structs that mirrors the hierarchical structure of the JSON. It enforces grammar rules — for instance, after a `{` you must have a key-value pair or `}`.

The output is a tree of dynamically-allocated structs. A JSON object becomes a linked list of key-value pairs; a JSON array becomes a linked list of values; a JSON string becomes a heap-allocated `char*`; and so on.

All of this requires fluency in pointers, dynamic memory, recursion, and C strings — but you don't need to master all of it before you start. The roadmap tells you exactly when each concept becomes relevant.

---

## 1. Prerequisite Knowledge Checklist

### ✅ Must Understand Before Starting

These are the concepts you will use from day one. Without them, even early milestones will stall.

---

#### Pointers and Pointer-to-Pointer

**What it is:** A pointer holds the memory address of another value. A pointer-to-pointer (`char**`) holds the address of a pointer.

**Why the parser needs it:** Your lexer will walk through the input string by advancing a pointer — the *cursor*. The parser needs to pass this cursor to sub-functions and have them advance it, which means passing a pointer to the cursor (`const char**`). Without this, each function would get its own copy of the position and the caller would never know how far the input was consumed.

**How deeply:** You need to understand indirection, dereferencing (`*p`), address-of (`&x`), passing pointers to functions, and `const` qualification on pointers. You do not need pointer arithmetic beyond incrementing and indexing.

---

#### Strings in C

**What it is:** C strings are null-terminated arrays of `char`. There is no built-in string type.

**Why the parser needs it:** Your entire input is a C string. You will scan it character by character, compare substrings, copy string content onto the heap, and null-terminate those copies manually.

**How deeply:** Know `strlen`, `strcmp`, `strncmp`, `memcpy`, and how to manually null-terminate a buffer. Understand that a string literal `"hello"` is read-only and you cannot modify it.

---

#### Structs

**What it is:** A `struct` groups related fields under one name. You can have pointers to structs and structs that contain pointers to other structs.

**Why the parser needs it:** Every JSON value (string, number, boolean, null, object, array) will be represented as a struct. Objects and arrays will be linked lists of structs. You will define a recursive struct where a JSON value can contain other JSON values.

**How deeply:** Understand struct definition, `typedef`, member access (`.` and `->`), and passing structs by pointer to functions.

---

#### Enums

**What it is:** An enum is a named set of integer constants.

**Why the parser needs it:** You need to tag what kind of value a JSON node represents. Is it a string? A number? An object? An array? This tag is stored as an enum. Your parser will `switch` on this tag constantly.

**How deeply:** Basic enum definition and use in `switch` statements is all you need.

---

#### Dynamic Memory Allocation

**What it is:** `malloc`, `calloc`, `realloc`, and `free` let you allocate memory at runtime from the *heap*, as opposed to stack memory that disappears when a function returns.

**Why the parser needs it:** You don't know in advance how many keys an object has, how long a string is, or how deeply nested a value is. All parsed nodes must live on the heap so they outlive the function that created them.

**How deeply:** Know `malloc`/`free`, always check for NULL return, and understand that you must `free` everything you `malloc`. You do not need `realloc` for the initial version, but understanding it is useful.

---

#### Recursion

**What it is:** A function calling itself, directly or indirectly, to solve a sub-problem of the same type.

**Why the parser needs it:** JSON is inherently recursive. An array can contain objects; those objects can contain arrays. The function `parse_value` will call `parse_object`, which calls `parse_value` again for each value. This is not a workaround — it is the natural shape of the algorithm.

**How deeply:** You need to be comfortable with the call stack growing and unwinding, passing state through parameters rather than globals, and trusting that a recursive call will return a correct result.

---

### 🔶 Learn as You Implement (Milestone-Specific)

These concepts can be picked up when you hit the milestone that requires them. Don't block on them upfront.

| Concept | When It Becomes Relevant |
|---|---|
| Linked lists | Milestone 3 (objects and arrays) |
| `strtod` / `strtol` for number parsing | Milestone 4 (numbers) |
| `union` types in C | Milestone 2 (tagged union for JSON value) |
| `errno` and error state | Milestone 5 (error handling) |
| `valgrind` / memory sanitizers | Milestone 6 (memory cleanup) |

---

## 2. Key Terminology and Concepts

Before you can read about parsing techniques or reason about your design, you need a shared vocabulary. Each term is explained without assuming compiler background.

---

### Token

A **token** is the smallest meaningful unit in the source text after it has been broken up. For JSON, tokens are:

- Structural characters: `{`, `}`, `[`, `]`, `:`, `,`
- String literals: `"hello world"`
- Number literals: `42`, `-3.14`
- Keywords: `true`, `false`, `null`

A token is not a single character and not an entire expression — it's a categorized chunk. The lexer produces a sequence of tokens; the parser reads that sequence.

---

### Lexer (Tokenizer)

The **lexer** (also called a *tokenizer* or *scanner*) is the part of your parser that reads raw text and produces tokens. It knows nothing about structure. It just answers the question: "What is the next token in this string?"

For JSON, the lexer needs to:
- Skip whitespace
- Detect what kind of token starts at the current position
- Extract the token content
- Advance the cursor past the token

---

### Parser

The **parser** reads the stream of tokens and builds structured output. It enforces **grammar** — the rules that define what sequences of tokens are valid JSON.

In a simple design, the lexer and parser can be fused: the parser calls a `next_token()` helper inline rather than pre-tokenizing everything. This is fine for JSON and easier to implement.

---

### Grammar

A **grammar** is a set of rules describing what sequences of tokens are legal in a language. JSON's grammar says things like:

- A *value* is a string, number, boolean, null, object, or array.
- An *object* starts with `{`, followed by zero or more `"key" : value` pairs separated by `,`, followed by `}`.
- An *array* starts with `[`, followed by zero or more values separated by `,`, followed by `]`.

Your parser is essentially an executable encoding of this grammar. Each grammar rule becomes a function.

---

### Recursive Descent Parsing

**Recursive descent** is the parsing strategy you will use. It is the simplest technique that works for JSON.

The idea: for each rule in the grammar, write a function. When that rule can contain another rule (e.g., a value inside an array), the function calls the function for that rule. Because grammar rules can refer to themselves (a value can be an array which contains values), the functions can call each other recursively.

The result is a top-down parser that mirrors the grammar directly. There is no separate parsing table, no state machine, and no complex algorithm — just one function per grammar rule.

---

### Abstract Syntax Tree (AST)

An **AST** is the tree-shaped data structure that represents the parsed input. Each node in the tree corresponds to a construct in the JSON:

```
Object
├── "name" → String("Alice")
└── "age"  → Number(30)
```

For JSON, the AST is often called a *value tree* because every node is a JSON value. Unlike in general compilers (where the AST may be simplified or transformed), your JSON AST will be a direct structural copy of the input.

---

### Cursor

The **cursor** is a pointer into the input string marking your current read position. When the lexer reads a token, it advances the cursor past it. The cursor is typically a `const char*` that is passed by pointer (`const char**`) so functions can advance it.

Think of it like a reading finger on a page — it always points to "the next character to read."

---

### Tagged Union / Discriminated Union

A **tagged union** is a C pattern for representing a value that can be one of several types. You pair an enum tag with a `union`:

```
struct JsonValue {
    JsonType type;   // the tag: STRING, NUMBER, OBJECT, ARRAY, BOOL, NULL_VAL
    union {
        char*        string_val;
        double       number_val;
        JsonObject*  object_val;
        JsonArray*   array_val;
        int          bool_val;
    } as;
};
```

The `type` field tells you which member of the `union` is valid. You always check the tag before accessing a union member.

---

### Memory Ownership

**Memory ownership** is the question of "who is responsible for freeing this allocation?" In C there is no garbage collector, so you must decide. In your parser:

- The parser *owns* all the nodes it allocates.
- The caller *owns* the root value returned by the parser.
- You must provide a `json_free()` function that recursively frees the entire tree.

Failing to enforce clear ownership is the primary cause of memory leaks in C parsers.

---

### Parsing State / Error State

As the parser runs, it tracks whether it is in a valid state. If it encounters malformed input — say, a key without a colon — it needs to report an error and stop. A simple approach is to return `NULL` from every parsing function to signal failure, and propagate `NULL` upward through the call stack. A more informative approach stores an error message and position.

---

## 3. Mental Model and Step-by-Step Parsing Algorithm

This section explains the algorithm in plain language. No code — just the sequence of decisions and why each step exists.

---

### Phase 1: Lexing (What is the next token?)

The lexer's job is simple and stateless — given a position in the string, identify and return the next token.

**Step 1 — Skip whitespace:** JSON allows spaces, tabs, newlines, and carriage returns between tokens. Before reading anything, advance the cursor past any whitespace.

**Step 2 — Inspect the current character:** The first character determines the token type:
- `{` → `TOKEN_LBRACE`
- `}` → `TOKEN_RBRACE`
- `[` → `TOKEN_LBRACKET`
- `]` → `TOKEN_RBRACKET`
- `:` → `TOKEN_COLON`
- `,` → `TOKEN_COMMA`
- `"` → beginning of a string — read until the closing `"`
- `-`, `0`–`9` → beginning of a number — read until non-numeric characters
- `t` → attempt to match `true`
- `f` → attempt to match `false`
- `n` → attempt to match `null`
- anything else → lexer error

**Step 3 — Advance the cursor past the token:** After identifying the token, the cursor should sit on the first character that is NOT part of the token.

---

### Phase 2: Parsing (What structure does this token sequence form?)

The parser calls the lexer repeatedly and builds the tree.

**`parse_value` — The central function:**  
This is the entry point for parsing any JSON value. It asks the lexer "what's the next token?" and dispatches:

- `"` → call `parse_string`
- digit or `-` → call `parse_number`
- `{` → call `parse_object`
- `[` → call `parse_array`
- `t` → call `parse_true`
- `f` → call `parse_false`
- `n` → call `parse_null`
- anything else → return error

**`parse_object`:**
1. Consume the `{` token (already seen by `parse_value`)
2. If the next token is `}`, return an empty object
3. Otherwise, enter a loop:
   - Expect a string token — this is the key
   - Expect a `:` token
   - Call `parse_value` recursively — this is the value
   - Add the key-value pair to the object
   - If the next token is `,`, continue the loop
   - If the next token is `}`, return the object
   - Otherwise, return an error

**`parse_array`:**
Similar to `parse_object`, but without keys. Expect `[`, then zero or more values separated by `,`, then `]`.

**`parse_string`:**
1. Advance past the opening `"`
2. Scan forward until the closing `"`, handling escape sequences (`\"`, `\\`, `\n`, `\t`, `\uXXXX`)
3. Copy the content into a heap-allocated buffer
4. Null-terminate and return

**`parse_number`:**
1. Find the end of the number (includes optional `-`, digits, `.`, `e`, `E`, `+`, `-`)
2. Use `strtod` to convert the substring to a `double`

---

### How Nesting Works

The key insight: because `parse_object` and `parse_array` both call `parse_value`, and `parse_value` can call either of them, nesting is handled automatically by the call stack. Each recursive call processes one level of nesting. The depth is only limited by your system's stack size (deep nesting in real parsers is handled with explicit stacks, but for a beginner project, recursion is fine).

---

### How Malformed Input Is Detected

At every point where the grammar requires a specific token, you check that the actual next token matches. If it doesn't, you return an error:

- Missing `:` after an object key → error
- Missing `"` to close a string → error
- Trailing comma in an array → error (strict JSON disallows this)
- Unknown character → lexer error

---

## 4. Recommended Internal Data Representation

### The Core Design

Use a **tagged union struct** for JSON values, a **singly-linked list** for object key-value pairs, and a **singly-linked list** for array elements.

This is not the most memory-efficient representation, but it is the most beginner-friendly — each new key or array element is a new `malloc` call, no resizing logic needed.

---

### The Three Struct Types You Need

**1. `JsonValue` — Represents any JSON value**

Fields:
- `type` — an enum: `JSON_NULL`, `JSON_BOOL`, `JSON_NUMBER`, `JSON_STRING`, `JSON_ARRAY`, `JSON_OBJECT`
- A union containing one field per type: `double` for numbers, `char*` for strings, `int` for booleans, and pointers to `JsonObject` / `JsonArray` for compound types

**2. `JsonPair` — One key-value entry in an object**

Fields:
- `key` — a heap-allocated `char*`
- `value` — a pointer to a `JsonValue`
- `next` — a pointer to the next `JsonPair` (forming a linked list)

**3. `JsonArray` — One element in an array**

Fields:
- `value` — a pointer to a `JsonValue`
- `next` — a pointer to the next `JsonArray` node

---

### Why Pointers and Dynamic Allocation Are Necessary

You cannot put a `JsonValue` directly inside itself (C forbids this), so all containment must happen through pointers. The size of objects and arrays is unknown at compile time, so allocations must happen at runtime. Even string lengths are unknown — you can't declare `char key[???]` — so strings must be heap-allocated.

---

### Alternative Approaches (and Why to Skip Them for Now)

| Approach | Trade-off |
|---|---|
| Pre-allocated arena allocator | Very fast, but you need to implement your own memory pool — advanced |
| Dynamic arrays (`realloc`) instead of linked lists | Faster iteration, but more complex allocation logic |
| `cJSON`-style flat struct (all fields in one struct, unused ones zero) | Simple but wastes memory and lacks type safety |

**Recommendation:** Linked lists + tagged union struct. It closely mirrors the logical structure of JSON, handles arbitrary nesting without extra machinery, and keeps allocation simple.

---

## 5. Milestone-Based Implementation Roadmap

Each milestone builds on the previous. Do not skip ahead. The goal is always a working, testable component, not a complete draft.

---

### Milestone 1 — Lexer for Structural Tokens

**What to implement:**
A function `next_token(const char** cursor, Token* out)` that reads the next token and advances the cursor. Handle only: `{`, `}`, `[`, `]`, `:`, `,`, whitespace skipping, and end-of-input.

**Concepts it teaches:**
- Cursor mechanics (pointer-to-pointer)
- Character comparison and dispatch with `switch`
- Returning data via output parameters

**Test inputs:**
- `{}` → should produce: `LBRACE`, `RBRACE`, `EOF`
- `{ }` → same, with whitespace handled
- `[,]` → `LBRACKET`, `COMMA`, `RBRACKET`
- `@` → lexer error

**Understand before moving on:**
Why the cursor is `const char**` and not `const char*`. What happens if you forget to advance the cursor.

---

### Milestone 2 — Lexer for Strings, Numbers, and Keywords

**What to implement:**
Extend `next_token` to handle string literals (basic, no escapes yet), number literals (integers only), and the keywords `true`, `false`, `null`.

**Concepts it teaches:**
- Scanning forward to find a delimiter (`"`)
- Using `strncmp` to match keywords
- The difference between lexing (what characters form the token?) and parsing (what does the token mean?)

**Test inputs:**
- `"hello"` → `STRING("hello")`
- `123` → `NUMBER(123)`
- `true` → `BOOL_TRUE`
- `trueXYZ` — decide: is this valid? (hint: depends on context, test both)
- `"unterminated` → lexer error

**Understand before moving on:**
How the string's start and end positions are captured. Why the lexer stores a `const char*` pointer into the original buffer (slice) rather than copying immediately.

---

### Milestone 3 — Parser for Null, Bool, and a Single Object

**What to implement:**
Define the `JsonValue`, `JsonPair` structs. Implement `parse_value`, `parse_null`, `parse_bool`, and `parse_object` (keys must be strings, values must be null or bool only for now).

**Concepts it teaches:**
- Tagged union in practice
- `malloc` and `free` for struct allocation
- Linked list construction (prepend or append `JsonPair` nodes)
- Recursive descent structure

**Test inputs:**
- `null` → `JsonValue{type: JSON_NULL}`
- `true` → `JsonValue{type: JSON_BOOL, bool_val: 1}`
- `{"a": null, "b": true}` → object with two pairs
- `{"a":}` → parser error (missing value)
- `{"a" null}` → parser error (missing colon)

**Understand before moving on:**
How a linked list is built one node at a time. How errors are propagated upward (returning `NULL` and freeing partial allocations).

---

### Milestone 4 — Parser for Strings, Numbers, and Arrays

**What to implement:**
Add `parse_string` (copy string content to heap), `parse_number` (using `strtod`), and `parse_array`. All value types now work. Objects can contain arrays; arrays can contain objects.

**Concepts it teaches:**
- Heap string allocation and null-termination
- Using standard library (`strtod`, `memcpy`)
- How recursive calls naturally handle nesting

**Test inputs:**
- `"hello world"` → `JsonValue{type: JSON_STRING, string_val: "hello world"}`
- `[1, 2, 3]` → array of three numbers
- `{"arr": [null, true]}` → object containing an array
- `[[[]]]` → deeply nested empty arrays
- `[1, 2,]` → trailing comma — decide if you reject this

**Understand before moving on:**
Why the string content must be copied to the heap (not just pointed into the input). What `strtod` returns and how to detect an invalid number.

---

### Milestone 5 — String Escape Sequences

**What to implement:**
Handle `\"`, `\\`, `\/`, `\b`, `\f`, `\n`, `\r`, `\t` inside strings. Optionally handle `\uXXXX` (Unicode escape — harder).

**Concepts it teaches:**
- Multi-pass character scanning
- Writing to a separate output buffer while scanning input
- Distinguishing between the source representation and the in-memory representation

**Test inputs:**
- `"line1\nline2"` → string containing an actual newline character
- `"say \"hello\""` → string containing quotes
- `"\\"` → string containing one backslash
- `"\q"` → invalid escape — parser error
- `"\u0041"` → the letter `A` (if you implement Unicode)

**Understand before moving on:**
Why the parsed string may be shorter than the source text (e.g., `\"` in source is just `"` in memory). How to size your output buffer conservatively.

---

### Milestone 6 — Memory Cleanup

**What to implement:**
A `json_free(JsonValue* value)` function that recursively walks the entire tree and `free`s every allocation: strings, pair keys, linked list nodes, value nodes.

**Concepts it teaches:**
- Recursive tree traversal
- The order of freeing (free children before the node itself)
- Running `valgrind` or address sanitizer to verify

**Test inputs:**
Run your existing tests, then call `json_free` on the result. Use `valgrind --leak-check=full` or compile with `-fsanitize=address`. Zero leaks is the goal.

**Understand before moving on:**
Why freeing a node before its children is wrong. The ownership model — who frees what.

---

### Milestone 7 — Error Reporting

**What to implement:**
Instead of returning bare `NULL`, attach an error message and character offset to the parser. Define a `JsonResult` struct with a success/failure flag, error message, error position, and the parsed value.

**Concepts it teaches:**
- Struct-based return values
- Error propagation through recursive calls
- Position tracking in the cursor

**Test inputs:**
Intentionally malformed JSON. Verify that the error message and position are correct and helpful.

---

## 6. Testing Checklist

### Basic Correctness

- [ ] `null` → null value
- [ ] `true`, `false` → boolean values
- [ ] `0`, `42`, `-7`, `3.14`, `-0.001`, `1e10`, `1.5E-3` → number values
- [ ] `""` → empty string
- [ ] `"hello"` → simple string
- [ ] `{}` → empty object
- [ ] `[]` → empty array
- [ ] `{"a":1}` → single-entry object
- [ ] `[1,2,3]` → three-element array
- [ ] `{"x":[1,{"y":null}]}` → nested structures

### String Edge Cases

- [ ] `"\""` → string containing a quote
- [ ] `"\\"` → string containing a backslash
- [ ] `"\n"`, `"\t"`, `"\r"` → escape sequences
- [ ] `"\u0041"` → Unicode escape (if implemented)
- [ ] Unescaped control characters (bytes 0x00–0x1F) → should error per JSON spec
- [ ] String without closing `"` → lexer error

### Number Edge Cases

- [ ] Leading zeros: `007` → spec disallows this
- [ ] Lone `-` → error
- [ ] `1.` → error (must have digits after decimal point)
- [ ] `.5` → error (must have digit before decimal point)
- [ ] `1e` → error (exponent with no digits)

### Structural Edge Cases

- [ ] Trailing comma in object or array → error (strict JSON)
- [ ] Missing `,` between values → error
- [ ] Extra closing brace → error
- [ ] Deeply nested input (20+ levels) → should work correctly
- [ ] Whitespace everywhere: `{ "a" : 1 }` → valid

### Memory Correctness

- [ ] Zero memory leaks on valid input (valgrind)
- [ ] Zero memory leaks on invalid input (partial parse, then cleanup)
- [ ] No use-after-free (address sanitizer)
- [ ] No double-free (address sanitizer)

### Minimum vs. Standards-Compliant

| Feature | Required for Basic Parser | Required for Standards-Compliant |
|---|---|---|
| All six value types | ✅ | ✅ |
| Basic escape sequences | ✅ | ✅ |
| `\uXXXX` Unicode escapes | ❌ (skip for now) | ✅ |
| UTF-8 surrogate pairs | ❌ | ✅ |
| Reject unescaped control chars | ❌ | ✅ |
| Reject leading zeros in numbers | ✅ | ✅ |
| Full `double` range for numbers | ✅ | ✅ |
| Streaming / incremental parsing | ❌ | ❌ (separate concern) |

---

## 7. Specific Search Terms and Learning Resources

For each concept, the resource is paired with the exact reason to use it.

---

### C Pointers and Pointer-to-Pointer

- **Search:** `"pointer to pointer C explained"`, `"double pointer C function argument"`
- **Resource:** *The C Programming Language* (K&R), Chapter 5 — the definitive reference for pointer mechanics in C. Focus on sections 5.1–5.5.
- **Why:** Passing `const char**` for the cursor is the first real pointer-to-pointer usage you will encounter. K&R builds this intuition clearly.

---

### C Strings and Memory

- **Search:** `"C string heap allocation", "strncpy vs memcpy C", "null terminator C string"`
- **Resource:** `man 3 string` (on your terminal) — lists every standard string function with signatures and behavior.
- **Why:** You will call `strlen`, `memcpy`, `strncmp` constantly. The man page tells you exactly what each does and what edge cases exist.

---

### Dynamic Memory (`malloc`/`free`)

- **Search:** `"malloc free C tutorial", "heap vs stack C", "C memory leak valgrind"`
- **Resource:** *CS:APP* (Computer Systems: A Programmer's Perspective), Chapter 9 (Virtual Memory) — explains what the heap actually is.
- **Resource:** [Beej's Guide to C Programming](https://beej.us/guide/bgc/), Chapter on Memory Allocation — free, beginner-friendly, covers `malloc`/`free`/`realloc` with diagrams.
- **Why:** Understanding the heap is essential before trusting your own allocations.

---

### Recursion in C

- **Search:** `"recursion C call stack", "recursive descent parser C"`
- **Resource:** *The C Programming Language* (K&R), Chapter 4.10 — recursion explained with a concrete example.
- **Why:** Your parser's correctness depends on understanding what the call stack holds at each level of nesting. K&R's example is short and concrete.

---

### Tagged Union / Union in C

- **Search:** `"union C tagged union", "discriminated union C", "C union vs struct"`
- **Resource:** `man 7 c99` or simply the C99 standard's union section — unions are often misunderstood; the man page is concise.
- **Resource:** [cppreference.com — Union declaration](https://en.cppreference.com/w/c/language/union) — free, well-organized, has examples.
- **Why:** Before you write the `JsonValue` struct, you must be confident about what a union is and what undefined behavior looks like when you access the wrong member.

---

### Linked Lists in C

- **Search:** `"singly linked list C implementation", "linked list malloc C", "prepend append linked list C"`
- **Resource:** [Stanford CS Education Library — Linked List Basics](http://cslibrary.stanford.edu/103/) — free PDF, purely focused on C linked lists with diagrams.
- **Why:** Your object and array nodes are linked lists. This guide covers every operation you need.

---

### JSON Specification

- **Resource:** [json.org](https://www.json.org) — the canonical, visual grammar diagram for JSON. Bookmark this. Every parsing question has its answer here.
- **Resource:** [RFC 8259](https://www.rfc-editor.org/rfc/rfc8259) — the actual JSON specification. Sections 6 (numbers) and 7 (strings) are where the tricky edge cases live.
- **Why:** When you're unsure whether `1.` or `007` is valid JSON, this is the authority.

---

### Recursive Descent Parsing

- **Search:** `"recursive descent parser theory", "recursive descent parsing JSON C"`
- **Resource:** [Crafting Interpreters](https://craftinginterpreters.com) (free online), Chapter 6 — explains recursive descent parsers in a beginner-friendly way. Ignore the Java — the concepts translate directly.
- **Why:** Chapter 6 gives you a crystal-clear mental model of how grammar rules become functions, how tokens are consumed, and how errors are detected. Read this early.

---

### `strtod` for Number Parsing

- **Search:** `"strtod C usage", "strtod vs atof C", "strtod error handling"`
- **Resource:** `man 3 strtod` on your terminal.
- **Why:** `atof` has no error detection. `strtod` lets you check whether the entire input was consumed and detect overflow. You need the man page to use it correctly.

---

### Valgrind and Memory Sanitizers

- **Search:** `"valgrind memcheck tutorial", "gcc -fsanitize=address usage"`
- **Resource:** [Valgrind Quick Start Guide](https://valgrind.org/docs/manual/quick-start.html) — official, short, sufficient.
- **Why:** You will have memory leaks. This tool finds them. Use it from Milestone 6 onward, but install it now.

---

> [!TIP]
> **Start with [json.org](https://www.json.org) and Chapter 6 of *Crafting Interpreters* before writing a single line of code.** These two resources alone will give you 80% of the mental model you need.

> [!IMPORTANT]
> The JSON spec at [json.org](https://www.json.org) uses railroad diagrams (visual grammar). Read them from left to right. Each path through the diagram is a valid input. If your parser rejects a valid path or accepts an invalid one, it has a bug.

> [!NOTE]
> You do not need to read any of these resources completely. For each milestone, return here, find the relevant resource, and read only the section that applies. Deep familiarity with the whole specification can come after the first working version.
