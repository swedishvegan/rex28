# rex19
header-only regex engine in only 19 lines of cursed c++

cool features:
 - regex match and search, with O(string_length $\times$ num_states) polynomial time guarantee for both
 - easy to use, lightweight, no dependencies, header-only
 - just don't look inside the code...

usage:
```cpp
// basic matching

rex variable_identifier("(a-z|A-Z|_)(a-z|A-Z|0-9|_)*");

variable_identifier.match("varName123"); // returns true
variable_identifier.match("not a variable"); // returns false
variable_identifier.match("varName, varName2", 7); // returns true; only checks the first 7 characters

// greedy matching, useful for writing lexers

rex keyword("keyword");

keyword.matchbeg("keyword is present"); // returns 7, the length of the first match
keyword.matchbeg("does not start with keyword"); // returns -1 since there is no match starting from the first letter

// basic search

rex example("(a*|0-9b*)+");
const char* search_string = "___aa2b3___";

example.first(search_string); // finds position and length of the first match, returns whether the search was a success

example.pos; // equals 3
example.len; // equals 5

// iterative search

rex apples("I|like|apples|and|oranges|!");
const char* sentence = "I do NOT like apples and oranges! They are disgusting...";
while (apples.next(sentence)) printf("%.*s ", apples.len, sentence + apples.pos); // prints out "I like apples and oranges ! "

apples.reset(); // rewinds the search iteration to the beginning
// The first and next functions accept an optional length argument like match

// defining variables

rex animals("cat|dog", "animal"); // can be referenced in other regex using the "animal" tag
rex pets("I have a pet {animal}");

pets.match("I have a pet dog"); // returns true
pets.match("I have a pet walrus"); // returns false

```
