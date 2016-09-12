#pragma once

void interrupt0();
void interrupt1();
void interrupt2();
void interrupt3();
void interrupt4();
void interrupt5();
void interrupt6();
void interrupt7();

static void (*interrupt[8])() = { interrupt0, 
                                   interrupt1, 
                                   interrupt2, 
                                   interrupt3, 
                                   interrupt4, 
                                   interrupt5, 
                                   interrupt6, 
                                   interrupt7};
