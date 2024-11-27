## Flash uf2

- (1) Hold the boot sel button (middle one)
- (2a) Plug (to turn on) the keyboard (before releasing the boot sel button)
- (2b) Another option (instead of 2a) is pressing the other (reset) button.
  Whatever is easier.

## Compile

cd ~/Code/c/rp2040/my-keyboard/build
make

- The uf2 file is in ./build/source.uf2

## To reset do this:

rm -rf build
mkdir build
cd build
export PICO_SDK_PATH=../../pico-sdk
cmake ..
make VERBOSE=1

## To copy over the e_* part

- Into this project
cp ~/Code/c/keyboard_project/e_* ~/Code/c/rp2040/my-keyboard/source/

- From this project
cp ~/Code/c/rp2040/my-keyboard/source/e_* ~/Code/c/keyboard_project/

