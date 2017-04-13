#ifndef SSTACK_H
#define SSTACK_H

typedef struct {
	char * data;
	void * next;
	void * prev;
} record_t;

typedef struct {
	void * (*malloc)(size_t);
	void (*free)(void *);
	record_t * head;
	record_t * tail;
} stack_t;

stack_t * create_stack(void * (*custom_malloc)(size_t),
		void (*custom_free)(void *));

void destroy_stack(stack_t * stack);

int append(stack_t * stack, char * data);

int prepend(stack_t * stack, char * data);

int pop_last(stack_t * stack, char ** data);

int pop_first(stack_t * stack, char ** data);

int stack_elements_count(stack_t * stack);

int stack_data_length(stack_t * stack);

void dump_stack(stack_t * stack);

#endif
