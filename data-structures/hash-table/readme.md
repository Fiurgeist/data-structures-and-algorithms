### Hash Table

Hash Tables (aka assoziative arrays/hash maps/dictionaries/...) use a hash value (some interger) generated from the key to store a value.
Since different keys can have the same hash, tables have to handle these collisions. There are generally two ways to do that:
- Open Addressing: which is used here, where the data is stored in the buckets directly and if there is a collision the next free bucket is used instead.
- Separate Chaining: where each bucket is a linked list, and if there are collision the value is just inserted into the list.
