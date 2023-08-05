#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    fcntl(1, F_SETFL, O_NONBLOCK);
    int nwrite;

    while (1)
    {
        nwrite = write(1, "hello\n", 6);
        if (nwrite < 0)
            perror("I'm failed");
    }
    return (0);
}