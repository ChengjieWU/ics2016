#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
		wp_pool[i].occupied = false;
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp() {
	if (free_ == NULL) assert(0);
	WP* pwp = free_;
	free_ = free_->next;
	pwp->next = head;
	head = pwp;
	pwp->occupied = true;
	return pwp;
}

void free_wp(WP* wp) {
	if (wp->occupied == false) return;
	wp->occupied = false;
	WP* pwp = head;
	if (wp == head) head = head->next;
	else 
	{
		while (pwp->next != wp) pwp = pwp->next;
		pwp->next = wp->next;
	}
	wp->next = free_;
	free_ = wp;
}

/*WP* head_wp() {
	return head;
}*/

void print_wp() {
	printf("Num\tType\tDisp\tEnb\tAddress\tWhat\n");
	WP* pwp;
	for (pwp = head; pwp != NULL; pwp = pwp->next) {
		printf("%d\thw watchpoint\tkeep\ty\t%s\t%d\n", pwp->NO, pwp->expression, pwp->encounter);	
	}
}

void free_wp_no(int no) {
	if (no < 0 || no > 31) return;
	WP* pwp = &wp_pool[no];
	free_wp(pwp);
}

int check_wp() {
	int stop = 0;
	WP* pwp = head;
	for (; pwp != NULL; pwp = pwp->next) {
		bool calcu_ok;
		uint32_t new_value = expr(pwp->expression, &calcu_ok);
		if (pwp->value != new_value) {
			pwp->encounter = pwp->encounter + 1;
			stop++;
			printf("\nHardware watchpoint %d: %s\n\nOld value = %u\nNew value = %u\n", pwp->NO, pwp->expression, pwp->value, new_value);
			pwp->value = new_value;
		}
	}
	return stop;
}
