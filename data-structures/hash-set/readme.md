### Hash Set

Hash Sets are basically the same as Hash Tables just instead of having key-value entries is has just values (or keys, depending on how you see it, since those are the unique things in a Table).
In fact, in most std library implementations (e.g. C++ or Java), the Hash Sets are just glorified wrappers around a Hash Table.

#### C impls

Instead of just wrapping a Hash Table, I wanted to try a independed implementation that doesn't waste the additional unused memory. But not having key-value entries makes it harder to store the information about tombstones.
There are 2 different approches to the problem, they are not nessecarly beautiful but ...work, I guess there's a reason why it's offten just a wrapper :D.
