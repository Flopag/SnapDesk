/**
 * @file ie.hpp
 * @author Pagano Florian
 * @brief Classes that permit to represent the IE of a frame's body
 * @version 0.1
 * @date 2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef BEACON_ELEMENT_HPP
#define BEACON_ELEMENT_HPP

#include <cstdint>
#include <stdexcept>
#include <stdexcept>
#include <map>
#include "decoder/big_number.hpp"

#define P_SSID 0 ///<element id 0 is the SSID
#define P_SUPPORTED_RATES 1 ///<element id 1 is the supported rated
#define P_FH 2 ///<element id 2 is the FH parameter set
#define P_DS 3 ///<element id 3 is the DS parameter set
#define P_CF 4 ///<element id 4 is the CF parameter set
#define P_TIM 5 ///<element id 5 is the TIM
#define P_IBSS 6 ///<element id 6 is the IBSS
#define P_Challenge_text 16 ///<element 16 is the challenge text

namespace decoder{
    /**
     * @brief An element of a linked list of IEs
     * @class Ie_node
     * 
     */
    class Ie_node {
        private:
            uint8_t element_id; ///<The element id of the IE
            Big_number element_value; ///<The value of the IE
            Ie_node *next_element; ///<A pointer to the next element of the linked list, or nullptr

            /**
             * @brief Look if the given string contains a hex number
             * 
             * @param str The string to verify
             * @return true if the string is a hex number
             * @return false if the string is not a hex number
             */
            bool is_valid_hex(const std::string str) const;
        
        public:
            /**
             * @brief Construct a new Ie_node object
             * 
             * @param element_id the element id of the new IE
             * @param value the value of the new IE
             */
            Ie_node(uint8_t element_id, Big_number value);
            /**
             * @brief Destroy the Ie_node object
             * 
             */
            ~Ie_node();

            /**
             * @brief Add a node to the linked list
             * 
             * @param new_element the new node to add
             */
            void add(Ie_node *new_element);
            /**
             * @brief Get the value of corresponding to a field
             * 
             * @param field the name of the field or its element id
             * @return Big_number the value that correcpond to the field or null if none
             */
            Big_number get_value(std::string field) const;
            /**
             * @brief Print the IE
             * 
             */
            void print();
    };
}

#endif