#pragma once
#include <sstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

// Add this macro if you want to disable passed-test output
#define PRINT_ALL_PASSED false

template <typename T>
void run_test(const std::string &test_name, std::function<void(T &, std::stringstream &)> test_func, const std::string &expected_output)
{
    T table;
    std::stringstream out;
    test_func(table, out);
    std::string result = out.str();
    if (result != expected_output)
    {
        std::cerr << "[FAILED] " << test_name << "\nExpected:\n"
                  << expected_output << "Got:\n"
                  << result << "\n";
    }
    else if (PRINT_ALL_PASSED)
    {
        std::cout << "[PASSED] " << test_name << "\n";
    }
}

template <typename T>
void run_all_tests()
{
    using namespace std;

    run_test<T>("Basic Insert/Delete/Check", [](auto &table, auto &out) {
        out << (table.Add("abc") ? "OK" : "FAIL") << "\n";
        out << (table.Has("abc") ? "OK" : "FAIL") << "\n";
        out << (table.Add("abc") ? "OK" : "FAIL") << "\n"; // duplicate
        out << (table.Delete("abc") ? "OK" : "FAIL") << "\n";
        out << (table.Has("abc") ? "OK" : "FAIL") << "\n";
        out << (table.Delete("abc") ? "OK" : "FAIL") << "\n"; // deleted
    }, "OK\nOK\nFAIL\nOK\nFAIL\nFAIL\n");

    run_test<T>("Reuse deleted slot", [](auto &table, auto &out) {
        out << (table.Add("a") ? "OK" : "FAIL") << "\n";
        out << (table.Delete("a") ? "OK" : "FAIL") << "\n";
        out << (table.Add("b") ? "OK" : "FAIL") << "\n";
        out << (table.Has("a") ? "OK" : "FAIL") << "\n";
        out << (table.Has("b") ? "OK" : "FAIL") << "\n";
    }, "OK\nOK\nOK\nFAIL\nOK\n");

    run_test<T>("Trigger rehash and validate integrity", [](auto &table, auto &out) {
        for (char c = 'a'; c <= 'z'; ++c)
            out << (table.Add(string(1, c)) ? "OK" : "FAIL") << "\n";

        for (char c = 'a'; c <= 'z'; ++c)
            out << (table.Has(string(1, c)) ? "OK" : "FAIL") << "\n";

        out << (table.Add("z") ? "OK" : "FAIL") << "\n"; // duplicate
    }, []() {
        std::stringstream s;
        for (int i = 0; i < 26; ++i) s << "OK\n";
        for (int i = 0; i < 26; ++i) s << "OK\n";
        s << "FAIL\n";
        return s.str();
    }());

    run_test<T>("Repeated delete/insert stress", [](auto &table, auto &out) {
        for (int i = 0; i < 100; ++i)
        {
            string key = "k" + to_string(i % 10);
            out << (table.Add(key) ? "OK" : "FAIL") << "\n";
            out << (table.Delete(key) ? "OK" : "FAIL") << "\n";
        }
    }, []() {
        std::stringstream s;
        for (int i = 0; i < 100; ++i) {
            s << "OK\n";
            s << "OK\n";
        }
        return s.str();
    }());

    run_test<T>("Many deletions and reinserts", [](auto &table, auto &out) {
        for (int i = 0; i < 30; ++i)
            out << (table.Add("key" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < 30; i += 2)
            out << (table.Delete("key" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < 30; ++i)
            out << (table.Has("key" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < 30; i += 2)
            out << (table.Add("key" + to_string(i)) ? "OK" : "FAIL") << "\n"; // Reuse deleted
    }, []() {
        std::stringstream s;
        for (int i = 0; i < 30; ++i) s << "OK\n";
        for (int i = 0; i < 30; i += 2) s << "OK\n";
        for (int i = 0; i < 30; ++i) s << (i % 2 == 0 ? "FAIL\n" : "OK\n");
        for (int i = 0; i < 30; i += 2) s << "OK\n";
        return s.str();
    }());

    run_test<T>("Stress insert and query", [](auto &table, auto &out) {
        const int N = 1000;
        for (int i = 0; i < N; ++i)
            out << (table.Add("stress" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < N; ++i)
            out << (table.Has("stress" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < N; i += 3)
            out << (table.Delete("stress" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < N; ++i)
            out << (table.Has("stress" + to_string(i)) ? "OK" : "FAIL") << "\n";
    }, []() {
        std::stringstream s;
        for (int i = 0; i < 1000; ++i) s << "OK\n";
        for (int i = 0; i < 1000; ++i) s << "OK\n";
        for (int i = 0; i < 1000; i += 3) s << "OK\n";
        for (int i = 0; i < 1000; ++i)
            s << (i % 3 == 0 ? "FAIL\n" : "OK\n");
        return s.str();
    }());

    run_test<T>("Empty table operations", [](auto &table, auto &out) {
        out << (table.Has("nonexistent") ? "OK" : "FAIL") << "\n";
        out << (table.Delete("nonexistent") ? "OK" : "FAIL") << "\n";
    }, "FAIL\nFAIL\n");

    run_test<T>("Add/Delete same element repeatedly", [](auto &table, auto &out) {
        for (int i = 0; i < 10; ++i) {
            out << (table.Add("repeat") ? "OK" : "FAIL") << "\n";
            out << (table.Delete("repeat") ? "OK" : "FAIL") << "\n";
        }
    }, []() {
        std::stringstream s;
        for (int i = 0; i < 10; ++i) {
            s << "OK\n";
            s << "OK\n";
        }
        return s.str();
    }());

    run_test<T>("Add/Delete multiple elements", [](auto &table, auto &out) {
        for (int i = 0; i < 10; ++i)
            out << (table.Add("key" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < 10; ++i)
            out << (table.Delete("key" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < 10; ++i)
            out << (table.Has("key" + to_string(i)) ? "OK" : "FAIL") << "\n";
    }, []() {
        std::stringstream s;
        for (int i = 0; i < 10; ++i) s << "OK\n";
        for (int i = 0; i < 10; ++i) s << "OK\n";
        for (int i = 0; i < 10; ++i) s << "FAIL\n";
        return s.str();
    }());

    run_test<T>("Collision handling", [](auto &table, auto &out) {
        out << (table.Add("a") ? "OK" : "FAIL") << "\n";
        out << (table.Add("b") ? "OK" : "FAIL") << "\n"; // Assume "a" and "b" collide
        out << (table.Has("a") ? "OK" : "FAIL") << "\n";
        out << (table.Has("b") ? "OK" : "FAIL") << "\n";
        out << (table.Delete("a") ? "OK" : "FAIL") << "\n";
        out << (table.Has("a") ? "OK" : "FAIL") << "\n";
        out << (table.Has("b") ? "OK" : "FAIL") << "\n";
    }, "OK\nOK\nOK\nOK\nOK\nFAIL\nOK\n");

    run_test<T>("Stress test with large number of elements", [](auto &table, auto &out) {
        const int N = 10000;
        for (int i = 0; i < N; ++i)
            out << (table.Add("key" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < N; ++i)
            out << (table.Has("key" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < N; i += 2)
            out << (table.Delete("key" + to_string(i)) ? "OK" : "FAIL") << "\n";

        for (int i = 0; i < N; ++i)
            out << (table.Has("key" + to_string(i)) ? "OK" : "FAIL") << "\n";
    }, []() {
        std::stringstream s;
        const int N = 10000;
        for (int i = 0; i < N; ++i) s << "OK\n";
        for (int i = 0; i < N; ++i) s << "OK\n";
        for (int i = 0; i < N; i += 2) s << "OK\n";
        for (int i = 0; i < N; ++i)
            s << (i % 2 == 0 ? "FAIL\n" : "OK\n");
        return s.str();
    }());

    run_test<T>("Edge case: Long strings", [](auto &table, auto &out) {
        std::string long_key(1000, 'x');
        out << (table.Add(long_key) ? "OK" : "FAIL") << "\n";
        out << (table.Has(long_key) ? "OK" : "FAIL") << "\n";
        out << (table.Delete(long_key) ? "OK" : "FAIL") << "\n";
        out << (table.Has(long_key) ? "OK" : "FAIL") << "\n";
    }, "OK\nOK\nOK\nFAIL\n");

    run_test<T>("Edge case: Empty string key", [](auto &table, auto &out) {
        out << (table.Add("") ? "OK" : "FAIL") << "\n";
        out << (table.Has("") ? "OK" : "FAIL") << "\n";
        out << (table.Delete("") ? "OK" : "FAIL") << "\n";
        out << (table.Has("") ? "OK" : "FAIL") << "\n";
    }, "OK\nOK\nOK\nFAIL\n");

    run_test<T>("Simple sequence test", [](auto &table, auto &out) {
        out << (table.Add("hello") ? "OK" : "FAIL") << "\n";
        out << (table.Add("bye") ? "OK" : "FAIL") << "\n";
        out << (table.Has("bye") ? "OK" : "FAIL") << "\n";
        out << (table.Add("bye") ? "OK" : "FAIL") << "\n";
        out << (table.Delete("bye") ? "OK" : "FAIL") << "\n";
        out << (table.Has("bye") ? "OK" : "FAIL") << "\n";
        out << (table.Has("hello") ? "OK" : "FAIL") << "\n";
    }, "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");

    run_test<T>("Complex probing test", [](auto &table, auto &out) {
        out << (table.Add("key1") ? "OK" : "FAIL") << "\n";
        out << (table.Add("key2") ? "OK" : "FAIL") << "\n";
        out << (table.Add("key3") ? "OK" : "FAIL") << "\n";
        out << (table.Delete("key2") ? "OK" : "FAIL") << "\n";
        out << (table.Add("key4") ? "OK" : "FAIL") << "\n"; // Should reuse deleted slot
        out << (table.Has("key1") ? "OK" : "FAIL") << "\n";
        out << (table.Has("key2") ? "OK" : "FAIL") << "\n";
        out << (table.Has("key3") ? "OK" : "FAIL") << "\n";
        out << (table.Has("key4") ? "OK" : "FAIL") << "\n";
    }, "OK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\nOK\n");

    run_test<T>("Probing with collisions", [](auto &table, auto &out) {
        out << (table.Add("a") ? "OK" : "FAIL") << "\n";
        out << (table.Add("b") ? "OK" : "FAIL") << "\n"; // Assume "a" and "b" collide
        out << (table.Add("c") ? "OK" : "FAIL") << "\n"; // Another collision
        out << (table.Delete("b") ? "OK" : "FAIL") << "\n";
        out << (table.Add("d") ? "OK" : "FAIL") << "\n"; // Reuse slot of "b"
        out << (table.Has("a") ? "OK" : "FAIL") << "\n";
        out << (table.Has("b") ? "OK" : "FAIL") << "\n";
        out << (table.Has("c") ? "OK" : "FAIL") << "\n";
        out << (table.Has("d") ? "OK" : "FAIL") << "\n";
    }, "OK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\nOK\n");
}
