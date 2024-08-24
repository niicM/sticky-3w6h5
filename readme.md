To reset do this:

rm -rf build
mkdir build
cd build
export PICO_SDK_PATH=../../pico-sdk
cmake ..
make VERBOSE=1




cp ~/Code/c/keyboard_project/e_* ~/Code/c/rp2040/my-keyboard/source/