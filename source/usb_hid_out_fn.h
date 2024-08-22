
#ifndef USB_HID_OUT_FN_H_
#define USB_HID_OUT_FN_H_

// To only send one keycode.
// For mod:
// LC = 1, LS = 2, LA = 4, LG = 8
// RC = 16, RS = 32, RA = 64, RG = 128
// To combine bitwise OR them together
// first can usually be false
void send_keycode(char mod, char key, bool first);

// keys shound end with 0xff
// keys are actual key codes (not text)
void send_keycodes(char* mods, char* keys);

// Null terminated C string
void send_string(char* str);

void send_at_once();

#endif