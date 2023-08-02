#include <iostream>
#include <sstream>
#include <string>

void test1(const std::string &str)
{
    std::stringstream ss(str);
    std::string command;
    std::string username;
    std::string hostname;
    std::string servername;
    std::string realname;

    if (!(ss >> command >> username >> hostname >> servername >> realname))
        std::cout << "fail to parse token\n";
    else
        std::cout << "success to parse token\n";
    std::cout << command << std::endl;
    std::cout << username << std::endl;
    std::cout << hostname << std::endl;
    std::cout << servername << std::endl;
    std::cout << realname << std::endl;

    if (ss.eof())
        std::cout << "eof\n";
    else
        std::cout << "not eof\n";
    std::cout << "---------------------\n";
}

void test_junk(const std::string &str)
{
    std::stringstream ss(str);
    std::string junk;

    ss >> junk;
    if (!(ss >> junk))
        std::cout << "fail to parse token\n";
    else
        std::cout << "success to parse token\n";
}

int main(void)
{
    // test1("a b c d");
    // test1("a");
    // test1("");
    // //
    test1("a b c d e");
    // test1("a b	c		d e ");
    test_junk("  ");
    test_junk(" a  ");

    return (0);
}