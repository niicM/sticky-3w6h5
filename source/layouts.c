
// For a current layout, in our case QWERTY, to produce a ascii character, 
// there is an associated key code, and also modifiers (shift, alt gr, etc). 
// For each of the ascii pritable characters that you showed me (in that order), 
// I need you to provide me with a line. 
// Each line has: the modifier code as a hex byte, a comma, the keycode, also 
// as a hex byte, another comma, and, as a comment // followed by the character 
// being printed.
// As an example this is the line for "a":
// 0x00, 0x04, // a

char out_kwerty[] = {
    0x00, 0x2C, // ' ' (space)
    0x02, 0x1E, // !
    0x02, 0x34, // "
    0x02, 0x20, // #
    0x02, 0x21, // $
    0x02, 0x22, // %
    0x02, 0x24, // &
    0x00, 0x34, // '
    0x02, 0x26, // (
    0x02, 0x27, // )
    0x02, 0x25, // *
    0x02, 0x2E, // +
    0x00, 0x36, // ,
    0x00, 0x2D, // -
    0x00, 0x37, // .
    0x00, 0x38, // /
    0x00, 0x27, // 0
    0x00, 0x1E, // 1
    0x00, 0x1F, // 2
    0x00, 0x20, // 3
    0x00, 0x21, // 4
    0x00, 0x22, // 5
    0x00, 0x23, // 6
    0x00, 0x24, // 7
    0x00, 0x25, // 8
    0x00, 0x26, // 9
    0x02, 0x33, // :
    0x00, 0x33, // ;
    0x02, 0x36, // <
    0x00, 0x2E, // =
    0x02, 0x37, // >
    0x02, 0x38, // ?
    0x02, 0x1F, // @
    0x02, 0x04, // A
    0x02, 0x05, // B
    0x02, 0x06, // C
    0x02, 0x07, // D
    0x02, 0x08, // E
    0x02, 0x09, // F
    0x02, 0x0A, // G
    0x02, 0x0B, // H
    0x02, 0x0C, // I
    0x02, 0x0D, // J
    0x02, 0x0E, // K
    0x02, 0x0F, // L
    0x02, 0x10, // M
    0x02, 0x11, // N
    0x02, 0x12, // O
    0x02, 0x13, // P
    0x02, 0x14, // Q
    0x02, 0x15, // R
    0x02, 0x16, // S
    0x02, 0x17, // T
    0x02, 0x18, // U
    0x02, 0x19, // V
    0x02, 0x1A, // W
    0x02, 0x1B, // X
    0x02, 0x1C, // Y
    0x02, 0x1D, // Z
    0x00, 0x2F, // [
    0x00, 0x31, // '\' (backslash)
    0x00, 0x30, // ]
    0x02, 0x23, // ^
    0x02, 0x2D, // _
    0x00, 0x35, // `
    0x00, 0x04, // a
    0x00, 0x05, // b
    0x00, 0x06, // c
    0x00, 0x07, // d
    0x00, 0x08, // e
    0x00, 0x09, // f
    0x00, 0x0A, // g
    0x00, 0x0B, // h
    0x00, 0x0C, // i
    0x00, 0x0D, // j
    0x00, 0x0E, // k
    0x00, 0x0F, // l
    0x00, 0x10, // m
    0x00, 0x11, // n
    0x00, 0x12, // o
    0x00, 0x13, // p
    0x00, 0x14, // q
    0x00, 0x15, // r
    0x00, 0x16, // s
    0x00, 0x17, // t
    0x00, 0x18, // u
    0x00, 0x19, // v
    0x00, 0x1A, // w
    0x00, 0x1B, // x
    0x00, 0x1C, // y
    0x00, 0x1D, // z
    0x02, 0x2F, // {
    0x02, 0x31, // |
    0x02, 0x30, // }
    0x02, 0x35, // ~
};
 
char out_dvorak[] = {
    0x00, 0x2C, // ' ' (space)
    0x02, 0x1E, // !
    0x02, 0x1F, // "
    0x02, 0x20, // #
    0x02, 0x21, // $
    0x02, 0x22, // %
    0x02, 0x23, // &
    0x00, 0x2D, // '
    0x02, 0x25, // (
    0x02, 0x26, // )
    0x02, 0x2E, // *
    0x02, 0x30, // +
    0x00, 0x36, // ,
    0x00, 0x38, // -
    0x00, 0x37, // .
    0x00, 0x24, // /
    0x00, 0x27, // 0
    0x00, 0x1E, // 1
    0x00, 0x34, // 2
    0x00, 0x20, // 3
    0x00, 0x21, // 4
    0x00, 0x22, // 5
    0x00, 0x23, // 6
    0x00, 0x24, // 7
    0x00, 0x25, // 8
    0x00, 0x26, // 9
    0x00, 0x34, // :
    0x00, 0x33, // ;
    0x02, 0x36, // <
    0x00, 0x2E, // =
    0x02, 0x37, // >
    0x02, 0x38, // ?
    0x02, 0x1F, // @
    0x02, 0x14, // A
    0x02, 0x05, // B
    0x02, 0x06, // C
    0x02, 0x07, // D
    0x02, 0x08, // E
    0x02, 0x09, // F
    0x02, 0x0A, // G
    0x02, 0x0B, // H
    0x02, 0x0C, // I
    0x02, 0x0D, // J
    0x02, 0x0E, // K
    0x02, 0x0F, // L
    0x02, 0x33, // M
    0x02, 0x11, // N
    0x02, 0x12, // O
    0x02, 0x13, // P
    0x02, 0x04, // Q
    0x02, 0x15, // R
    0x02, 0x16, // S
    0x02, 0x17, // T
    0x02, 0x18, // U
    0x02, 0x19, // V
    0x02, 0x1D, // W
    0x02, 0x1B, // X
    0x02, 0x1C, // Y
    0x02, 0x1A, // Z
    0x00, 0x2F, // [
    0x00, 0x31, // '\' (backslash)
    0x00, 0x30, // ]
    0x02, 0x10, // ^
    0x00, 0x2D, // _
    0x00, 0x35, // `
    0x00, 0x14, // a
    0x00, 0x05, // b
    0x00, 0x06, // c
    0x00, 0x07, // d
    0x00, 0x08, // e
    0x00, 0x09, // f
    0x00, 0x0A, // g
    0x00, 0x0B, // h
    0x00, 0x0C, // i
    0x00, 0x0D, // j
    0x00, 0x0E, // k
    0x00, 0x0F, // l
    0x00, 0x33, // m
    0x00, 0x11, // n
    0x00, 0x12, // o
    0x00, 0x13, // p
    0x00, 0x04, // q
    0x00, 0x15, // r
    0x00, 0x16, // s
    0x00, 0x17, // t
    0x00, 0x18, // u
    0x00, 0x19, // v
    0x00, 0x1D, // w
    0x00, 0x1B, // x
    0x00, 0x1C, // y
    0x00, 0x1A, // z
    0x02, 0x2F, // {
    0x02, 0x31, // |
    0x02, 0x30, // }
    0x02, 0x35, // ~
};
