##### 1. What it is and why it exists

A linked list is a data structure where elements are stored in individual dynamically allocated structs, each containing a pointer to the next struct in the sequence. The sequence is terminated by a null pointer, and the entire list is tracked via a single head pointer. It exists to provide a mechanism for $O(1)$ insertions and deletions at specific points (like the front of a list) without needing to copy or shift surrounding elements as an array would require.

##### 2. Mental model

```text
Head Pointer          List Elements (Heap-Allocated)
+-------+            +-------+------+      +-------+------+      +-------+------+
| head  | ---------> | val:2 | next | ---> | val:7 | next | ---> | val:0 | next | ---> NULL
+-------+            +-------+------+      +-------+------+      +-------+------+

```

*(Note: The source provides graphical node box diagrams; this ASCII representation adapts that structural model [extra].)*

##### 3. Structure and invariants

A basic singly-linked list requires a node struct and a head pointer.

* `next`: A pointer within each element pointing to the next node in the list, or null if it is the last element.

* `value`: The contents stored in the element.

* `head`: A pointer to the first element of the list, which serves as the main entry point (often passed as a pointer-to-pointer when modifying the list).

**Invariants:**

* The list must always be terminated by a null pointer to indicate no more elements.

* The head pointer must point to the valid first element, or be null if the list is empty.

* If implementing a queue, a separate tail pointer must accurately point to the very last node in the chain.

##### 4. Operations

| Operation | Idea | Time Complexity |
| --- | --- | --- |
| **Insert Front (Push)** | Allocate a new node, point its next to the current head, update head pointer.

 | O(1)

 |
| **Remove Front (Pop)** | Save the head pointer, update head to the second element, free the saved pointer.

 | O(1)

 |
| **Search / Iterate** | Follow the `next` pointers sequentially from the head until the element is found.

 | O(N)

 |
| **Insert Tail (Enqueue)** | Point the old tail's next to a new node, then update the tail pointer.

 | O(1) (if tracking tail)

 |

##### 5. Pseudocode

```text
// 1. Insert Front (Stack Push)
function list_push(head_ptr, value):
    new_node = allocate(sizeof node_struct)
    assert new_node is not null
    
    new_node.value = value
    new_node.next = dereference(head_ptr)
    dereference(head_ptr) = new_node

// 2. Remove Front (Stack Pop)
function list_pop(head_ptr):
    // Check for empty list
    assert dereference(head_ptr) is not null
    
    first_node = dereference(head_ptr)
    return_value = first_node.value
    
    // Patch out the first element
    dereference(head_ptr) = first_node.next
    free(first_node)
    
    return return_value

// 3. Search / Iterate
function list_print(head):
    current = head
    while current is not null:
        print(current.value)
        current = current.next

// 4. Insert Tail (Queue style)
function list_enqueue(head_ptr, tail_ptr, value):
    new_node = allocate(sizeof node_struct)
    assert new_node is not null
    
    new_node.value = value
    new_node.next = null
    
    if dereference(head_ptr) is null:
        dereference(head_ptr) = new_node
    else:
        current_tail = dereference(tail_ptr)
        current_tail.next = new_node
        
    dereference(tail_ptr) = new_node

```

##### 6. Why it works

Linked lists achieve constant $O(1)$ time complexity for insertions and deletions at known locations because these operations only require allocating a single new node and updating a constant number of localized pointers. Unlike arrays, there is no need to copy, shift, or resize a monolithic block of memory to accommodate the new data.

##### 7. Pitfalls

* **Double pointer confusion:** Failing to pass the head pointer by reference (as a pointer-to-pointer) to functions that modify the front of the list, meaning the caller's head pointer is never updated when the list changes [extra].
* **Use-after-free:** Accessing the `next` pointer of a node after passing that node to `free`, a mistake commonly made when iterating through a list to destroy it.

* **Losing the chain:** Reassigning a node's `next` pointer or the head pointer before saving the old reference, completely severing access to the rest of the list [extra].
* **Null pointer dereferences:** Failing to check if the list is empty (e.g., inside an enqueue operation or before a pop) before attempting to read or modify node fields, resulting in a crash.

* **Memory leaks:** Dropping the head pointer without traversing the list and individually `free`ing every single node, abandoning the allocated blocks on the heap [extra].

##### 8. Use it when / avoid it when

**Use it when:** You need to rapidly insert or delete elements right next to an element you already have a pointer to, or when implementing strict $O(1)$ structures like stacks and queues.
**Avoid it when:** Your application requires random access to elements by index, as reaching an arbitrary element takes $O(N)$ time, or when inserting strictly at the end where a dynamic array might offer better performance.

##### 9. Implementation checklist

* `listPush(head_ptr, value)`: Allocates a new node and inserts it at the front of the list.
* `listPop(head_ptr)`: Removes the front node, patches the head pointer, frees the node, and returns its value.
* `listPrint(head)`: Iterates over the entire list to process or print each value.
* `listDestroy(head_ptr)`: Traverses the list safely (saving the `next` pointer before freeing the current node) to free all allocated memory.

**3 Test Cases:**

1. **Empty List Pop:** Attempt to pop from an empty list (null head pointer) to verify that your code catches the empty state via an assertion rather than triggering a segmentation fault.
2. **Push and Pop Consistency:** Push elements 1 through 5, then pop them all, verifying they emerge in reverse order (5 down to 1) and that the head pointer correctly returns to null.
3. **Iteration Integrity:** Push 3 items and run the print loop to ensure it successfully reads all three elements and stops exactly at the null pointer without overrunning the list.

##### 10. Connections

* **Stacks:** Easily implemented as a singly-linked list where all pushes and pops occur at the head in $O(1)$ time.

* **Queues:** Built on a linked list by maintaining a pointer to both the head and the tail, allowing $O(1)$ operations at both ends.

* **Deques:** Implemented using a doubly-linked list (often circular with a dummy node) so that elements can be added or removed from either end in $O(1)$ time.

* **Hash Tables (Chaining):** Use arrays of linked list pointers to store multiple collision elements that hash to the same bucket.
