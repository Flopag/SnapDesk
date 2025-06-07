#include "compiler/function_node.hpp"

using namespace executable_tree;

std::string Sha256::to_string(size_t depth) const {
    std::string output = Node::to_string(depth);
                
    output += "SHA()\n";
    output += Function::to_string(depth);

    return output;
};

std::string Sha256::get_value(const decoder::Frame *target_frame) const {
    Node::get_value(target_frame);

    std::string data = "";
    for(Node* arg : args){
        data += arg->get_value(target_frame);
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to initialize digest");
    }

    if (EVP_DigestUpdate(mdctx, data.c_str(), data.size()) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to update digest");
    }

    if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to finalize digest");
    }

    std::string hash_string = "";
    for(size_t i = 0; i < hash_len; ++i){
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", hash[i]);
        hash_string += buf;
    }

    return hash_string;
};

std::string Cut_bit::to_string(size_t depth) const {
    std::string output = Node::to_string(depth);
                
    output += "Cut_bit()\n";
    output += Function::to_string(depth);

    return output;
};

std::string Cut_bit::get_value(const decoder::Frame *target_frame) const {
    Node::get_value(target_frame);

    decoder::Big_number value = decoder::Big_number::null();
    size_t first_bit;
    size_t cut_length;

    size_t i = 0;
    for(Node* arg : args){
        std::string arg_value = arg->get_value(target_frame);

        switch (i){
        case 0:
            value = decoder::Big_number::from_hex_string(arg_value);
            break;
        case 1:
            first_bit = std::stoul(arg_value);
            break;
        case 2:
            cut_length = std::stoul(arg_value);
            break;
        default:
            throw runtime_error("Too many args in Cut_bit");
        }

        ++i;
    }

    if(i < 3)
        throw runtime_error("Too few args in Cut_bit");

    if(!value.is_null())
        value.cut_bit(first_bit, cut_length);
    else
        return "";

    return value.hex_string();
};

std::string Cut_byte::to_string(size_t depth) const {
    std::string output = Node::to_string(depth);
                
    output += "Cut_byte()\n";
    output += Function::to_string(depth);

    return output;
};

std::string Cut_byte::get_value(const decoder::Frame *target_frame) const {
    Node::get_value(target_frame);

    decoder::Big_number value = decoder::Big_number::null();
    size_t first_byte;
    size_t cut_length;

    size_t i = 0;
    for(Node* arg : args){
        std::string arg_value = arg->get_value(target_frame);

        switch (i){
        case 0:
            value = decoder::Big_number::from_hex_string(arg_value);
            break;
        case 1:
            first_byte = std::stoul(arg_value);
            break;
        case 2:
            cut_length = std::stoul(arg_value);
            break;
        default:
            throw runtime_error("Too many args in Cut_byte");
        }

        ++i;
    }

    if(i < 3)
        throw runtime_error("Too few args in Cut_byte");

    if(!value.is_null())
        value.cut_byte(first_byte, cut_length);
    else
        return "";

    return value.hex_string();
};