/**
 * @file const.hpp
 * @author Pagano Florian
 * @brief Contains all constant that are used in snapdesk
 * @version 0.1
 * @date 2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef CONST_HPP
#define CONST_HPP

#define FRAME_HEADER_MIN_LENGTH 24 ///<The min length of the header of a frame
#define FRAME_HEADER_MAX_LENGTH 36 ///<The max length of the header of a frame

#define BEACON_FRAME_MAX_LENGTH 2346 ///<The max length of a beacon frame
#define BEACON_FRAME_BODY_MIN_LENGTH 12 ///<The min length of the body of a beacon frame

// To change whith true value
#define FRAME_MAX_LENGTH BEACON_FRAME_MAX_LENGTH ///<The max length of a frame 

#define DATABASE_ROOT "database" ///<The root directory name where logs will be saved

#endif
