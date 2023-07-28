#include <iostream>
#include <string>

using namespace std;

int main(void)
{
    string str1 = "hi hello";
    string str2 = "hi ";
    string str3 = "hi2  ";
    string str4 = "hi3 ";

    int pos1, pos2, pos3;

    pos1 = str1.find(' ');
    pos2 = str2.find(' ');
    pos3 = str3.find(' ');

    str1 = str1.substr(pos1 + 1);
    str2 = str2.substr(pos2 + 1);
    str3 = str3.substr(pos3 + 1);
    // str4 = str2.substr(pos2 + 2);

    cout << "size: " << str1.size() << ", string: " << str1 << endl;
    cout << "size: " << str2.size() << ", string: " << str2 << endl;
    cout << "size: " << str3.size() << ", string: " << str3 << endl;
    cout << "size: " << str4.size() << ", string: " << str4 << endl;

    return 0;
}
