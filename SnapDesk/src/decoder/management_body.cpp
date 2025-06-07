#include "decoder/management_body.hpp"

using namespace decoder;

void Beacon_body::decode(){
    size_t cursor = 0;
    size_t remain_length = _raw_buffer_size;

    // Get Timestamp
    _timestamp = Big_number::from_buffer(_raw_body_buffer+cursor, remain_length, 8);
    cursor += 8;
    remain_length -= 8;

    // Get Beacon Interval
    _beacon_interval = Big_number::from_buffer(_raw_body_buffer+cursor, remain_length, 2);
    cursor += 2;
    remain_length -= 2;

    // Get Capabilities Information
    _capabilities_information = Big_number::from_buffer(_raw_body_buffer+cursor, remain_length, 2);
    cursor += 2;
    remain_length -= 2;

    // Get Parameters
    while(cursor < _raw_buffer_size){
        uint8_t element_id = _raw_body_buffer[cursor];
        uint8_t element_length = _raw_body_buffer[cursor+1];

        // Early stopping if end of body reached
        if(element_length == 0)
            break;

        // Stop if buffer overflow
        if(!(element_length+cursor < _raw_buffer_size))
            break;

        add_ie(element_id, element_length, cursor+2);  

        cursor += 2 + element_length;
    }
}

void Beacon_body::add_ie(uint8_t element_id, uint8_t element_length, size_t start_position){
    if(start_position+element_length > _raw_buffer_size)
        throw invalid_argument("element outside buffer");

    Ie_node *new_ie = 
        new Ie_node(
            element_id, 
            Big_number::from_buffer_inv(_raw_body_buffer+start_position, _raw_buffer_size-start_position, element_length)
        );

    if(!_first_ie)
        _first_ie = new_ie;
    else
        _first_ie->add(new_ie);
}

Beacon_body::Beacon_body(uint8_t *raw_body_buffer, size_t raw_buffer_size) : Body(raw_body_buffer, raw_buffer_size) {}

Beacon_body::~Beacon_body(){
    if(_first_ie)
        delete _first_ie;
}

void Beacon_body::print() const{
    printf("Beacon body :\n");

    printf("Fixed parameters :\n");
    printf("├─Timestamp--------------------: %s\n", _timestamp.hex_string().c_str());
    printf("├─Beacon Interval--------------: %s\n", _beacon_interval.hex_string().c_str());
    printf("└─Capabilities Information-----: %s\n", _capabilities_information.hex_string().c_str());

    printf("\nIEs :\n");

    if(_first_ie)
        _first_ie->print();

    printf("\n");
    printf("==============================\n");
}

Big_number Beacon_body::get_value(string field) const{
    Big_number value;

    if(field == "timestamp")
        value = _timestamp;
    else if(field == "beacon_interval")
        value = _beacon_interval;
    else if(field == "capabilities_information")
        value = _capabilities_information;
    else if(_first_ie)
        value = _first_ie->get_value(field);
    else
        value = Big_number::null();

    return value;
}