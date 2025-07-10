#include "rex19.hpp"
#include <iostream>

void test(rex& r, const char* s, bool expected, bool reverse = false) {
    bool actual = r.match(s, -1, reverse);
    if (actual != expected)
        std::cout << "\t" << s << " failed: got " << actual
                  << ", expected " << expected << " on " << s << '\n';
}

void testsearch(rex& r, const char* s, int start, unsigned int len) {
    r.next(s);
    if (start != r.pos || len != r.len)
        std::cout << "\t" << s << " failed: got (" << r.pos << ',' << r.len
                  << "), expected (" << start << ',' << len << ") on " << s << "\n";
}

/* ─────────────────────────── regular match tests ────────────────────────── */
void run_regular_tests() {
    {
        const char* p = "ab*c?d";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "a", false);
        test(r, "ad", true);
        test(r, "acd", true);
        test(r, "aabcd", false);
        test(r, "abbbbbd", true);
        test(r, "abc", false);
    }
    {
        const char* p = "0-9a-z*";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "3q", true);
        test(r, "55z", false);
        test(r, "9A", false);
        test(r, "7", true);
    }
    {
        const char* p = "x+y*";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "x", true);
        test(r, "xx", true);
        test(r, "xyy", true);
        test(r, "yyyy", false);
    }
    {
        const char* p = "1*23|4+";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "11123", true);
        test(r, "234", false);
        test(r, "44", true);
        test(r, "23", true);
    }
    {
        const char* p = "(a-z|A-Z|_)(a-z|A-Z|0-9|_)*";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "varName", true);
        test(r, "123bad", false);
        test(r, "_", true);
        test(r, "lololHAHA33", true);
        test(r, "abc def", false);
        test(r, "varName+otherVarName", false);
    }
    {
        const char* p = "(a|ab)*b";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "b", true);
        test(r, "ab", true);
        test(r, "abb", true);
        test(r, "aab", true);
        test(r, "aaab", true);
        test(r, "aaba", false);
    }
    {
        const char* p = "a?aa";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "aa", true);
        test(r, "aaa", true);
        test(r, "a", false);
    }
    {
        const char* p = "(a|b)+c";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "c", false);
        test(r, "ac", true);
        test(r, "bc", true);
        test(r, "abc", true);
        test(r, "aabbc", true);
    }
    {
        const char* p = "a*b*a";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "a", true);
        test(r, "ba", true);
        test(r, "aa", true);
        test(r, "bba", true);
        test(r, "bbb", false);
    }
    {
        const char* p = "(x|y)*(x|y)";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "", false);
        test(r, "x", true);
        test(r, "y", true);
        test(r, "xy", true);
        test(r, "yy", true);
        test(r, "yxy", true);
    }
    {
        const char* p = "(a|b|c)*";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "", true);
        test(r, "abcabc", true);
        test(r, "abcd", false);
    }
    {
        const char* p = "(ab)*c";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "c", true);
        test(r, "abc", true);
        test(r, "ababc", true);
        test(r, "abab", false);
    }
    {
        const char* p = "a?b?c?";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "", true);
        test(r, "a", true);
        test(r, "b", true);
        test(r, "c", true);
        test(r, "ab", true);
        test(r, "ac", true);
        test(r, "bc", true);
        test(r, "abc", true);
        test(r, "abcc", false);
    }
    {
        const char* p = "((a|b)c)*d";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "d", true);
        test(r, "acd", true);
        test(r, "bcd", true);
        test(r, "acbcd", true);
        test(r, "acbc", false);
    }
    {
        const char* p = "(a|b|c)+";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "", false);
        test(r, "a", true);
        test(r, "b", true);
        test(r, "c", true);
        test(r, "abc", true);
        test(r, "abccba", true);
        test(r, "abcd", false);
    }
    {
        const char* p = "a*bc";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "bc", true);
        test(r, "abc", true);
        test(r, "aabc", true);
        test(r, "c", false);
    }
    {
        const char* p = "(ab)*c";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "c", true);
        test(r, "abc", true);
        test(r, "ababc", true);
        test(r, "ab", false);
    }
    {
        const char* p = "(a*)*b";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "b", true);
        test(r, "ab", true);
        test(r, "aaab", true);
    }
    {
        const char* p = "(a?)*b";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "b", true);
        test(r, "ab", true);
        test(r, "aab", true);
    }
    {
        const char* p = "a*a*";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "", true);
        test(r, "a", true);
        test(r, "aa", true);
    }
    {
        const char* p = "a*";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "b", false);
    }
    {
        const char* p = "ab*";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "a", true);
        test(r, "ab", true);
        test(r, "abb", true);
        test(r, "b", false);
    }
    {
        const char* p = "b(a*a)";
        rex r(p);
        std::cout << "Testing: " << p << '\n';
        test(r, "ba", true);
        test(r, "baa", true);
        test(r, "ab", false);
    }
}

