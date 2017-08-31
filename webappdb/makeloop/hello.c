#include <stdio.h>
#include "hello.h"

int main(){
    struct mytype my={"hongqi",50,6};
    printf("Hello %s: %d, %d\n",my.name, my.age, my.id);
}
