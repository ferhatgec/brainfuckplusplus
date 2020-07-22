/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include <stdio.h>
#include <stdlib.h>
#include "BrainfuckPlusPlus.h"

char *code = "++++++++++[>+>+++>+++++++>++++++++++<<<<-]>>>++.>+.+++++++..+++.<<++.>------.>+++.-----------------.++++++++.+++++.--------.+++++++++++++++.------------------.++++++++.<++++++++++++++.>+.+++++++++.--.<.>-------.+++++++++.--.<<+.";

int main() {
    BfInterpreter(code);
    return 0;
}
