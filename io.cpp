#include "io.hpp"

void output_bits::put_bit(const bool bit){
    if(bit) this->next_value|=this->mask;
    this->mask>>=1;
    if(!this->mask){
        this->out_object.putByte(this->next_value);
        this->mask = 0x80;
        this->next_value = 0;
    }
}