/* ─────────────────────────── reverse-match tests ────────────────────────── */
void run_reverse_tests() {
    {
        const char* p = "abc";
        rex r(p);
        std::cout << "Testing reversed: " << p << '\n';
        test(r, "abc", false, true);
        test(r, "cba", true, true);
        test(r, "cb", false, true);
    }
    {
        const char* p = "aa*b";
        rex r(p);
        std::cout << "Testing reversed: " << p << '\n';
        test(r, "ba", true, true);
        test(r, "ab", false, true);
    }
    {
        const char* p = "a+(b|c*)d|ef";
        rex r(p);
        std::cout << "Testing reversed: " << p << '\n';
        test(r, "fe", true, true);
        test(r, "f", false, true);
        test(r, "db", false, true);
        test(r, "dccccaa", true, true);
        test(r, "ccca", false, true);
        test(r, "da", true, true);
        test(r, "dba", true, true);
    }
    {
        const char* p = "xyz";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "zyx", true, true);
        test(r, "xyz", false, true);
        test(r, "yx", false, true);
    }

    {
        const char* p = "a+b*";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "aa", true, true);
        test(r, "aab", false, true);
        test(r, "ba", true, true);
        test(r, "b", false, true);
    }

    {
        const char* p = "(a|b)*c";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "cba", true, true);
        test(r, "c", true, true);
        test(r, "cbx", false, true);
    }

    {
        const char* p = "(ab)*c";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "c", true, true);
        test(r, "bac", false, true);
        test(r, "cbababa", true, true);
    }

    {
        const char* p = "a?b+c?";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "cbb", true, true);
        test(r, "bbc", false, true);
        test(r, "bb", true, true);
        test(r, "b", true, true);
        test(r, "cb", true, true);
    }

    {
        const char* p = "(x|y)*z";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "zyx", true, true);
        test(r, "zyyyy", true, true);
        test(r, "z", true, true);
        test(r, "xyz", false, true);
    }

    {
        const char* p = "(a*)*b";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "baaa", true, true);
        test(r, "b", true, true);
        test(r, "ab", false, true);
        test(r, "aa", false, true);
    }

    {
        const char* p = "a?b?c?";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "abc", false, true);
        test(r, "cba", true, true);
        test(r, "cb", true, true);
        test(r, "a", true, true);
        test(r, "d", false, true);
    }

    {
        const char* p = "a*b*a";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "abb", true, true);
        test(r, "ab", true, true);     
        test(r, "baaa", false, true);
        test(r, "aaa", true, true);
    }

    {
        const char* p = "(a|b)+c";
        rex r(p);
        std::cout << "Testing reversed: " << p << "\n";
        test(r, "cb", true, true);
        test(r, "cba", true, true);
        test(r, "c", false, true);
        test(r, "dcba", false, true);
    }
}

