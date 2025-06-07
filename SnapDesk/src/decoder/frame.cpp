#include "decoder/frame.hpp"
#include "decoder/management_body.hpp"

using namespace decoder;

/* Constructor */

Frame::Frame(os_communicator::Communicator *_communicator){
    if(!_communicator)
        throw invalid_argument("No communicator given");
                
    communicator = _communicator;
    raw_buffer_size = FRAME_MAX_LENGTH;
    raw_frame_size = 0;
    raw_frame_buffer = new uint8_t[BEACON_FRAME_MAX_LENGTH];
    is_decoded = false;
    has_raw_data = false;
}

Frame::~Frame(){
    delete[] raw_frame_buffer;

    if(body)
        delete body;
}

/* Public */

bool Frame::get_is_decoded() const {
    return is_decoded;
}

void Frame::update_raw_data(){
    raw_frame_size = communicator->in_buffer(raw_frame_buffer, raw_buffer_size);

    is_decoded = false;

    if(raw_buffer_size < raw_frame_size)
        throw runtime_error("Size of the frame greater than the buffer");
                
    has_raw_data = true;
}

void Frame::print_raw_data(){
    if(!has_raw_data){
        printf("no data\n");
        return;
    }

    printf("0x");

    for(size_t i = 0; i < raw_frame_size; ++i)
        printf("%02X", raw_frame_buffer[i]);

    printf("\n");
}

void Frame::decode(){
    if(!has_raw_data)
        throw runtime_error("No raw data given"); 
    if(is_decoded)
        return;

    size_t cursor = 0;
    size_t remain_length = raw_frame_size;

    // Get MAC header
    frame_control = Big_number::from_buffer(raw_frame_buffer+cursor, remain_length, 2);
    cursor += 2;
    remain_length -= 2;
    duration = Big_number::from_buffer(raw_frame_buffer+cursor, remain_length, 2);
    cursor += 2;
    remain_length -= 2;
    destination_address = Big_number::from_buffer(raw_frame_buffer+cursor, remain_length, 6);
    cursor += 6;
    remain_length -= 6;
    source_address = Big_number::from_buffer(raw_frame_buffer+cursor, remain_length, 6);
    cursor += 6;
    remain_length -= 6;
    bssid = Big_number::from_buffer(raw_frame_buffer+cursor, remain_length, 6);
    cursor += 6;
    remain_length -= 6;
    sequence_control = Big_number::from_buffer(raw_frame_buffer+cursor, remain_length, 2);
    cursor += 2;
    remain_length -= 2;
    frame_check_sum = Big_number::from_buffer(raw_frame_buffer+raw_frame_size-4, 4, 4);

    // Get body
    if(body)
        delete body;

    Big_number tmp = frame_control.copy();
    tmp.cut_bit(2, 2);
    size_t type = tmp.to_size_t();
    
    tmp = frame_control.copy();
    tmp.cut_bit(4, 4);
    size_t sub_type = tmp.to_size_t();
    
    // -4 because last 4 bytes are FCS
    body = Body::get_body(raw_frame_buffer+cursor, remain_length-4, type, sub_type);

    is_decoded = true;
}

void Frame::print() const {
    if(!is_decoded)
        return;
                
    printf("Frame Header :\n");
    printf("├─Frame control----------------: %s\n", frame_control.hex_string().c_str());
    printf("├─Duration---------------------: %s\n", duration.hex_string().c_str());
    printf("├─Destination address----------: %s\n", destination_address.hex_string().c_str());
    printf("├─Source address---------------: %s\n", source_address.hex_string().c_str());
    printf("├─BSSID------------------------: %s\n", bssid.hex_string().c_str());
    printf("├─Sequence control-------------: %s\n", sequence_control.hex_string().c_str());
    printf("└─Frame check sum (FCS)--------: %s\n", frame_check_sum.hex_string().c_str());

    printf("\n");

    if(body)
        body->print();
    else
        printf("No body\n");
};

Big_number Frame::get_value(string field) const {
    if(!is_decoded)
        throw runtime_error("Frame must be encoded to get header value");

    size_t value = 0;

    if (field == "frame_control")
        return frame_control;
    else if (field == "duration")
        return duration;
    else if (field == "destination_address")
        return destination_address;
    else if (field == "source_address")
        return source_address;
    else if (field == "bssid")
        return bssid;
    else if (field == "sequence_control")
        return sequence_control;
    else if (field == "frame_check_sum")
        return frame_check_sum;
    else if(body)
        return body->get_value(field);
    
    return Big_number::null();
};

Body::Body(uint8_t *raw_body_buffer, size_t raw_buffer_size) : _raw_body_buffer(raw_body_buffer), _raw_buffer_size(raw_buffer_size) {
    if(!raw_body_buffer)
        throw invalid_argument("No buffer given");
                
    if(raw_buffer_size < 1)
        throw invalid_argument("Buffer size must be greater than 0");
}

Body *Body::get_body(uint8_t *raw_body_buffer, size_t raw_buffer_size, size_t type, size_t sub_type){
    Body *new_body = nullptr;
    switch(type)
    {
    case 0: // Management
        switch (sub_type)
        {
        case 8: // Beacon
            new_body = new Beacon_body(raw_body_buffer, raw_buffer_size);
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }

    if(!new_body)
        throw invalid_argument("No body found for type " + to_string(type) + " and sub_type " + to_string(sub_type));

    new_body->decode();

    return new_body;
}