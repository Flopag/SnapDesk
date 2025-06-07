/**
 * @file os_communicator.hpp
 * @author Pagano Florian
 * @brief The os_communicator component handle all interaction between snapdesk and the current os
 * @version 0.1
 * @date 2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef OS_COMMUNICATOR_HPP
#define OS_COMMUNICATOR_HPP

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>
#include <filesystem>

#define F_NONE 0 ///<A file that does not exist
#define F_FILE 1 ///<A file
#define F_FOLDER 2 ///<A folder

using namespace std;

namespace os_communicator{

    /**
     * @brief The Communicator is the main class of the os_communicator component
     * @class Communicator
     * 
     */
    class Communicator{
        private:
            string _file_name; ///< The name of the file which the object will interact with
        
        public:
            /**
             * @brief Construct a new Communicator object
             * 
             * @param file_name The name of the file to interact with
             */
            Communicator(string file_name);

            /**
             * @brief Say if the given file exist
             * 
             * @return true if the file exist
             * @return false if the file does not exist
             */
            bool exist();
            /**
             * @brief Copy the content of the file into the given buffer
             * 
             * @param buffer the buffer where the data will be copied
             * @param buffer_size the size of the buffer
             * @return size_t the number of byte read
             */
            size_t in_buffer(uint8_t *buffer, const size_t buffer_size);
            /**
             * @brief Get the line number line_number from the file
             * 
             * @param line_number the number of the line from file to get
             * @return string the line
             */
            string get_line(size_t line_number) const;
            /**
             * @brief Add a line at the end of the file
             * 
             * @param line the line to add to the end of the file
             */
            void add_line(string line);
            /**
             * @brief Replace the line line_number with the line new_line
             * 
             * @param line_number the number of the file to replace
             * @param new_line the line that will replace the old one
             */
            void replace_line(size_t line_number, string new_line);
            /**
             * @brief Create an empty file
             * 
             */
            void new_file();

            /**
             * @brief Get the current date
             * 
             * @return string the current date
             */
            static string get_current_date();
            /**
             * @brief Notify the user with a message
             * 
             * @param message the message that will be displayed
             */
            static void notify(string message);
            /**
             * @brief Pause the process for an amount of time
             * 
             * @param seconds the number of seconds to pause the process
             */
            static void sleep(size_t seconds);
            
            /**
             * @brief Create a folder
             * 
             * @param folder_name the name of the folder
             */
            static void create_folder(string folder_name);
            /**
             * @brief Get the type of a file (file, folder, nothing)
             * 
             * @param name the name of the file
             * @return size_t the type of the file
             */
            static size_t get_file_type(string name);
    };
}

#endif