#include "hal.h"

#include "x86.h"

#define NR_KEYS 18

enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};

/* Only the following keys are used in NEMU-PAL. */
static const int keycode_array[] = {
	K_UP, K_DOWN, K_LEFT, K_RIGHT, K_ESCAPE,
	K_RETURN, K_SPACE, K_PAGEUP, K_PAGEDOWN, K_r,
	K_a, K_d, K_e, K_w, K_q,
	K_s, K_f, K_p
};

static int key_state[NR_KEYS];

void
keyboard_event(void) {
	/* TODO: Fetch the scancode and update the key states. */
	volatile uint8_t scancode = in_byte(0x60);
	int i;
	for (i = 0; i < NR_KEYS; i++)
		if (keycode_array[i] == (scancode & 0x7f))
			break;
	if (i == NR_KEYS) return;
	volatile int state = key_state[i];
	switch (state)
	{
		case KEY_STATE_EMPTY:
			if (!(scancode & 0x80))
			{
				Log("press\n");
				key_state[i] = KEY_STATE_PRESS;
			}
			break;
		case KEY_STATE_WAIT_RELEASE:
			if (scancode & 0x80)
			{
				Log("release\n");
				key_state[i] = KEY_STATE_RELEASE;
			}
			break;
		default:
			break;
	}
	
}

static inline int
get_keycode(int index) {
	assert(index >= 0 && index < NR_KEYS);
	return keycode_array[index];
}

static inline int
query_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	return key_state[index];
}

static inline void
release_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_WAIT_RELEASE;
}

static inline void
clear_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_EMPTY;
}

bool 
process_keys(void (*key_press_callback)(int), void (*key_release_callback)(int)) {
	cli();
	/* TODO: Traverse the key states. Find a key just pressed or released.
	 * If a pressed key is found, call `key_press_callback' with the keycode.
	 * If a released key is found, call `key_release_callback' with the keycode.
	 * If any such key is found, the function return true.
	 * If no such key is found, the function return false.
	 * Remember to enable interrupts before returning from the function.
	 */
	int i;
	bool found = false;
	for (i = 0; i < NR_KEYS; i++)
	{
		volatile int state = query_key(i);
		switch (state)
		{
			case KEY_STATE_PRESS:
				found = true;
				key_press_callback(get_keycode(i));
				release_key(i);
				break;
			case KEY_STATE_RELEASE:
				found = true;
				key_release_callback(get_keycode(i));
				clear_key(i);
				break;
			default:
				break;
		}
	}
	sti();
	return found;
}
