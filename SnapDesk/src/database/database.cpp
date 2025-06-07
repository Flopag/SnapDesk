#include "database/core.hpp"

namespace database{

    std::string hash(std::string str){
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

        if (EVP_DigestUpdate(mdctx, str.c_str(), str.size()) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to update digest");
        }

        if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to finalize digest");
        }

        std::string output = "";
        for(size_t i = 0; i < hash_len; ++i){
            char buf[3];
            snprintf(buf, sizeof(buf), "%02x", hash[i]);
            output += buf;
        }

        return output;
    }

    /* Constructor */

    Database::Database(std::string ssid, std::string code_file_name) : _ssid(ssid), _code_file_name(code_file_name) {
        // Get the folder path
        std::replace(code_file_name.begin(), code_file_name.end(), '.', '_');
        std::replace(code_file_name.begin(), code_file_name.end(), '/', '-');
        std::string folder_name = std::string(DATABASE_ROOT) + "/" + code_file_name; //TODO handle folders and replacing "-" by "/"
        
        os_communicator::Communicator::create_folder(folder_name);

        std::string hashed_code = "";

        // Get the code content
        os_communicator::Communicator *tmp = new os_communicator::Communicator(_code_file_name);
        if(tmp->exist()){
            std::string last_line = tmp->get_line(0);
            for(size_t i=1; last_line != ""; ++i){
                hashed_code += last_line;
                last_line = tmp->get_line(i);
            }
        }
        delete tmp;

        hashed_code = hash(hashed_code);

        folder_name += "/" + hashed_code;
        
        os_communicator::Communicator::create_folder(folder_name);

        // Get the destination file name
        std::string destination_file_name = folder_name + "/" + ssid;

        // Get colums names
        std::vector<std::string> column_names = COLUMN_NAMES;

        // get csv  
        tmp = new os_communicator::Communicator(destination_file_name+DATA_EXTENTION);
        if(tmp->exist())
            _csv = new Csv(destination_file_name);
        else
            _csv = Csv::create(destination_file_name, column_names, KEY_COLUMN_NUMBER);
        delete tmp;
    };

    Database::~Database(){
        if(_csv)
            delete _csv;
    };

    /* Public */
    
    std::string Database::get_cell(std::string key_value, std::string column_name){
        return _csv->get_cell(_csv->get_row_number(key_value), _csv->get_column_number(column_name));
    }

    std::vector<std::string> Database::get_key_of_entries(std::string column_name, std::string cell_value){
        return _csv->get_all_keys_with_value(_csv->get_column_number(column_name), cell_value);
    }

    void Database::add_entry(std::vector<std::string> values){
        return _csv->push_row(values);
    }

    void Database::replace_entry(std::string key_value, std::vector<std::string> values){
        return _csv->replace_row(_csv->get_row_number(key_value), values);
    }
}