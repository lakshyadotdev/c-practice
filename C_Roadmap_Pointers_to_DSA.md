# C Roadmap: Pointers → Memory → Basic DSA (12–14 hrs)

**Assumes:** you're solid up to functions (loops, if/else, arrays basics, function calls).
**Goal:** be able to write real C — pointers, dynamic memory, strings, structs — and know the core data structures/algorithms well enough to build small projects and not get destroyed in an interview.

Format below: `[time] Topic — what to actually do — resource`

---

## Block 1 — Pointers Core (2.5 hrs)

**1. Pointer fundamentals (45 min)**
- What a pointer is, `&` and `*`, pointer types, NULL, pointer size vs data size, why pointers matter (pass-by-reference, efficiency, dynamic structures).
- Do: declare/print addresses, swap two ints using a pointer-taking function.
- Resource: [Beej's Guide to C — Pointers chapter](https://beej.us/guide/bgc/html/split/pointers.html) (best free explanation, period)

**2. Pointers + arrays (45 min)**
- Array-pointer equivalence (`arr` decays to `&arr[0]`), pointer arithmetic (`ptr+1` moves by `sizeof(type)`), indexing via pointers (`*(arr+i)` == `arr[i]`).
- Do: traverse and sum an array two ways (indexing vs pointer walking), write a function that takes `int *arr, int n`.
- Resource: [learn-c.org — Pointer Arithmetic](https://www.learn-c.org/en/Pointer_arithmetic)

**3. Pointers to pointers, const with pointers, function pointers (1 hr)**
- `int **`, why you need it (e.g. modifying a pointer inside a function), `const int *` vs `int * const`, basic function pointer syntax and one use case (callback / simple dispatch table).
- Do: write a function that reallocates/reassigns a caller's pointer using `int **`.
- Resource: [Beej's Guide — Pointers to Pointers](https://beej.us/guide/bgc/html/split/pointers.html#pointerstopointers) + [cdecl.org](https://cdecl.org/) (paste any gnarly pointer declaration to decode it — bookmark this)

---

## Block 2 — Arrays & Strings in Depth (2 hrs)

**4. Multi-dimensional arrays & array-of-pointers (30 min)**
- 2D array memory layout (row-major), `int arr[R][C]` vs `int **` (NOT the same thing — this trips everyone up), passing 2D arrays to functions.
- Resource: [Beej's Guide — Multidimensional Arrays](https://beej.us/guide/bgc/html/split/arrays.html)

**5. C strings (1 hr)**
- Strings = `char[]` + null terminator, `<string.h>` functions (`strlen`, `strcpy`, `strcat`, `strcmp`, `strchr`, `strstr`, `strtok`), why `char *s = "literal"` is read-only, buffer overflow risk (`strcpy` vs `strncpy`).
- Do: implement `my_strlen`, `my_strcpy`, `my_strcmp` yourself from scratch (this is the single best exercise for cementing pointers+strings).
- Resource: [learn-c.org — Strings](https://www.learn-c.org/en/Strings) + [cppreference string.h](https://en.cppreference.com/w/c/string/byte)

**6. Arrays of strings / command-line args (30 min)**
- `char *argv[]`, array of char pointers, `int main(int argc, char *argv[])`.
- Do: write a program that echoes its CLI args.

---

## Block 3 — Dynamic Memory (2.5 hrs)

**7. Stack vs heap, malloc/free (1 hr)**
- Where variables live, why dynamic memory is needed, `malloc`, `free`, checking for NULL, what happens if you forget `free` (leak) or use-after-free (dangling pointer) or double-free.
- Do: dynamically allocate an array whose size is read from user input, fill it, free it.
- Resource: [Beej's Guide — Memory Allocation](https://beej.us/guide/bgc/html/split/memory-allocation-functions.html)

**8. calloc, realloc, sizeof gotchas (45 min)**
- Difference from malloc, growing a dynamic array (this IS how you'll build a dynamic array/vector), common `sizeof` mistakes (`sizeof(ptr)` vs `sizeof(array)`).
- Do: build a simple growable "vector" (int array that doubles capacity via `realloc` when full) — this is a real, useful mini-project and sets up Block 4.
- Resource: [Beej's Guide — same page, calloc/realloc section](https://beej.us/guide/bgc/html/split/memory-allocation-functions.html)

**9. Structs + pointers to structs, `typedef` (45 min)**
- Struct basics, `->` operator, nested structs, why you almost always pass structs by pointer, `typedef struct {...} Name;`.
- Do: define a `Point` and a `Person` struct, write functions that take pointers to them and modify fields.
- Resource: [learn-c.org — Structs](https://www.learn-c.org/en/Structs)

**Checkpoint tool:** run everything through **Valgrind** (`valgrind --leak-check=full ./a.out`) once you hit this point — it will catch leaks/invalid access and massively speeds up learning correct memory habits. If on Windows, use WSL or Compiler Explorer/[onlinegdb.com](https://www.onlinegdb.com/) which has a memory checker too.

---

## Block 4 — Basic DSA in C (4.5–5.5 hrs)

This is "the important stuff" — not exhaustive, but enough to build real projects and cover interview basics.

**10. Recursion (30 min)** — factorial, fibonacci, and base-case/recursive-case thinking. Needed for trees/sorting later.
- Resource: [GeeksforGeeks — Recursion in C](https://www.geeksforgeeks.org/recursion/recursion-in-c/)

**11. Dynamic array / "vector" pattern (already built in step 8) — formalize it (15 min)**
- Wrap it as `struct { int *data; int size; int capacity; }` with `push`, `get`, `free_vector` functions. This is your first real reusable data structure.

**12. Linked Lists (1 hr)**
- Singly linked list: node struct (`data` + `next` pointer), insert at head/tail, delete a node, traverse, free the whole list (important — don't leak).
- Do: build one from scratch, implement insert/delete/print.
- Resource: [GeeksforGeeks — Linked List in C](https://www.geeksforgeeks.org/data-structures/linked-list/) + [VisuAlgo (visual)](https://visualgo.net/en/list)
- *(Optional if time: doubly linked list — same idea, +`prev` pointer, 15 min)*

**13. Stack & Queue (45 min)**
- Implement stack via array or linked list (push/pop/peek), queue via array or linked list (enqueue/dequeue). Understand where each is used (stack: undo/parsing/recursion simulation; queue: BFS/scheduling).
- Resource: [GeeksforGeeks — Stack](https://www.geeksforgeeks.org/dsa/stack-data-structure/), [Queue](https://www.geeksforgeeks.org/dsa/queue-data-structure/)

**14. Searching & Sorting (1 hr)**
- Linear search, binary search (needs sorted array — understand why), bubble/selection sort (for intuition), then **quicksort or mergesort** (the ones that actually matter — understand divide & conquer + Big-O).
- Do: implement binary search + one O(n log n) sort by hand.
- Resource: [Beej's Guide has a sorting example](https://beej.us/guide/bgc/) / [VisuAlgo — Sorting](https://visualgo.net/en/sorting) (great for visual intuition) / [GeeksforGeeks Big-O primer](https://www.geeksforgeeks.org/dsa/analysis-of-algorithms-set-1-asymptotic-analysis/)

**15. Hashing basics (30–45 min)**
- What a hash table is and why O(1) average lookup, simple hash function (mod-based), collision handling via chaining (linked list per bucket — ties directly back to what you just learned).
- Do: implement a tiny hash table of fixed size storing `int` keys with chaining.
- Resource: [GeeksforGeeks — Hashing](https://www.geeksforgeeks.org/dsa/hashing-data-structure/)

**16. Binary Trees (optional, 45 min if time allows)**
- Node struct with left/right pointers, insert, in-order traversal (recursive). Just enough to recognize the pattern — full trees/graphs are a "later" topic.
- Resource: [GeeksforGeeks — Binary Tree](https://www.geeksforgeeks.org/dsa/binary-tree-data-structure/)

---

## Block 5 — Tie It Together (30–60 min)

- Skim: **compilation stages** (preprocess → compile → assemble → link), **header files + `#include` guards**, splitting code into `.c`/`.h` files, basic `Makefile` or `gcc file1.c file2.c -o out`.
- Resource: [Beej's Guide — Multi-file projects section](https://beej.us/guide/bgc/html/split/more-esoteric-topics.html)
- Why: your first "decent project" will need this the moment it's more than one file.

---

## Project ideas once you finish (pick 1–2)
1. **Dynamic array-based To-Do list** with add/remove/save-to-file (uses malloc/realloc + strings + file I/O — look up `fopen`/`fread`/`fwrite` briefly, it's small).
2. **Custom string library** — your own `mystring.h` with the functions from step 5 plus a growable string type.
3. **Simple linked-list-based student/employee database** with struct records, CRUD ops.
4. **Text-based inventory or contact manager** using a hash table for fast lookup by name/ID.
5. **A small custom memory allocator** (advanced/stretch) — reimplement a toy `malloc`/`free` using a free list. Great deep-dive if you want to go further after this roadmap.

## Reference tools to keep open while working
- [cdecl.org](https://cdecl.org/) — decode/build pointer declarations
- [Compiler Explorer](https://godbolt.org/) or [onlinegdb.com](https://www.onlinegdb.com/) — quick compile/run/debug in browser
- [VisuAlgo](https://visualgo.net/) — visualize data structures & algorithms
- `valgrind --leak-check=full ./a.out` — catch memory bugs (Linux/WSL/Mac)
- [cppreference.com C section](https://en.cppreference.com/w/c) — the actual standard library reference, faster than most tutorials once you know what you're looking for

---

### Suggested time split (14 hr version)
| Block | Hours |
|---|---|
| 1. Pointers core | 2.5 |
| 2. Arrays & strings | 2 |
| 3. Dynamic memory + structs | 2.5 |
| 4. Basic DSA | 5.5 |
| 5. Multi-file/build basics | 0.5 |
| Buffer/review | 1 |

Cut Block 4's trees section and the buffer if you need to land at 12 hrs sharp.
