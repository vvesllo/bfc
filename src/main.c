#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned char bf_value;

struct BFSource
{
    char* code;
    size_t length;
    size_t index;
};

struct BFMemory
{
    bf_value values[1024];
    size_t index;
};

struct Brainfuck
{
    struct BFMemory* memory;
    struct BFSource* source;
};

struct Brainfuck* bf_init();
void bf_clear(struct Brainfuck* bf);

void bf_op_forward(struct Brainfuck* bf);
void bf_op_backward(struct Brainfuck* bf);
void bf_op_add(struct Brainfuck* bf);
void bf_op_sub(struct Brainfuck* bf);
void bf_op_loop(struct Brainfuck* bf);
void bf_op_print_char(const struct Brainfuck* bf);
void bf_op_print_int(const struct Brainfuck* bf);
void bf_op_input(struct Brainfuck* bf);

void bf_load(struct Brainfuck* bf, const char* filepath);
void bf_set_index(struct Brainfuck* bf, size_t index);
void bf_advance(struct Brainfuck* bf);
char bf_current_char(struct Brainfuck* bf);
bf_value bf_current_value(struct Brainfuck* bf);

void bf_parse(struct Brainfuck* bf);


int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        printf("Usage: bfc.exe [file.bf]");
        return 0;
    }

    struct Brainfuck* bf = bf_init();
    
    bf_load(bf, argv[1]);

    while (1)
    {
        if (bf_current_char(bf) == '\0') break;
        
        bf_parse(bf);
        bf_advance(bf);
    }
    
    bf_clear(bf);

    return 0;
}

struct Brainfuck* bf_init()
{
    struct Brainfuck* bf = (struct Brainfuck*)malloc(sizeof(struct Brainfuck)); 
    bf->memory = (struct BFMemory*)malloc(sizeof(struct BFMemory));
    bf->source = (struct BFSource*)malloc(sizeof(struct BFSource));

    bf->memory->index = 0;
    bf->source->index = 0;

    memset(bf->memory->values, 0, sizeof(bf->memory->values));

    return bf;
}

void bf_clear(struct Brainfuck* bf)
{
    free(bf->source->code); 
    free(bf->source); 
    free(bf->memory); 
    free(bf); 
}

void bf_op_forward(struct Brainfuck* bf) { ++bf->memory->index; }
void bf_op_backward(struct Brainfuck* bf) { --bf->memory->index; }
void bf_op_add(struct Brainfuck* bf) { ++bf->memory->values[bf->memory->index]; }
void bf_op_sub(struct Brainfuck* bf) { --bf->memory->values[bf->memory->index]; }
void bf_op_print_char(const struct Brainfuck* bf) { printf("%c", bf->memory->values[bf->memory->index]); }
void bf_op_print_int(const struct Brainfuck* bf) { printf("%d", bf->memory->values[bf->memory->index]); }
void bf_op_input(struct Brainfuck* bf) 
{
    int input;
    scanf("%d", &input);

    bf->memory->values[bf->memory->index] = input;
}

void bf_op_loop(struct Brainfuck* bf) 
{
    size_t loop_start = bf->source->index; 

    bf_advance(bf);

    do
    {
        bf->source->index = loop_start + 1;
        
        while (bf_current_char(bf) != ']') 
        {
            bf_parse(bf);
            bf_advance(bf);
        }
    }
    while (bf_current_value(bf) != 0);
    
    bf_advance(bf);
}


void bf_load(struct Brainfuck* bf, const char* filepath)
{
    FILE* file = fopen(filepath, "r");
    fseek(file, 0, SEEK_END);
    bf->source->length = ftell(file);
    fseek(file, 0, SEEK_SET);

    bf->source->code = (char*)malloc(bf->source->length + 1);
    size_t bytes_read = fread(bf->source->code, 1, bf->source->length, file);
    bf->source->code[bytes_read] = '\0';
 
    fclose(file);
}
void bf_set_index(struct Brainfuck* bf, size_t index) { bf->source->index = index; }
void bf_advance(struct Brainfuck* bf) { ++bf->source->index; }
char bf_current_char(struct Brainfuck* bf) { return bf->source->code[bf->source->index]; }
bf_value bf_current_value(struct Brainfuck* bf) { return bf->memory->values[bf->memory->index]; }

void bf_parse(struct Brainfuck* bf)
{
    switch (bf_current_char(bf))
    {
    case '>': bf_op_forward(bf); break;
    case '<': bf_op_backward(bf); break;
    case '+': bf_op_add(bf); break;
    case '-': bf_op_sub(bf); break;
    case '.': bf_op_print_char(bf); break;
    case '\'': bf_op_print_int(bf); break;
    case ',': bf_op_input(bf); break;
    case '[': bf_op_loop(bf); break;
    }
}