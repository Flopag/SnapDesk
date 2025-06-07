#include "os_communicator/os_communicator.hpp"

using namespace std;

namespace os_communicator
{
    /* Constructor */

    Communicator::Communicator(string file_name) : _file_name(file_name) {};

    /* Public */

    bool Communicator::exist(){
        ifstream file(_file_name);

        if(file.is_open()){
            file.close();
            return true;
        }
        
        file.close();
        return false;
    }

    size_t Communicator::in_buffer(uint8_t *buffer, const size_t buffer_size){
        if(!buffer)
            throw invalid_argument("No buffer given");
        if(buffer_size < 0)
            throw invalid_argument("Negative buffer size");

        size_t byte_read = 0;

        ifstream file(_file_name);

        if(!file.is_open())
            throw runtime_error("Failed to open file: " + _file_name);

        for(size_t i = 0; i < buffer_size; ++i){
            char c = file.get();

            // Checking for end of file
            if (file.eof())
                break;
    
            buffer[i] = c;

            byte_read++;
        }

        file.close();

        return byte_read;
    }    

    string Communicator::get_line(size_t line_number) const {
        if(line_number < 0)
            throw invalid_argument("Line number smaller than 0");

        string line;
        size_t current_line = 0;
        std::ifstream file(_file_name);

        if (!file.is_open()) {
            throw runtime_error("Failed to open file: " + _file_name);
        }

        while(getline(file, line)){
            if(current_line == line_number)
                return line;
            current_line++;
        }

        file.close();

        return "";
    }

    void Communicator::add_line(string line){
        std::ofstream file(_file_name, std::fstream::out | std::fstream::app);

        if (!file.is_open()) {
            throw runtime_error("Failed to open file: " + _file_name);
        }

        file << line << std::endl;

        file.close();
    }

    void Communicator::replace_line(size_t line_number, string new_line){
        if(get_line(line_number) == "")
            throw invalid_argument("The line number " + std::to_string(line_number) + " does not exist");

        std::fstream file_in(_file_name, std::fstream::in);

        if (!file_in.is_open()) {
            throw runtime_error("Failed to open file: " + _file_name);
        }

        std::string out = "";
        std::string line;

        // Copy all lines previous to line_number
        for(size_t i = 0; i < line_number; ++i){
            std::getline(file_in, line);
            out += line + "\n";
        }

        // Replace the line number line_number
        std::getline(file_in, line);
        out += new_line + "\n";

        // Copy add lines after line_number
        for(size_t i = line_number+1; 1; ++i){
            std::getline(file_in, line);
            if(file_in.eof())
                break;
            out += line + "\n";
        }

        file_in.close();

        // Write The modified file
        std::fstream file_out(_file_name, std::fstream::out);

        if (!file_out.is_open()) {
            throw runtime_error("Failed to open file: " + _file_name);
        }

        file_out << out;
        file_out.close();
    }

    void Communicator::new_file(){
        std::ofstream file(_file_name, std::fstream::out);

        file.close();
    }

    string Communicator::get_current_date(){
        std::time_t time = std::time(nullptr);
        std::tm tm = *std::localtime(&time);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d/%m/%Y %H h %M min %S sec");
        return oss.str();
    }

    void Communicator::notify(string message){
        system(("notify-send \"" + message + "\" -a snapdesk -t 5000").c_str());
    }

    void Communicator::sleep(size_t seconds){
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }

    void Communicator::create_folder(string folder_name){
        if(Communicator::get_file_type(folder_name) == F_FILE)
            throw runtime_error("The file " + folder_name + " must be a folder");
        else if(Communicator::get_file_type(folder_name) != F_NONE)
            return;
        filesystem::create_directory(folder_name);
    }

    size_t Communicator::get_file_type(string name){
        if(filesystem::exists(name)){
            if(filesystem::is_directory(name))
                return F_FOLDER;
            else
                return F_FILE;
        }
        return F_NONE;
    }
}