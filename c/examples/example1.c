#include <stdio.h>
#include <UniSerializer.h>

int main()
{
   /* printf function displays the content that is
    * passed between the double quotes.
    */
    UniBuffer buffer = Uni_init(64, true, true);
    printf("%d\n", (int)buffer.length);

    printf("Hello World\n");
    Uni_free(&buffer);
    return 0;
}