/* ───────────────────────────── search tests ─────────────────────────────── */
void run_search_tests() {
    {
        const char* p = "abcd";
        rex r(p);
        std::cout << "Testing search: " << p << '\n';
        testsearch(r, "aaaabcddddd", 3, 4);
        testsearch(r, "aaaabcddddd", -1, 0);
        testsearch(r, "abcdabcd", 0, 4);
        testsearch(r, "abcdabcd", 4, 4);
    }
    {
        const char* p = "a+\1-\255*b+|XY";
        rex r(p);
        std::cout << "Testing search: " << p << '\n';
        const char* s = "__aaXYbb__XY__XY";
        testsearch(r, s, 2, 6);
        testsearch(r, s, 10, 2);
        testsearch(r, s, 14, 2);
        testsearch(r, s, -1, 0);
    }
    {
        const char* p = "I|ate|an|apple.";
        rex r(p);
        std::cout << "Testing search: " << p << '\n';
        const char* spaced = "I ate an apple. It was delicious!";
        testsearch(r, spaced, 0, 1);
        testsearch(r, spaced, 2, 3);
        testsearch(r, spaced, 6, 2);
        testsearch(r, spaced, 9, 6);
        testsearch(r, spaced, 16, 1);
        testsearch(r, spaced, -1, 0);

        const char* tight = "Iateanapple.Itwasdelicious!";
        testsearch(r, tight, 0, 1);
        testsearch(r, tight, 1, 3);
        testsearch(r, tight, 4, 2);
        testsearch(r, tight, 6, 6);
        testsearch(r, tight, 12, 1);
        testsearch(r, tight, -1, 0);
    }
    {
        const char* p = "dog|dogs";
        rex r(p);
        std::cout << "Testing search: " << p << '\n';
        testsearch(r, "I like dogs and cats", 7, 4);
        r.reset();
        testsearch(r, "A dog is loyal", 2, 3);
        r.reset();
        testsearch(r, "The hotdog vendor", 7, 3);
        r.reset();
        testsearch(r, "The cat purrs", -1, 0);
    }

    {
        const char* p = "(ha)+!";
        rex r(p);
        std::cout << "Testing search: " << p << '\n';
        testsearch(r, "haha! wow", 0, 5);
        r.reset();
        testsearch(r, "ha!ha!ha!", 0, 3);
        testsearch(r, "ha!ha!h1!", 3, 3);
        r.reset();
        testsearch(r, "heh!", -1, 0);
    }

    {
        const char* p = "\\[.*\\]";
        rex r(p);
        std::cout << "Testing search: " << p << '\n';
        testsearch(r, "data [123] test [abc]", 5, 16);
        r.reset();
        testsearch(r, "[outer [inner]]", 0, 15);
        r.reset();
        testsearch(r, "no brackets here", -1, 0);
    }

    {
        const char* p = "(a|b)*c";
        rex r(p);
        std::cout << "Testing search: " << p << '\n';
        testsearch(r, "xxabababczzz", 2, 7);
        r.reset();
        testsearch(r, "abcabc", 0, 3);
        r.reset();
        testsearch(r, "xyz", -1, 0);
    }

    {
        const char* p = "z+";
        rex r(p);
        std::cout << "Testing search: " << p << '\n';
        testsearch(r, "aaazz", 3, 2);
        r.reset();
        testsearch(r, "zzzz", 0, 4);
        r.reset();
        testsearch(r, "zzabczz", 0, 2);
        r.reset();
        testsearch(r, "abc", -1, 0);
    }

    {
        const char* p = "(cat|dog)+";
        rex r(p);
        std::cout << "Testing search: " << p << '\n';
        testsearch(r, "catdogcatmeow", 0, 9);
        r.reset();
        testsearch(r, "woofdogcat", 4, 6);
        r.reset();
        testsearch(r, "mouse", -1, 0);
    }
}

/* ───────────────────────────── parser tests ─────────────────────────────── */
void run_parser_tests() {
    {
        const char* p = "\\\\a-z*\\(0-9*\\)";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "\\a(100)", true);
        test(r, "\\hello(6969)", true);
        test(r, "\\h()", true);
        test(r, "\\(2)", true);
        test(r, "hello(222)", false);
        test(r, "\\hello222", false);
    }
    {
        const char* p = "\\\\";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "\\",  true);
        test(r, "",    false);
        test(r, "\\\\", false);
    }
    {
        const char* p = "\\q-\\z+";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "q",    true);
        test(r, "t",    true);
        test(r, "z",    true);
        test(r, "qqz",  true);
        test(r, "p",    false);
        test(r, "{",    false);
    }
    {
        const char* p = "a-\\z+";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "a",  true);
        test(r, "m",  true);
        test(r, "z",  true);
        test(r, "aa", true);
        test(r, "{",  false);
    }
    {
        const char* p = "ab\\\\";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "ab\\",  true);
        test(r, "ab",    false);
        test(r, "ab\\\\", false);
    }
    {
        const char* p = "\\(";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "(",  true);
        test(r, "a",  false);
        test(r, "\\", false);
    }
    {
        const char* p = "\\*-\\+";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "*", true);
        test(r, "+", true);
        test(r, ",", false);
    }
    {
        const char* p = "m-\\q+";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "m",     true);
        test(r, "o",     true);
        test(r, "p",     true);
        test(r, "q",     true);
        test(r, "r",     false);
        test(r, "mnopq", true);
    }
    {
        const char* p = "\\+";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "+",  true);
        test(r, "++", false);
    }
    {
        const char* p = "\\*a\\+";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "*a+", true);
        test(r, "*a", false);
        test(r, "a+", false);
    }

    {
        const char* p = "\\(a\\|b\\)";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "(a|b)", true);
        test(r, "a|b", false);
        test(r, "(a|c)", false);
    }

    {
        const char* p = "\\(\\)";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "()", true);
        test(r, "(", false);
        test(r, ")", false);
    }

    {
        const char* p = "\\\\\\\\*";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "\\", true);
        test(r, "\\\\", true);
        test(r, "\\\\\\", true);
        test(r, "", false);
    }

    {
        const char* p = "\\|";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "|", true);
        test(r, "\\", false);
        test(r, "", false);
    }

    {
        const char* p = "a\\?b";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "a?b", true);
        test(r, "ab", false);
        test(r, "a", false);
    }

    {
        const char* p = "a(b|c\\+)*d";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "ad", true);
        test(r, "abbd", true);
        test(r, "abc+bd", true);
        test(r, "abc+d", true);
        test(r, "ab+c", false);
    }

    {
        const char* p = "(a\\*)+";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "a*", true);
        test(r, "a*a*", true);
        test(r, "a", false);
        test(r, "*", false);
    }

    {
        const char* p = "\\)-\\}*";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, ")", true);
        test(r, "}", true);
        test(r, ")*+,-./", true);
        test(r, "~", false);
    }

    {
        const char* p = "\\a-\\z\\*";
        rex r(p);
        std::cout << "Testing parser: " << p << '\n';
        test(r, "a*", true);
        test(r, "z*", true);
        test(r, "m*", true);
        test(r, "a", false);
    }
}

