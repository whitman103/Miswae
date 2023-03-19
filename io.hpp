#ifndef IO_H
#define IO_H

#include <type_traits>
#include <fstream>

class output_interface {
    public:
        output_interface(std::ofstream &stream) : stream_object(stream){};
        void putByte(char c){ this->stream_object.put(c); } 
    private:
        std::ofstream &stream_object;
};

class output_bits{
    public:
        output_bits(std::ofstream &stream) : out_object(output_interface(stream)), next_value(0), mask(0x80) {};
        ~output_bits(){
            if(this->mask!= 0x80) this->out_object.putByte(this->next_value);
        }
        void put_bit(const bool bit);

    private:
        output_interface out_object;
        char next_value;
        unsigned char mask;
};



class input_interface {
    public:
        input_interface(std::ifstream &stream) : stream_object(stream) {};
        int getByte(){ return this->stream_object.get(); };
    private:
        std::ifstream &stream_object;  
};




#endif