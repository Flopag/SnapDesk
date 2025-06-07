#include "compiler/node.hpp"

using namespace executable_tree;

/* Node */

std::string Node::get_value(const decoder::Frame *target_frame) const {
    if(!target_frame)
        throw invalid_argument("No frame given");
    if(!target_frame->get_is_decoded())
        throw invalid_argument("Given frame is not decoded");
                
    return "";
};

void Node::add_node(Node* arg) {
    throw runtime_error("This type of node cannot have children");
};

std::string Node::to_string(size_t depth) const {
    std::string output = "";

    for(size_t i=0; i < depth; ++i)
        output += "-";

    return output;
};

/* Root */

std::string Root::get_value(const decoder::Frame *target_frame) const {
    Node::get_value(target_frame);

    if(!_first_node)
        throw runtime_error("No tree");

    std::string value;

    try{
        value = _first_node->get_value(target_frame);
    }
    catch(const std::exception& e){
        std::string message = "Runtime error in code: ";
        message += e.what();
        throw runtime_error(message);
    }
    

    return value;
};

void Root::add_node(Node* arg) {
    if(!arg)
        throw invalid_argument("no arg given");
                
    if(_first_node)
        throw runtime_error("root node had already a child");

    _first_node = arg;
}

std::string Root::to_string(size_t depth) const {
    if(_first_node)
        return _first_node->to_string(depth);
                
    return "No tree";
};

/* Function */

void Function::add_node(Node* arg) {
    if(!arg)
        throw invalid_argument("no arg given");

    args.push_back(arg);
}

std::string Function::to_string(size_t depth) const {
    std::string output = "";

    for(Node* arg : args)
        output += arg->to_string(depth+1);
                
    return output;
};

/* Value */

std::string Value::get_value(const decoder::Frame *target_frame) const {
    Node::get_value(target_frame);
    return value;
};

void Value::add_node(Node* arg) {
    Node:add_node(arg);
};

std::string Value::to_string(size_t depth) const {
    std::string output = Node::to_string(depth);

    output += value + "\n";
                
    return output;
};

/* Getter */

std::string Getter::get_value(const decoder::Frame *target_frame) const {
    Node::get_value(target_frame);

    decoder::Big_number value;

    value = target_frame->get_value(field_name);
                
    if(value.is_null())
        return "";
                
    return value.hex_string();
};    

void Getter::add_node(Node* arg) {
    Node:add_node(arg);
};  

std::string Getter::to_string(size_t depth) const {
    std::string output = Node::to_string(depth);

    output += "get -> ";

    output += field_name + "\n";

    return output;
};