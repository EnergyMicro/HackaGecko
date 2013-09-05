#ifndef _SI1143_H_
#define _SI1143_H_

void Si1143_Init(void);
bool Si1143_ReadPos(int *ret_x, int *ret_y);
bool Si1143_NewEvent(void);

#endif
