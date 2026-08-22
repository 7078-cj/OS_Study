#include "types.h"

void printf(char* str){

    uint16_t* VideoMemory = (uint16_t*)0xb8000;

    for(int i = 0; str[i] != '\0'; i++){
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    }
}

typedef void (*constructor)(void);

typedef void (*constructor)(void);

extern constructor start_ctors;
extern constructor end_ctors;

void callConstructors(){
    for (constructor* i = &start_ctors; i != &end_ctors; i++) {
        (*i)();
    }
}
void kernelMain(void* multiboot_structure, unsigned int magic_number){
    printf("Hello World WOW \n");

    while(1);
}