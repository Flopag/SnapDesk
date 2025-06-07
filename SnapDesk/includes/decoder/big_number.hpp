/**
 * @file big_number.hpp
 * @author Florian Pagano
 * @brief 
 * @version 0.1
 * @date 2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef BIG_NUMBER_HPP
#define BIG_NUMBER_HPP

#include <vector>
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <string>


namespace decoder{
    /**
     * @brief This class represent and manipulate indifined sized numbers using bytes
     * @class Big_number
     * 
     */
    class Big_number {
        private:
            std::vector<uint8_t> number; ///<The number byte per byte
            bool _is_null = false; ///<is true if the number is null, false if it has a value

            /**
             * @brief Create a new Big_number given a number vector
             * 
             * @param _number The value of the Big_number
             * @return Big_number& The new Big_number corresponding to _number
             */
            Big_number& operator=(const std::vector<uint8_t> _number);
        
        public:
            /**
             * @brief Look if the number is null or not
             * 
             * @return true if the number is null
             * @return false if the number is not null
             */
            bool is_null() const;
            /**
             * @brief Throw an error if the number is null
             * 
             */
            void throw_if_null() const;
            /**
             * @brief transform the number into a string containing the hex of it
             * 
             * @return std::string the string containing the hexs
             */
            std::string hex_string() const;
            /**
             * @brief transform the number into a string containing the char of it
             * 
             * @return std::string the string containing the chars
             */
            std::string char_string() const;
            /**
             * @brief Transform the number into a size_t number (only if the number can be contained in a size_t)
             * 
             * @return size_t the number corresponding to the Big_number
             */
            size_t to_size_t() const;
            /**
             * @brief Cut the number to obtain only the given interval (unit byte)
             * 
             * @param from the first byte of the new number
             * @param size the size of the new number
             */
            void cut_byte(size_t from, size_t size);
            /**
             * @brief Cut the number to obtain only the given interval (unit bit)
             * 
             * @param from the first bit of the new number
             * @param size the size of the new number
             */
            void cut_bit(size_t from, size_t size);
            /**
             * @brief Clone the Big_number object
             * 
             * @return Big_number the clone of the object
             */
            Big_number copy() const;

            /* Operators */

            /**
             * @brief Copy the value of a Big_number object into another
             * 
             * @param _number the Big_number object from where the value will be copied
             * @return Big_number& the Big_number where the value has been copied
             */
            Big_number& operator=(const Big_number _number);

            /* Constructors */

            /**
             * @brief Create a instance of Big_number containing the null value
             * 
             * @return Big_number the new instance
             */
            static Big_number null();
            /**
             * @brief Create a instance of Big_number containing the number from the given buffer
             * 
             * @param buffer the buffer containing the number, starting at the first number byte
             * @param buffer_size the size of the buffer
             * @param number_size the size of the number inside the buffer
             * @return Big_number the new instance with the value of the number in it
             */
            static Big_number from_buffer(const uint8_t *buffer, const size_t buffer_size, const size_t number_size);
            /**
             * @brief Exactly the same as from_buffer() but with inverted bytes
             * 
             * @param buffer the buffer containing the number, starting at the first number byte
             * @param buffer_size the size of the buffer
             * @param number_size the size of the number inside the buffer
             * @return Big_number the new instance containing the value of the buffer
             */
            static Big_number from_buffer_inv(const uint8_t *buffer, const size_t buffer_size, const size_t number_size);
            /**
             * @brief Create a instance of Big_number containing the number from a given string
             * 
             * @param string the string containing the bytes in hex form
             * @return Big_number the new instance containing the value of the number
             */
            static Big_number from_hex_string(const std::string string);
    };
}

#endif