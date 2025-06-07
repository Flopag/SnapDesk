/**
 * @file management_body.hpp
 * @author Pagano Florian
 * @brief 
 * @version 0.1
 * @date 2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef MANAGEMENT_BODY_HPP
#define MANAGEMENT_BODY_HPP

#include <cstdint>
#include <const.hpp>
#include <string>
#include "decoder/frame.hpp"
#include "decoder/ie.hpp"
#include "decoder/big_number.hpp"

using namespace std;

namespace decoder{
    /**
     * @brief Is the body of a beacon frame
     * @class Beacon_body
     * 
     */
    class Beacon_body : public Body {
        private:
            Big_number _timestamp; ///<The timestamp fixed fields
            Big_number _beacon_interval; ///<The beacon interval fixed fields
            Big_number _capabilities_information; ///<The capabilities information fixed fields
            Ie_node *_first_ie = nullptr; ///<The linked list of body's IEs

            /**
             * @brief decode the body and fill the fields
             * 
             */
            void decode() override;
            /**
             * @brief Add an IE to the linked list
             * 
             * @param element_id the element id of the IE
             * @param element_length The byte length of the IE content
             * @param start_position The position of the IE content in the buffer
             */
            void add_ie(uint8_t element_id, uint8_t element_length, size_t start_position);

        public:
            /**
             * @brief Construct a new Beacon_body object
             * 
             * @param raw_body_buffer a buffer containing the exact beacon body
             * @param raw_buffer_size the size of the buffer
             */
            Beacon_body(uint8_t *raw_body_buffer, size_t raw_buffer_size);
            /**
             * @brief Destroy the Beacon_body object
             * 
             */
            ~Beacon_body();

            /**
             * @brief Print the content of the beacon body
             * 
             */
            void print() const override;
            /**
             * @brief Get the value of a specific field
             * 
             * @param field the name of the field, or the element id corresponding to an IE
             * @return Big_number the value, or null if this do not exist
             */
            Big_number get_value(string field) const override;
    };
}

#endif