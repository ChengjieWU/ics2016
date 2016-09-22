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

WP* head_wp() {
	return head;
}

void free_wp_no(int no) {
	if (no < 0 || no > 31) return;
	WP* pwp = &wp_pool[no];
	free_wp(pwp);
}
