#include "decoder/big_number.hpp"

using namespace decoder;

Big_number& Big_number::operator=(const std::vector<uint8_t> _number) {
    number.clear();

    number = _number;

    _is_null = false;

    return *this;
};

bool Big_number::is_null() const {
    return _is_null;
};

void Big_number::throw_if_null() const {
    if(is_null())
        throw std::invalid_argument("use of null number");
};

std::string Big_number::hex_string() const {
    throw_if_null();

    std::string output = "";

    for(uint8_t byte : number){
        char buffer[3]; // 2 digits + \0
        sprintf(buffer, "%02X", byte);
        output += buffer;
    }

    return output;
};

std::string Big_number::char_string() const {
    throw_if_null();

    std::string output = "";

    for(uint8_t byte : number){
        char buffer[2]; // 1 char + \0
        sprintf(buffer, "%c", byte);
        output += buffer;
    }

    return output;
};

size_t Big_number::to_size_t() const {
    throw_if_null();

    if(number.size() > sizeof(size_t))
        throw std::invalid_argument("number too big to fit in size_t");

    size_t output = 0;

    for(size_t i = 0; i < number.size(); ++i){
        output += number[i] << (8*(number.size()-1-i));
    }

    return output;
}

void Big_number::cut_byte(size_t from, size_t size) {
    this->throw_if_null();

    if(from+size > this->number.size())
        throw std::invalid_argument("cut out of range");
    if(size == 0){
        _is_null = true;
        return;
    }

    number.erase(number.begin(), number.end() - (from+size));

    number.erase(number.begin() + size, number.end());
}

void Big_number::cut_bit(size_t from, size_t size) {
    this->throw_if_null();
    if(from+size > this->number.size()*8)
        throw std::invalid_argument("cut out of range");

    size_t byte_from = (from-(from%8)) / 8;
    size_t byte_size = ((from+size)-((from+size)%8)) / 8 - byte_from + (((from+size)%8) > 0 ? 1 : 0);

    this->cut_byte(byte_from, byte_size);

    if(is_null())
        return;

    size_t bits_to_remove_after = from % 8;
    size_t bits_to_remove_before = number.size() * 8 - size - bits_to_remove_after;

    for(size_t i=0; i<bits_to_remove_before; ++i){
        uint8_t block = number[0];

        block = block & (~(1 << (7-i)));

        number[0] = block;
    }

    for(size_t i=0; i < bits_to_remove_after; ++i){
        bool previous_value = false;
        for(size_t j=0; j < number.size(); ++j){
            uint8_t block = number[j];

            bool last_value = (block%2 == 1) ? true : false;

            block = block >> 1;

            if(previous_value)
                block = block | 0x80;
            else
                block = block & 0x7F;
                        

            number[j] = block;

            previous_value = last_value;
        }
    }

    if(bits_to_remove_before + bits_to_remove_after >= 8)
        number.erase(number.begin(), number.begin()+1);
}

Big_number Big_number::copy() const{
    Big_number copy;

    if(is_null())
        return Big_number::null();

    copy = number;

    return copy;
}

Big_number& Big_number::operator=(const Big_number _number) {
    number.clear();

    number = _number.number;
    _is_null = _number._is_null;

    return *this;
};

Big_number Big_number::null() {
    Big_number big_number;
    big_number._is_null = true;

    return big_number;
};

Big_number Big_number::from_buffer_inv(const uint8_t *buffer, const size_t buffer_size, const size_t number_size) {
    std::vector<uint8_t> number;

    if(number_size > buffer_size)
        throw std::invalid_argument("given buffer smaller that number size");

    for(size_t i = 0; i < number_size; ++i){
        number.push_back(buffer[i]);
    }

    Big_number big_number;
    big_number = number;

    return big_number;
}

Big_number Big_number::from_buffer(const uint8_t *buffer, const size_t buffer_size, const size_t number_size) {
    std::vector<uint8_t> number;

    if(number_size > buffer_size)
        throw std::invalid_argument("given buffer smaller that number size");

    for(int i = number_size-1; i >= 0; --i){
        number.push_back(buffer[i]);
    }

    Big_number big_number;
    big_number = number;

    return big_number;
}

Big_number Big_number::from_hex_string(const std::string string){
    std::vector<uint8_t> number;

    if(string.size() == 0)
        return Big_number::null();

    const char *c_string = string.c_str();

    for(size_t i = 0; i < string.size(); i += 2){
        char temp[2];
        sprintf(temp, "%c%c", string.c_str()[i], string.c_str()[i+1]);
        number.push_back((uint8_t) std::stoul(temp, nullptr, 16));
    }

    Big_number big_number;
    big_number = number;

    return big_number;
}