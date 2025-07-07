#include "rex28.hpp"
#include <iostream>

void test(rex& r, const char* s, bool expected, bool reverse = false) {
    bool actual = r.match(s, reverse);
    if (actual != expected) std::cout << "\t" << s << " failed test: got " << actual << ", expected " << expected << "\n";
}

void testsearch(rex& r, const char* s, int start, unsigned int len) {
    r.next(s);
    if (start != r.pos || len != r.len) std::cout << "\t" << s << " failed test: got (" << r.pos << "," << r.len << "), expected (" << start << "," << len << ")\n";  
}

int main() {

    const char* p1 = "ab*c?d";
    rex r1(p1);
    std::cout << "Testing: " << p1 << "\n";
    test(r1, "a", false);
    test(r1, "ad", true);
    test(r1, "acd", true);
    test(r1, "aabcd", false);
    test(r1, "abbbbbd", true);
    test(r1, "abc", false);

    const char* p2 = "0-9a-z*";
    rex r2(p2);
    std::cout << "Testing: " << p2 << "\n";
    test(r2, "3q", true);
    test(r2, "55z", false);
    test(r2, "9A", false);
    test(r2, "7", true);

    const char* p3 = "x+y*";
    rex r3(p3);
    std::cout << "Testing: " << p3 << "\n";
    test(r3, "x", true);
    test(r3, "xx", true);
    test(r3, "xyy", true);
    test(r3, "yyyy", false);

    const char* p4 = "1*23|4+";
    rex r4(p4);
    std::cout << "Testing: " << p4 << "\n";
    test(r4, "11123", true);
    test(r4, "234", false);
    test(r4, "44", true);
    test(r4, "23", true);

    const char* p5 = "(a-z|A-Z|_)(a-z|A-Z|0-9|_)*";
    rex r5(p5);
    std::cout << "Testing: " << p5 << "\n";
    test(r5, "varName", true);
    test(r5, "123bad", false);
    test(r5, "_", true);
    test(r5, "lololHAHA33", true);
    test(r5, "abc def", false);
    test(r5, "varName+otherVarName", false);

        const char* p6 = "(a|ab)*b";
    rex r6(p6);
    std::cout << "Testing: " << p6 << "\n";
    test(r6, "b", true);
    test(r6, "ab", true);
    test(r6, "abb", true);
    test(r6, "aab", true);
    test(r6, "aaab", true);
    test(r6, "aaba", false);

    const char* p7 = "a?aa";
    rex r7(p7);
    std::cout << "Testing: " << p7 << "\n";
    test(r7, "aa", true);
    test(r7, "aaa", true);
    test(r7, "a", false);

    const char* p8 = "(a|b)+c";
    rex r8(p8);
    std::cout << "Testing: " << p8 << "\n";
    test(r8, "c", false);
    test(r8, "ac", true);
    test(r8, "bc", true);
    test(r8, "abc", true);
    test(r8, "aabbc", true);

    const char* p9 = "a*b*a";
    rex r9(p9);
    std::cout << "Testing: " << p9 << "\n";
    test(r9, "a", true);
    test(r9, "ba", true);
    test(r9, "aa", true);
    test(r9, "bba", true);
    test(r9, "bbb", false);

    const char* p10 = "(x|y)*(x|y)";
    rex r10(p10);
    std::cout << "Testing: " << p10 << "\n";
    test(r10, "", false);
    test(r10, "x", true);
    test(r10, "y", true);
    test(r10, "xy", true);
    test(r10, "yy", true);
    test(r10, "yxy", true);

    const char* p11 = "(a|b|c)*";
    rex r11(p11);
    std::cout << "Testing: " << p11 << "\n";
    test(r11, "", true);
    test(r11, "abcabc", true);
    test(r11, "abcd", false);

    const char* p12 = "(ab)*c";
    rex r12(p12);
    std::cout << "Testing: " << p12 << "\n";
    test(r12, "c", true);
    test(r12, "abc", true);
    test(r12, "ababc", true);
    test(r12, "abab", false);

    const char* p13 = "a?b?c?";
    rex r13(p13);
    std::cout << "Testing: " << p13 << "\n";
    test(r13, "", true);
    test(r13, "a", true);
    test(r13, "b", true);
    test(r13, "c", true);
    test(r13, "ab", true);
    test(r13, "ac", true);
    test(r13, "bc", true);
    test(r13, "abc", true);
    test(r13, "abcc", false);

    const char* p14 = "((a|b)c)*d";
    rex r14(p14);
    std::cout << "Testing: " << p14 << "\n";
    test(r14, "d", true);
    test(r14, "acd", true);
    test(r14, "bcd", true);
    test(r14, "acbcd", true);
    test(r14, "acbc", false);

    const char* p15 = "(a|b|c)+";
    rex r15(p15);
    std::cout << "Testing: " << p15 << "\n";
    test(r15, "", false);
    test(r15, "a", true);
    test(r15, "b", true);
    test(r15, "c", true);
    test(r15, "abc", true);
    test(r15, "abccba", true);
    test(r15, "abcd", false);

        const char* p16 = "a*bc";
    rex r16(p16);
    std::cout << "Testing: " << p16 << "\n";
    test(r16, "bc", true);       // Fails if concat propagation is wrong (epsilon from a* not enabling b)
    test(r16, "abc", true);
    test(r16, "aabc", true);
    test(r16, "c", false);

    const char* p17 = "(ab)*c";
    rex r17(p17);
    std::cout << "Testing: " << p17 << "\n";
    test(r17, "c", true);        // Fails if kleene star doesn't re-enable after zero repetitions
    test(r17, "abc", true);
    test(r17, "ababc", true);
    test(r17, "ab", false);

    const char* p18 = "(a*)*b";
    rex r18(p18);
    std::cout << "Testing: " << p18 << "\n";
    test(r18, "b", true);        // Nested kleene star with epsilon must not fail
    test(r18, "ab", true);
    test(r18, "aaab", true);

    const char* p19 = "(a?)*b";
    rex r19(p19);
    std::cout << "Testing: " << p19 << "\n";
    test(r19, "b", true);        // Optional inside star requires correct epsilon handling
    test(r19, "ab", true);
    test(r19, "aab", true);

    const char* p20 = "a*";
    rex r20(p20);
    std::cout << "Testing: " << p20 << "\n";
    test(r20, "", true);         // Should accept empty string
    test(r20, "a", true);
    test(r20, "aa", true);

    const char* p21 = "a*";
    rex r21(p21);
    std::cout << "Testing: " << p21 << "\n";
    test(r21, "b", false);       // Should not accept non-a

    const char* p22 = "ab*";
    rex r22(p22);
    std::cout << "Testing: " << p22 << "\n";
    test(r22, "a", true);        // b* allows zero b
    test(r22, "ab", true);
    test(r22, "abb", true);
    test(r22, "b", false);

    const char* p23 = "b(a*a)";
    rex r23(p23);
    std::cout << "Testing: " << p23 << "\n";
    test(r23, "ba", true);
    test(r23, "ab", false);

    const char* p24 = "abc";
    rex r24(p24);
    std::cout << "Testing reversed: " << p24 << "\n";
    test(r24, "abc", false, true);
    test(r24, "cba", true, true);
    test(r24, "cb", false, true);

    const char* p25 = "aa*b";
    rex r25(p25);
    std::cout << "Testing reversed: " << p25 << "\n";
    test(r25, "ba", true, true);
    test(r25, "ab", false, true);
    
    const char* p26 = "a+(b|c*)d|ef";
    rex r26(p26);
    std::cout << "Testing reversed: " << p26 << "\n";
    test(r26, "fe", true, true);
    test(r26, "f", false, true);
    test(r26, "db", false, true);
    test(r26, "dccccaa", true, true);
    test(r26, "ccca", false, true);
    test(r26, "da", true, true);
    test(r26, "dba", true, true);

    const char* p27 = "abcd";
    rex r27(p27);
    std::cout << "Testing search: " << p27 << "\n";
    testsearch(r27, "aaaabcddddd", 3, 4);
    testsearch(r27, "aaaabcddddd", -1, 0);
    testsearch(r27, "abcdabcd", 0, 4);
    testsearch(r27, "abcdabcd", 4, 4);

    const char* p28 = "a+\1-\255*b+|XY";
    rex r28(p28);
    std::cout << "Testing search: " << p28 << "\n";
    const char* s28 = "__aaXYbb__XY__XY";
    testsearch(r28, s28, 2, 6);
    testsearch(r28, s28, 10, 2);
    testsearch(r28, s28, 14, 2);
    testsearch(r28, s28, -1, 0);

    const char* p29 = "I|ate|an|apple.";
    rex r29(p29);
    std::cout << "Testing search: " << p29 << "\n";
    const char* s29 = "I ate an apple. It was delicious!";
    testsearch(r29, s29, 0, 1);
    testsearch(r29, s29, 2, 3);
    testsearch(r29, s29, 6, 2);
    testsearch(r29, s29, 9, 6);
    testsearch(r29, s29, 16, 1);
    testsearch(r29, s29, -1, 0);
    const char* s292 = "Iateanapple.Itwasdelicious!";
    testsearch(r29, s292, 0, 1);
    testsearch(r29, s292, 1, 3);
    testsearch(r29, s292, 4, 2);
    testsearch(r29, s292, 6, 6);
    testsearch(r29, s292, 12, 1);
    testsearch(r29, s292, -1, 0);
    
    const char* p30 = "\\\\a-z*\\(0-9*\\)";
    rex r30(p30);
    std::cout << "Testing parser: " << p30 << "\n";
    test(r30, "\\a(100)", true);
    test(r30, "\\hello(6969)", true);
    test(r30, "\\h()", true);
    test(r30, "\\(2)", true);
    test(r30, "hello(222)", false);
    test(r30, "\\hello222", false);

    /* 31 – single literal backslash                                       */
    const char* p31 = "\\\\";
    rex r31(p31);
    std::cout << "Testing parser: " << p31 << "\n";
    test(r31, "\\",  true);
    test(r31, "",    false);
    test(r31, "\\\\", false);

    /* 32 – both limits of a range escaped  (‘q’..‘z’)  with “+” repetition */
    const char* p32 = "\\q-\\z+";
    rex r32(p32);
    std::cout << "Testing parser: " << p32 << "\n";
    test(r32, "q",    true);
    test(r32, "t",    true);
    test(r32, "z",    true);
    test(r32, "qqz",  true);
    test(r32, "p",    false);
    test(r32, "{",    false);

    /* 33 – only the high limit of a range escaped  (‘a’..‘z’) “+”          */
    const char* p33 = "a-\\z+";
    rex r33(p33);
    std::cout << "Testing parser: " << p33 << "\n";
    test(r33, "a",  true);
    test(r33, "m",  true);
    test(r33, "z",  true);
    test(r33, "aa", true);
    test(r33, "{",  false);

    /* 34 – pattern ending with a solitary backslash                        */
    const char* p34 = "ab\\\\";         /* pattern:  a b \                  */
    rex r34(p34);
    std::cout << "Testing parser: " << p34 << "\n";
    test(r34, "ab\\",  true);
    test(r34, "ab",    false);
    test(r34, "ab\\\\", false);

    /* 35 – escaped literal parenthesis “\(”                                */
    const char* p35 = "\\(";
    rex r35(p35);
    std::cout << "Testing parser: " << p35 << "\n";
    test(r35, "(",  true);
    test(r35, "a",  false);
    test(r35, "\\", false);

    /* 36 – range whose **low** limit is escaped  (‘*’..‘+’)                */
    const char* p36 = "\\*-+";
    rex r36(p36);
    std::cout << "Testing parser: " << p36 << "\n";
    test(r36, "*", true);
    test(r36, "+", true);
    test(r36, ",", false);

    /* 37 – another low-escaped range, with “+” repetition (‘m’..‘q’)        */
    const char* p37 = "m-\\q+";
    rex r37(p37);
    std::cout << "Testing parser: " << p37 << "\n";
    test(r37, "m",     true);
    test(r37, "o",     true);
    test(r37, "p",     true);
    test(r37, "q",     true);
    test(r37, "r",     false);
    test(r37, "mnopq", true);

    /* 38 – literal plus sign “\+”                                          */
    const char* p38 = "\\+";
    rex r38(p38);
    std::cout << "Testing parser: " << p38 << "\n";
    test(r38, "+",  true);
    test(r38, "++", false);

}