### 1. What it is and why it exists

Standard C arrays have a fixed size that must be known when they are declared or allocated. A dynamic array is a data structure that wraps a standard array and includes enough tracking information to grow or shrink automatically as needed. It exists to provide the contiguous-memory performance benefits of standard arrays without locking the programmer into a rigid, upfront size constraint, achieving this through the careful use of the `realloc` function.

### 2. Mental model

*Note: The source text does not provide a memory diagram for this structure, so the following standard model is provided [extra].*

```text
Dynamic Array Struct
+------------------+
| capacity: 8      |
| count:    5      |
| data:     *------+
+------------------+
                   |
                   v  Heap-Allocated Data Block
                   +----+----+----+----+----+----+----+----+
                   | 10 | 22 | 34 | 41 | 55 | ?? | ?? | ?? |
                   +----+----+----+----+----+----+----+----+
                     0    1    2    3    4    5    6    7

```

### 3. Structure and invariants

The source notes that the structure must include "enough information beyond the contents of the array itself to allow resizing," but skips the exact field definitions. A standard implementation requires three fields [extra]:

* `data`: A pointer to the dynamically allocated memory block [extra].
* `capacity`: The total number of elements the current memory block can hold [extra].
* `count`: The number of elements actually in use by the program [extra].

**Invariants:**

* `count <= capacity` must hold at all times [extra].
* The `data` pointer must point to a valid heap block of exactly `capacity * sizeof(element)` bytes, or be null if `capacity` is 0 [extra].

### 4. Operations

Note: The source mentions resizing via `realloc` and its amortized cost, but skips the explicit operation list. The table below outlines the standard operations [extra].

| Operation | Idea | Time Complexity |
| --- | --- | --- |
| **Create** | Allocate the struct and an initial `data` block. | O(1) [extra] |
| **Get / Set** | Access or modify the element at a specific index. | O(1) [extra] |
| **Append** | Insert at the end; if `count == capacity`, expand the block. | Amortized O(1), Worst O(n)

 |
| **Destroy** | Free the `data` block, then free the struct. | O(1) [extra] |

### 5. Pseudocode

*Note: The source text does not provide the code for the dynamic array, linking only to an external `array.c` file. The following is standard C-style pseudocode [extra].*

```text
// 1. Create
function array_create(initial_capacity):
    array = allocate(sizeof(DynamicArray))
    if array is null: return null
    
    array.capacity = initial_capacity
    array.count = 0
    array.data = allocate(initial_capacity * sizeof(Element))
    
    if array.data is null:
        free(array)
        return null
        
    return array

// 2. Expand (Internal helper)
function array_expand(array):
    new_capacity = array.capacity * 2
    if new_capacity == 0: new_capacity = 1
    
    // Careful use of realloc to avoid memory leaks on failure
    new_data = realloc(array.data, new_capacity * sizeof(Element))
    if new_data is null:
        return FAILURE // Original array.data is left intact
        
    array.data = new_data
    array.capacity = new_capacity
    return SUCCESS

// 3. Append
function array_append(array, value):
    if array.count == array.capacity:
        status = array_expand(array)
        if status == FAILURE: return FAILURE
        
    array.data[array.count] = value
    array.count = array.count + 1
    return SUCCESS

// 4. Get
function array_get(array, index):
    if index < 0 or index >= array.count:
        return OUT_OF_BOUNDS_ERROR
    return array.data[index]

// 5. Destroy
function array_destroy(array):
    if array is not null:
        free(array.data)
        free(array)

```

### 6. Why it works

Using an array allows for fast access, and pre-allocating space is almost always faster than allocating memory exactly as needed. While expanding the array via `realloc` is an expensive O(n) operation in the worst case because elements may need to be copied, doubling the capacity ensures this copy operation happens infrequently. Consequently, the heavy cost is distributed across many cheap insertions, resulting in an amortized cost that remains small.

### 7. Pitfalls

*Note: The source is vague on dynamic array pitfalls, but outlines general memory issues. The following applies standard C pitfalls to this structure [extra].*

1. **Orphaning the struct:** Calling `free(array)` without first calling `free(array->data)`, resulting in a memory leak of the internal buffer [extra].
2. **`realloc` overwrite leak:** Assigning the result of `realloc` directly back to `array->data`. If `realloc` fails and returns null, the original pointer is overwritten, meaning the old block can no longer be freed [extra].
3. **Dangling pointers:** Storing a direct pointer to an element inside the dynamic array (e.g., `int* p = &array->data[2]`). If a subsequent append triggers a `realloc`, the entire block might move to a new memory address, leaving `p` pointing to invalid memory [extra].
4. **Buffer overflows:** Forgetting to update the `count` property after a manual insertion, or failing to check `index >= array.count` before a read [extra].

### 8. Use it when / avoid it when

**Use it when:** You need fast O(1) index access, excellent cache locality, and a structure that mostly grows at the end, but you do not know the maximum capacity required at compile time [extra].
**Avoid it when:** You need to frequently insert or delete elements in the middle or at the front of the sequence. For those operations, shifting the contiguous elements takes O(n) time, making linked lists or trees a better choice.

### 9. Implementation checklist

Note: Checklist derived from standard practices [extra], leveraging `valgrind` as emphasized by the source.

**Functions to write:**

* `array_create(capacity)`: Allocates struct and data block, initializing fields.
* `array_destroy(array)`: Safely frees the data block and then the struct.
* `array_append(array, value)`: Checks capacity, resizes if needed via doubling, and adds the element.
* `array_get(array, index)`: Retrieves an element with bounds checking.

**Test cases to run:**

1. **Trigger Resize:** Append elements exceeding the initial capacity to verify `realloc` correctly moves data without corruption.
2. **Bounds Protection:** Attempt to get an element at `count + 5` to ensure the program catches the out-of-bounds access instead of segfaulting.
3. **Valgrind Verification:** Create an array, force multiple reallocations, destroy the array, and run the program through `valgrind` to ensure it reports "0 bytes definitely lost".

### 10. Connections

* **Stacks:** When the elements are small, a stack is best built out of an array (tracking the index of the top element), which allows pushes and pops to just update the pointer without frequent `malloc` calls.

* **Packed Heaps:** A binary heap is typically stored implicitly in an array without pointer gaps, reading through the tree in breadth-first search order. Because insertions happen at the end, it relies on this underlying array to expand dynamically when needed.
