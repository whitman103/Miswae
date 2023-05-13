#ifndef IO_HPP
#define IO_HPP


struct OutCacheString{
    std::string out="";
    uint8_t cache, count;
    OutCacheString() : cache(0), count(0), out(""){};
    void append(const int bit){
        this->cache<<= 1;
        this->cache |= bit;
        this->count +=1;
        if(count ==8){
            this->out.append(reinterpret_cast<const char *>(&this->cache),1);
            this->count = 0;
        }
    }

    void flush(){
        if(this->count>0){
            for(int i = count; i<8; ++i){
                this->append(0);
            }
            assert(this->count==0);
        }
    }

    void append_bit_and_pending(const int bit, uint64_t &pending_bits){
        this->append(bit);
        while(pending_bits>0){
            this->append(!bit);
            pending_bits -= 1;
        }
    }
};

struct InCacheString{
    explicit InCacheString(const std::string& in): in_(in){};

    uint8_t cache=0;
    uint8_t cached_bits=0;
    size_t in_ptr = 0;

    void get(uint32_t& value){
        if(this->cached_bits == 0){
            if(this->in_ptr == this->in_.size()){
                value <<= 1;
                return;
            }

        this->cache = (uint8_t) in_[this->in_ptr];
        this->in_ptr++;
        this->cached_bits = 8;
        }
        value <<= 1;
        value |= (this->cache >> (this->cached_bits -1))& 1;
        this->cached_bits--;
    }

    void initialize(uint32_t& value, const int bitNumber){
        for(int i = 0; i<bitNumber; ++i){
            this->get(value);
        }
    }

    void print_held(){std::cout<<this->in_<<std::endl;}
    const std::string in_;
};



#endif