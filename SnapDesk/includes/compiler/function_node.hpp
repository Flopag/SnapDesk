/**
 * @file function_node.hpp
 * @author Pagano Florian
 * @brief 
 * @version 0.1
 * @date 2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef FUNCTION_NODE_HPP
#define FUNCTION_NODE_HPP

#include "compiler/node.hpp"
#include "decoder/big_number.hpp"

#include <openssl/evp.h>
#include <string>

namespace executable_tree{
    /**
     * @brief This Function concatenates its arguments and 
     * @class Sha256
     * 
     */
    class Sha256 : public Function {
        public:
            std::string to_string(size_t depth) const override;

            std::string get_value(const decoder::Frame *target_frame) const override;
    } ;

    /**
     * @brief This Function cut the bits of the first arg from the second arg of the size of the third arg
     * 
     */
    class Cut_bit : public Function {
        public:
            std::string to_string(size_t depth) const override;

            std::string get_value(const decoder::Frame *target_frame) const override;
    } ;

    /**
     * @brief This Function cut the bytes of the first arg fril the secong arg of the size of the third arg
     * 
     */
    class Cut_byte : public Function {
        public:
            std::string to_string(size_t depth) const override;

            std::string get_value(const decoder::Frame *target_frame) const override;
    } ;
}

#endif