#ifndef __KEY_HPP
#define __KEY_HPP
#include "main.h"
class Key {
private:
    uint8_t key_mode;
public:
    Key();
    uint8_t Read(void);//读IO
    /*IO变化return1,无变化return0*/
    uint8_t Change(void);
 
};


#endif


