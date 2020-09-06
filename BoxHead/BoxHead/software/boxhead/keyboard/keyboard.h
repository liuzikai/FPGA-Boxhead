#ifndef KEYBOARD_H_
#define KAYBOARD_H_

extern void keyboard_init();
extern int keyboard_fetch(unsigned int *keycode);
extern int keyboard_hot_plugged();

#endif
