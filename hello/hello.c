#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Asks for User input on name (Needs cs50.h)
    string name = get_string("What's your name? ");
    printf("Hello, %s! \n", name);
}