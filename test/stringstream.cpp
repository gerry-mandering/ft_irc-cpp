#include <iostream>
#include <sstream>

using namespace std;

int main(void)
{
    stringstream ss;

    ss << "hi";
    cout << ss.str() << endl;
    ss << "hello";
    cout << ss.str() << endl;
    ss << "world";
    cout << ss.str() << endl;
    ss << "!";
    cout << ss.str() << endl;
    return 0;
}