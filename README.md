This project works as a demonstration of multithread key-value store. In a simplification it is a light, Redis-like structure.

When running the program, it uses the code written in kvstore (both .h and .cpp) and ttl_worker (both .h and .cpp).

It shows, from top line to the bottom:

→basic key insertion (first three lines)

→key deletion (fourth and fifth line)

→temporary keys (sixth, seventh, and eight line)

→Next section presents timed keys, and purging.

→Last section shows the program working in the background on 8000 concurrent operations.

[All code sections of each of those points can be easily found via comments]