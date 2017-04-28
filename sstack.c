#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sstack.h"

void dump_element(record_t * r);
record_t * create_element(stack_t * stack, char * data);


stack_t * create_stack(void * (*custom_malloc)(size_t),
		void (*custom_free)(void *))
{
	stack_t * stack = NULL;

	if (custom_malloc == NULL) {
		stack = malloc(sizeof(stack_t));
	} else {
		stack = custom_malloc(sizeof(stack_t));
	}

	if (stack == NULL) {
		return NULL;
	}

	memset(stack, 0, sizeof(stack_t));

	if (custom_malloc == NULL) {
		stack->malloc = &malloc;
	} else {
		stack->malloc = custom_malloc;
	}

	if (custom_free == NULL) {
		stack->free = &free;
	} else {
		stack->free = custom_free;
	}

	return stack;
}

void destroy_stack(stack_t * stack)
{
	record_t *current = NULL;
	void (*custom_free)(void *) = stack->free;
	void * prev = NULL;

	for(current = stack->head, prev = current == NULL ? NULL : current->prev;
			current != NULL;
			current = prev, prev = current == NULL ? NULL : current->prev) {
		stack->free(current->data);
		stack->free(current);
	}

	custom_free(stack);
}

record_t * create_element(stack_t * stack, char * data)
{
	record_t *link = (record_t*) stack->malloc(sizeof(record_t));
	if (link == NULL)
		goto exit_with_error;

	memset(link, 0, sizeof(record_t));

	size_t data_size = strlen(data) + 1;
	link->data = (char *) stack->malloc(data_size);
	if (link->data == NULL)
		goto exit_with_error;

	memcpy(link->data, data, data_size);

	return link;

exit_with_error:
	if (link) {
		stack->free(link);
	}
	return NULL;
}

int append(stack_t * stack, char * data)
{
	record_t *link = create_element(stack, data);
	if (link == NULL) {
		return 1;
	}

	record_t * last_record = stack->head;

	if (last_record == NULL) {
		stack->head = link;
	} else {

		last_record->next = link;
		link->prev = last_record;
		link->next = NULL;
		stack->head = link;
	}

	return 0;
}

int prepend(stack_t * stack, char * data) {
	record_t *link = create_element(stack, data);
	if (link == NULL) {
		return 1;
	}

	record_t * first_record = stack->head;

	if (first_record == NULL) {
		stack->head = link;
	} else {

		while (first_record->prev) {
			first_record = first_record->prev;
		}

		link->prev = NULL;
		link->next = first_record;
		first_record->prev = link;
	}
	return 0;
}

int pop_last(stack_t * stack, char ** data)
{
	record_t * last_record = stack->head;

	if (last_record) {
		stack->head = stack->head->prev;
		*data = last_record->data;
		stack->free(last_record);
		return 0;
	}

	*data = NULL;
	return 1;
}

int pop_first(stack_t * stack, char ** data)
{
	record_t *first_record = stack->head;

	if (first_record == NULL)
		goto no_data;

	while (first_record->prev) {
		first_record = first_record->prev;
	}

	if (first_record->next) {
		((record_t *)(first_record->next))->prev = NULL;
	}

	*data = first_record->data;
	stack->free(first_record);

	if (first_record == stack->head) {
		stack->head = NULL;
	}

	return 0;

no_data:
	*data = NULL;
	return 1;
}

void dump_element(record_t * r) {
	if (r == NULL)
		return;
	printf("\telement %p\n", (void *)r);
	printf("\t\tdata: %s\n", r->data == NULL ? "NULL" : r->data);
	printf("\t\tnext: %p\n", (void *)r->next);
	printf("\t\tprev: %p\n", (void *)r->prev);
}

void dump_stack(stack_t * stack)
{
	record_t * current = NULL;
	printf("Begin\n");

	for(current = stack->head; current != NULL; current = current->prev) {
		dump_element(current);
	}

	printf("End\n");
}

int stack_elements_count(stack_t * stack)
{
	int length = 0;
	record_t * current = NULL;

	for(current = stack->head; current != NULL; current = current->prev) {
		length++;
	}

	return length;
}

int stack_data_length(stack_t * stack)
{
	int length = 0;
	record_t * current = NULL;

	for(current = stack->head; current != NULL; current = current->prev) {
		length += strlen(current->data);
	}

	return length;
}


