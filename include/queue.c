#include "queue.h"

pcb_queue *new_pcb_list() {
    pcb_queue *list;
    if(!(list = malloc(sizeof(pcb_queue)))) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->len = 0;
    return list;
}

void display(pcb_queue *list) {
	printf("%s: ", __FUNCTION__);
	pcb_queue_node *cur = list->head;
	if (list->head == NULL) {
		assert(list->tail == NULL);
		printf("Empty queue \n");
		return;
	}
	while(cur != NULL) {
		printf("%d ", cur->pcb);
		cur = cur->next;
	}
	printf("\n");
	return;
}

pcb_queue_node *queue_push(pcb_queue *queue, pcb_queue_node *node) {
  if (!node) return NULL;

  if (queue->len) {
    node->prev = queue->tail;
    node->next = NULL;
    queue->tail->next = node;
    queue->tail = node;
  } else {
    queue->head = queue->tail = node;
    node->prev = node->next = NULL;
  }

  ++queue->len;
  return node;
}

pcb_queue_node *queue_pop_last(pcb_queue *queue) {
  if (!queue->len) return NULL;

  pcb_queue_node *node = queue->tail;

  if (--queue->len) {
    (queue->tail = node->prev)->next = NULL;
  } else {
    queue->tail = queue->head = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

pcb_queue_node *queue_pop_first(pcb_queue *queue) {
  if (!queue->len) return NULL;

  pcb_queue_node *node = queue->head;

  if (--queue->len) {
    (queue->head = node->next)->prev = NULL;
  } else {
    queue->head = queue->tail = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}


