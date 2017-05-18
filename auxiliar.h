#ifndef AUX_H
#define AUX_H

void initBT();
void setMotors(int motor_left, int motor_right);
long readUS();
int readBT_pack(int leitura[9]);
int readBT();

#endif
