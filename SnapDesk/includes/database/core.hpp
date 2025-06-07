/**
 * @file core.hpp
 * @author Pagano Florian
 * @brief Contains core classes to manipulate the database
 * @version 0.1
 * @date 2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef CORE_HPP
#define CORE_HPP

#include <string>
#include <vector>
#include <algorithm>

#include <openssl/evp.h>

#include "os_communicator/os_communicator.hpp"
#include "const.hpp"

// Number of line that are not raw data
#define HEADER_SIZE 1
#define DELIMITER ";"
#define DATA_EXTENTION ".csv"
#define METADATA_EXTENTION ".metadata"
#define KEY_LINE_NUMBER 0
#define COLUMN_NUMBER_LINE_NUMBER 1
#define COLUMN_NAMES {"output", "creation_date", "last_date"}
#define KEY_COLUMN_NUMBER 0

namespace database{
    /**
     * @brief This class represent and operates a csv file
     * @class Csv
     * 
     */
    class Csv{
        private:
            size_t _number_of_columns; ///<The number of columns in the file
            size_t _key_column_number; ///<The column number of the key column
            os_communicator::Communicator *_c_metadata; ///<The Communicator instance to the metadata file
            os_communicator::Communicator *_c_data; ///<The Communicator instance to the csv file

            /**
             * @brief Throw an error if the given values vector is not at the right size
             * 
             * @param values the vector of values to verify
             */
            void _test_values_size(std::vector<std::string> values);
            /**
             * @brief Get the line number of a value corresponding to a key
             * 
             * @param key_value the value to look for
             * @return size_t the line number of the targeted key or -1 if not found
             */
            size_t _get_key_position(std::string key_value);
            /**
             * @brief Tranform a vector of values into a single string line
             * 
             * @param values the values in form of vector
             * @return std::string the values in form of line
             */
            static std::string _values_to_line(std::vector<std::string> values);
            /**
             * @brief Give the position of the targeted column name from the csv header
             * 
             * @param header the csv header containing all column name
             * @param column_name the targeted column name
             * @return size_t the number of the column, or -1 if not found
             */
            size_t _get_column_position_from_header(std::string header, std::string column_name);
            /**
             * @brief Get a specific cell of a given line
             * 
             * @param row the line containing the row where we get the cell
             * @param column_number the column number corresponding to the cell
             * @return std::string the cell value
             */
            static std::string _get_cell_from_row(std::string row, size_t column_number);
            /**
             * @brief convert a string decimal number into a size_t
             * 
             * @param number the number as a string containing a decimal number
             * @return size_t the decimal number as a size_t
             */
            size_t _string_to_size_t(std::string number);

        public:
            /**
             * @brief Construct a new Csv object on a file that does exist
             * 
             * @param file_name the name of the storage file (without extentions)
             */
            Csv(std::string file_name);
            /**
             * @brief Destroy the Csv object
             * 
             */
            ~Csv();
            /**
             * @brief Construct a new Csv object on a file that does not exist
             * 
             * @param file_name the filename of the new storage file (without extentions)
             * @param column_names the names of the columns
             * @param key_column_number the column number of the key
             * @return Csv* the new Csv instance of the new storage file
             */
            static Csv *create(std::string file_name, std::vector<std::string> column_names, size_t key_column_number);

            /**
             * @brief Create a new entry in the file
             * 
             * @param values the vector of values to add
             */
            void push_row(std::vector<std::string> values);
            /**
             * @brief Replace a row by another
             * 
             * @param row_number the number of the row to replace
             * @param values the vector of values that will replace
             */
            void replace_row(size_t row_number, std::vector<std::string> values);
            /**
             * @brief Get the value of a cell
             * 
             * @param row_number the row number of the cell
             * @param column_number the column number of the cell
             * @return std::string the value of the cell
             */
            std::string get_cell(size_t row_number, size_t column_number);
            /**
             * @brief Get the row number of a given key value
             * 
             * @param key_value the value to match with the key
             * @return size_t the row number of the given key value, or -1 if not found
             */
            size_t get_row_number(std::string key_value);
            /**
             * @brief Get the column number from its name
             * 
             * @param column_name the name of the column
             * @return size_t the column number, or -1 if not found
             */
            size_t get_column_number(std::string column_name);
            /**
             * @brief Get all key values of a file
             * 
             * @return std::vector<std::string> a vector containing all the key values
             */
            std::vector<std::string> get_all_keys();
            /**
             * @brief Get all key values that has a value on a given column
             * 
             * @param column_number the number of the column to look for matches
             * @param value the value to compare
             * @return std::vector<std::string> a vector containing all the key values 
             */
            std::vector<std::string> get_all_keys_with_value(size_t column_number, std::string value);
    };

    /**
     * @brief This class provide a simple API to manipulate storage files
     * @class Database
     * 
     */
    class Database{
        private:
            std::string _ssid; ///<The ssid of the analysed frame
            std::string _code_file_name; ///<The file name of the used code
            std::string _code_hash; ///<The hash of the content of the given code
            Csv *_csv; ///<The Csv instance to manipulate the storage file

        public:
            /**
             * @brief Construct a new Database object
             * 
             * @param ssid The SSID of the analysed frames
             * @param code_file_name The file name of the used code
             */
            Database(std::string ssid, std::string code_file_name);
            /**
             * @brief Destroy the Database object
             * 
             */
            ~Database();

            /**
             * @brief Get a cell given the value of its key and its column number
             * 
             * @param key_value the value of the key
             * @param column_name the column name of the cell
             * @return std::string the value of the cell
             */
            std::string get_cell(std::string key_value, std::string column_name);
            /**
             * @brief Get all keys values corresponding to a given cell value
             * 
             * @param column_name the name of the column to compare
             * @param cell_value the value to compare
             * @return std::vector<std::string> the vector of key values
             */
            std::vector<std::string> get_key_of_entries(std::string column_name, std::string cell_value);
            /**
             * @brief Add an entry to the storage file
             * 
             * @param values the vector of values corresponding to the new entry
             */
            void add_entry(std::vector<std::string> values);
            /**
             * @brief Replace an entry by another
             * 
             * @param key_value the key value of the entry to replace
             * @param values the vector of new values to replace the ols entry with
             */
            void replace_entry(std::string key_value, std::vector<std::string> values);
    };
};

#endif