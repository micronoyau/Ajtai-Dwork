# Ajtai-Dwork

This is an implementation of Ajtai-Dwork public key cryptosystem in C.

For more details about the theory behind it, please visit http://user.math.uzh.ch/rosenthal/masterthesis/07711484/Hartmann2015.pdf .

Note that the pseudorandom generator used is not cryptographically secure.

To compile and run, you can run gcc main.c crypto.c linearalgebra.c -lm -o run && ./run (on Linux).
