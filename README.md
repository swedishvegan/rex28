# rex28
header-only regex engine in only 28 lines of cursed c++

cool features:
 - regex match and search, with O(string_length $\times$ num_states) polynomial time guarantee
 - easy to use, lightweight, no dependencies, header-only
 - just don't look inside the code...

usage:
```
// basic matching

rex variable_identifier("(a-z|A-Z|_)(a-z|A-Z|0-9|_)*");

variable_identifier.match("varName123"); // returns true
variable_identifier.match("not a variable"); // returns false

// basic search

rex example("(a*|0-9b*)+");
const char* search_string = "___aa2b3___";

example.first(search_string); // finds position and length of the first match, returns whether the search was a success

example.pos; // equals 3
example.len; // equals 5

// iterative search

rex apples("I|like|apples|and|oranges|!");
const char* sentence = "I do NOT like apples and oranges! They are disgusting...";

while (apples.next(sentence)) printf("%.*s ", apples.len, sentence + apples.pos); // prints out "I like apples and oranges! "

apples.reset(); // Rewinds the search iteration to the beginning

```
