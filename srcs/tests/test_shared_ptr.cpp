#include "shared_ptr.hpp"
#include <cstdlib>
#include <iostream>

void testLeaks()
{
    system("leaks a.out");
}

int main()
{
    atexit(testLeaks);
    // ptr1 pointing to an integer.
    ft::shared_ptr<int> ptr1(new int(151));
    std::cout << "--- Shared pointers ptr1 ---\n";
    *ptr1 = 100;
    std::cout << " ptr1's value now: " << *ptr1 << std::endl;
    std::cout << ptr1;

    {
        // ptr2 pointing to same integer
        // which ptr1 is pointing to
        // Shared pointer reference counter
        // should have increased now to 2.
        ft::shared_ptr<int> ptr2 = ptr1;
        std::cout << "--- Shared pointers ptr1, ptr2 ---\n";
        std::cout << ptr1;
        std::cout << ptr2;

        {
            // ptr3 pointing to same integer
            // which ptr1 and ptr2 are pointing to.
            // Shared pointer reference counter
            // should have increased now to 3.
            ft::shared_ptr<int> ptr3(ptr2);
            std::cout << "--- Shared pointers ptr1, ptr2, ptr3 "
                         "---\n";
            std::cout << ptr1;
            std::cout << ptr2;
            std::cout << ptr3;
        }

        // ptr3 is out of scope.
        // It would have been destructed.
        // So shared pointer reference counter
        // should have decreased now to 2.
        std::cout << "--- Shared pointers ptr1, ptr2 ---\n";
        std::cout << ptr1;
        std::cout << ptr2;
    }

    // ptr2 is out of scope.
    // It would have been destructed.
    // So shared pointer reference counter
    // should have decreased now to 1.
    std::cout << "--- Shared pointers ptr1 ---\n";
    std::cout << ptr1;

    return 0;
}
