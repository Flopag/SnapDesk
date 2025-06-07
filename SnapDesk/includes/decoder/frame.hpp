/**
 * @file frame.hpp
 * @author Pagano Florian
 * @brief The file contains all basic frame class
 * @version 0.1
 * @date 2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef FRAME_HPP
#define FRAME_HPP

#include <cstdint>
#include <const.hpp>
#include <string>
#include <vector>

#include "os_communicator/os_communicator.hpp"
#include "decoder/big_number.hpp"

using namespace std;

namespace decoder{
    /**
     * @brief This class set the base of all frame's body possible
     * @class Body
     */
    class Body {
        protected:
            uint8_t *_raw_body_buffer; ///<The buffer that exactly contains the body
            size_t _raw_buffer_size; ///<The size of the buffer

            /**
             * @brief decode the body and fill the fields
             * 
             */
            virtual void decode() = 0;

        public:
            /**
             * @brief Construct a new Body object
             * 
             * @param raw_body_buffer a buffer containing the exact body
             * @param raw_buffer_size the size of the buffer
             */
            Body(uint8_t *raw_body_buffer, size_t raw_buffer_size);
            
            /**
             * @brief Print the content of the body
             * 
             */
            virtual void print() const = 0;
            /**
             * @brief Get the value corresponding to the field
             * 
             * @param field the name of the field or the element id corresponding to the value
             * @return Big_number the value or null if not found
             */
            virtual Big_number get_value(string field) const = 0;

            /**
             * @brief Give the Body objects corresponding to the type and subtype
             * 
             * @param raw_body_buffer a buffer containing the exact body
             * @param raw_buffer_size the size of the buffer
             * @param type the type of the frame (management, control, or data)
             * @param sub_type the subtype of the frame
             * @return Body* the new body corresponding to args
             */
            static Body *get_body(uint8_t *raw_body_buffer, size_t raw_buffer_size, size_t type, size_t sub_type);
    };

    /**
     * @brief The class that represent the whole frame
     * @class Frame
     * 
     */
    class Frame{
        private:
            os_communicator::Communicator *communicator; ///<the Communicator to the character device giving the frames
        protected:
            uint8_t *raw_frame_buffer; ///<the buffer containing the frame that is not decoded
            size_t raw_buffer_size; ///<the size of the buffer
            size_t raw_frame_size; ///<the size of the frame in the buffer
            bool is_decoded; ///< true if the buffer is decoded, false if not
            bool has_raw_data; ///< true if raw_frame_buffer is filled

            // Header
            Big_number frame_control = Big_number::null();
            Big_number duration = Big_number::null();
            Big_number destination_address = Big_number::null();
            Big_number source_address = Big_number::null();
            Big_number bssid = Big_number::null();
            Big_number sequence_control = Big_number::null();
            Big_number frame_check_sum = Big_number::null();

            // Body
            Body *body = nullptr; ///<the body of the frame

        public:
            /**
             * @brief Construct a new Frame object
             * 
             * @param _communicator Communicator on the character device
             */
            Frame(os_communicator::Communicator *_communicator);
            /**
             * @brief Destroy the Frame object
             * 
             */
            ~Frame();

            /**
             * @brief Get the value of is_decoded
             * 
             * @return true if is_decoded is true
             * @return false if is_decoded is false
             */
            bool get_is_decoded() const;
            /**
             * @brief Get a new frame from the character device and put it in raw_frame_buffer
             * 
             */
            void update_raw_data();
            /**
             * @brief Print the raw_frame_buffer as hex bytes
             * 
             */
            void print_raw_data();
            /**
             * @brief Decode the raw_frame_buffer to fill the different fields
             * 
             */
            virtual void decode();
            /**
             * @brief Print the content of the decoded frame
             * 
             */
            virtual void print() const;
            /**
             * @brief Get the value corresponding to the field
             * 
             * @param field the name of the field that we want to get
             * @return Big_number the field value, or null if not found
             */
            Big_number get_value(string field) const;
    };
}

#endif