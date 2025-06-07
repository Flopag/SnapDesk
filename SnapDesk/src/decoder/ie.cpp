#include "decoder/ie.hpp"

using namespace decoder;

/* private */

bool Ie_node::is_valid_hex(const std::string str) const {
    for (char c : str) {
        if (!std::isxdigit(c)) {
            return false;
        }
    }
    return true;
}

/* Constructor */
Ie_node::Ie_node(uint8_t element_id, Big_number value) : element_id(element_id){
    next_element = nullptr;
    element_value = value;
}

Ie_node::~Ie_node(){
    if(next_element)
        delete next_element;
}

/* Public */

void Ie_node::add(Ie_node *new_element){
    if(!new_element)
        throw std::invalid_argument("No new parameter element");
                
    if(!next_element){
        next_element = new_element;
        return;
    }

    next_element->add(new_element);
}

Big_number Ie_node::get_value(std::string field) const {
    if (is_valid_hex(field)){
        uint8_t _element_id = std::stoi(field, nullptr, 16);
        if (element_id == _element_id)
            return element_value;
    } else {
        return Big_number::null();
    }

    if(next_element)
        return next_element->get_value(field);

    return Big_number::null();
};

void Ie_node::print(){
    /* Print element_id */

    if(!next_element)
        printf("└─"); 
    else
        printf("├─");

    switch (element_id) {
    case P_SSID:
        printf("SSID-------------------------: ");
        break;
    case P_SUPPORTED_RATES:
        printf("Supported rates--------------: ");
        break;
    case P_FH:
        printf("FH parameter set-------------: ");
        break;
    case P_DS:
        printf("DS parameter set-------------: ");
        break;
    case P_CF:
        printf("CF parameter set-------------: ");
        break;
    case P_TIM:
        printf("Traffic Indication Map (TIM)-: ");
        break;
    case P_IBSS:
        printf("IBSS parameter set-----------: ");
        break;
    case P_Challenge_text:
        printf("Challenge text---------------: ");
        break;
    default:
        printf("0x%02X-------------------------: ", element_id);
        break;
    }

    std::string value_string;

    switch (element_id){
    case P_SSID:
        value_string = element_value.char_string();
        break;
    default:
        value_string = element_value.hex_string();
        break;
    }

    printf("%s\n", value_string.c_str());

    if(next_element)
        next_element->print();
}