/* ───────────────────────────── variable tests ───────────────────────────── */
void run_variable_tests() {
    {
        const char* p1 = "apples|oranges";
        const char* p2 = "I like to eat {fruit}";
        rex r1(p1, "fruit");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "I like to eat apples", true);
        test(r2, "I don't like to eat apples", false);
        test(r2, "I like to eat oranges", true);
        test(r2, "I like to eat bananas", false);
    }
    {
        const char* p1 = "(cat|dog|bird)";
        const char* p2 = "{animal}s are cute";
        rex r1(p1, "animal");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "cats are cute", true);
        test(r2, "dogs are cute", true);
        test(r2, "birds are cute", true);
        test(r2, "lizards are cute", false);
    }

    {
        const char* p1 = "foo|bar";
        const char* p2 = "I saw {thing} and then {thing}";
        rex r1(p1, "thing");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "I saw foo and then foo", true);
        test(r2, "I saw bar and then bar", true);
        test(r2, "I saw foo and then bar", true);
        test(r2, "I saw foo and then baz", false);
    }

    {
        const char* p1 = "(one|two)";
        const char* p2 = "(a|b){num}(x|y)";
        rex r1(p1, "num");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "aonex", true);
        test(r2, "btwoy", true);
        test(r2, "btwox", true);
        test(r2, "ctwoy", false);
    }

    {
        const char* p1 = "hello";
        const char* p2 = "{greeting}, world!";
        rex r1(p1, "greeting");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "hello, world!", true);
        test(r2, "hi, world!", false);
    }

    {
        const char* p1 = "a*";
        const char* p2 = "prefix{v}suffix";
        rex r1(p1, "v");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "prefixsuffix", true);
        test(r2, "prefixaasuffix", true);
        test(r2, "prefixaaaaaasuffix", true);
        test(r2, "prefixXsuffix", false);
    }

    {
        const char* p1 = "a-z+";
        const char* p2 = "{word}";
        rex r1(p1, "word");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "abc", true);
        test(r2, "z", true);
        test(r2, "Z", false);
    }

    {
        const char* p1 = "yes|no";
        const char* p2 = "answer: {choice}";
        rex r1(p1, "choice");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "answer: yes", true);
        test(r2, "answer: no", true);
        test(r2, "answer: maybe", false);
    }

    {
        const char* p1 = "1*";
        const char* p2 = "value: {digits}";
        rex r1(p1, "digits");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "value: ", true);
        test(r2, "value: 1", true);
        test(r2, "value: 1111", true);
        test(r2, "value: 2", false);
    }

    {
        const char* p1 = "\\(|\\)";
        const char* p2 = "match {paren}";
        rex r1(p1, "paren");
        rex r2(p2);
        std::cout << "Testing variables: " << p1 << '\n';
        test(r2, "match (", true);
        test(r2, "match )", true);
        test(r2, "match []", false);
    }
}

/* ───────────────────────────── api tests ────────────────────────────────── */
void run_api_tests() {
    {
        const char* p = "a+";
        rex r(p);
        std::cout << "Testing full api\n";
        const char* s = "aaaaabbb";
        if (!r.match(s, 5)) std::cout << "\tr.match positive test failed\n";
        if (r.match(s, 6)) std::cout << "\tr.match negative test failed\n";
        if (r.matchbeg(s) != 1) std::cout << "\tr.matchbeg positive test failed\n";
        if (r.matchbeg(s+5) != -1) std::cout << "\tr.matchbeg negative test failed\n";
    }
}

/* ────────────────────────────────── main ────────────────────────────────── */
int main() {
    run_regular_tests();
    run_reverse_tests();
    run_search_tests();
    run_parser_tests();
    run_variable_tests();
    run_api_tests();
    return 0;

}
