#include "compiler/compiler.hpp"

using namespace compiler;

size_t Compiler::read_line(executable_tree::Node *current_node, size_t next_line) const{
    executable_tree::Node *new_node = nullptr;
    executable_tree::Node *aux_function_node = nullptr;
    while(!new_node){
        std::string line = _communicator->get_line(next_line);

        char first_char = line.front();
        char last_char = line.back();

        if(line == ""){
            // end of file
            break;
        } else if (first_char == '#'){
            next_line++;
        } else if(first_char == '}' && line.size() == 1) {
            // end of function
            if(!aux_function_node)
                throw runtime_error("End of function without begin it, line " + std::to_string(next_line+1));
            new_node = aux_function_node;
            aux_function_node = nullptr;
            break;
        } else if(aux_function_node) {
            // if we are in the function
            next_line = read_line(aux_function_node, next_line);
        } else if(last_char == '{'){
            // begin of function
            if(line == "Sha256 {")
                aux_function_node = new executable_tree::Sha256();
            else if(line == "Cut_bit {")
                aux_function_node = new executable_tree::Cut_bit();
            else if(line == "Cut_byte {")
                aux_function_node = new executable_tree::Cut_byte();
            else
                throw runtime_error("fonction " + line + " does not exist!");
                        
            next_line = read_line(aux_function_node, next_line+1);
        } else if(first_char == '>'){
            // Getter
            line.erase(0, 1); // get rid of >
            new_node = new executable_tree::Getter(line);
            break;
        } else {
            // Value
            new_node = new executable_tree::Value(line);
            break;
        }
    }
    if(aux_function_node)
        throw runtime_error("Function beggined but not ended");
    if(new_node)
        current_node->add_node(new_node);

    return next_line+1;
};

executable_tree::Node *Compiler::get_executable_tree() const {
    executable_tree::Root *root_node = new executable_tree::Root();

    size_t next_line = read_line(root_node, 0);

    // error if code remaining
    std::string line = _communicator->get_line(next_line);
    if(line != "")
        throw runtime_error("Too much code, from line " + std::to_string(next_line+1) + " => " + line);

    return root_node;
};