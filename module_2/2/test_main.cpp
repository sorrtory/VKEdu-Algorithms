#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../doctest.h"
#include "main.cpp"

TEST_CASE("BinaryTree Test Google Doc") {
    SUBCASE("Test Case 1") {
        BinaryTree<int> tree;
        tree.Add(2);
        tree.Add(1);
        tree.Add(3);

        std::ostringstream output;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        tree.Print();
        std::cout.rdbuf(oldCoutBuffer);

        CHECK(output.str() == "1 3 2 ");
    }

    SUBCASE("Test Case 2") {
        BinaryTree<int> tree;
        tree.Add(1);
        tree.Add(2);
        tree.Add(3);

        std::ostringstream output;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        tree.Print();
        std::cout.rdbuf(oldCoutBuffer);

        CHECK(output.str() == "3 2 1 ");
    }

    SUBCASE("Test Case 3") {
        BinaryTree<int> tree;
        tree.Add(3);
        tree.Add(1);
        tree.Add(2);

        std::ostringstream output;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        tree.Print();
        std::cout.rdbuf(oldCoutBuffer);

        CHECK(output.str() == "2 1 3 ");
    }
}

TEST_CASE("BinaryTree Test COntest") {
    SUBCASE("Пример 1") {
        BinaryTree<int> tree;
        tree.Add(7);
        tree.Add(2);
        tree.Add(10);
        tree.Add(8);
        tree.Add(5);
        tree.Add(3);
        tree.Add(6);
        tree.Add(4);
        tree.Add(1);
        tree.Add(9);

        std::ostringstream output;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        tree.Print();
        std::cout.rdbuf(oldCoutBuffer);

        CHECK(output.str() == "1 4 3 6 5 2 9 8 10 7 ");
    }

    SUBCASE("Пример 2") {
        BinaryTree<int> tree;
        tree.Add(7);
        tree.Add(10);
        tree.Add(2);
        tree.Add(6);
        tree.Add(1);
        tree.Add(9);
        tree.Add(4);
        tree.Add(5);
        tree.Add(8);
        tree.Add(3);

        std::ostringstream output;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        tree.Print();
        std::cout.rdbuf(oldCoutBuffer);

        CHECK(output.str() == "1 3 5 4 6 2 8 9 10 7 ");
    }

    SUBCASE("Пример 3") {
        BinaryTree<int> tree;
        tree.Add(4);
        tree.Add(3);
        tree.Add(9);
        tree.Add(5);
        tree.Add(6);
        tree.Add(8);
        tree.Add(7);
        tree.Add(2);
        tree.Add(10);
        tree.Add(1);

        std::ostringstream output;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        tree.Print();
        std::cout.rdbuf(oldCoutBuffer);

        CHECK(output.str() == "1 2 3 7 8 6 5 10 9 4 ");
    }
}
