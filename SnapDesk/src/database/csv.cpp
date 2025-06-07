#include "database/core.hpp"

namespace database{

    /* Constructor */

    Csv::Csv(std::string file_name){
        _c_data = new os_communicator::Communicator(file_name+DATA_EXTENTION);

        if(!_c_data->exist())
            throw invalid_argument("The file " + file_name+DATA_EXTENTION + " does not exist");

        _c_metadata = new os_communicator::Communicator(file_name+METADATA_EXTENTION);

        if(!_c_metadata->exist())
            throw invalid_argument("The file " + file_name+METADATA_EXTENTION + " does not exist");

        // Get number of columns
        _number_of_columns = _string_to_size_t(_c_metadata->get_line(COLUMN_NUMBER_LINE_NUMBER));

        // Get key_column_number
        _key_column_number = _string_to_size_t(_c_metadata->get_line(KEY_LINE_NUMBER));
    };

    Csv::~Csv(){
        if(_c_data)
            delete _c_data;
        if(_c_metadata)
            delete _c_metadata;
    };

    Csv *Csv::create(std::string file_name, std::vector<std::string> column_names, size_t key_column_number){
        os_communicator::Communicator *tmp;

        // Init of data
                
        tmp = new os_communicator::Communicator(file_name+DATA_EXTENTION);

        tmp->new_file();
        tmp->add_line(Csv::_values_to_line(column_names));

        delete tmp;

        // init of metadata

        tmp = new os_communicator::Communicator(file_name+METADATA_EXTENTION);

        tmp->new_file();
        tmp->add_line(std::to_string(key_column_number));
        tmp->add_line(std::to_string(column_names.size()));

        delete tmp;

        return new Csv(file_name);
    };

    /* private */

    void Csv::_test_values_size(std::vector<std::string> values){
        if(values.size() != _number_of_columns)
            throw invalid_argument("The size of row (" 
                + std::to_string(values.size()) 
                + ") is not equal to the number of column (" 
                + std::to_string(_number_of_columns) + ")");
    };

    size_t Csv::_get_key_position(std::string key_value){
        std::vector<std::string> keys = get_all_keys();
        auto it = std::find(keys.begin(), keys.end(), key_value);

        if(it == keys.end())
            return -1;
        else
            return std::distance(keys.begin(), it);
    }

    std::string Csv::_values_to_line(std::vector<std::string> values){
        std::string line = "";

        for(std::string value : values)
            line += value + DELIMITER;

        return line;
    };

    size_t Csv::_get_column_position_from_header(std::string header, std::string column_name){
        std::string line = header;

        for(size_t i = 0; i < _number_of_columns; ++i){
            if(line.substr(0, line.find(DELIMITER)) == column_name)
                return i;
            size_t pos = line.find(DELIMITER);
            if (pos != std::string::npos) {
                line = line.substr(pos + 1);
            }
        }
                
        return -1;
    }

    std::string Csv::_get_cell_from_row(std::string row, size_t column_number){
        std::string cell = row;

        for(size_t i = 0; i < column_number; ++i){
            size_t pos = cell.find(DELIMITER);
            if (pos != std::string::npos) {
                cell = cell.substr(pos + 1);
            }
        }

        if(cell.find(DELIMITER))
            cell = cell.substr(0, cell.find(DELIMITER));
                
        return cell;
    }

    size_t Csv::_string_to_size_t(std::string number){
        size_t output;
        sscanf(number.c_str(), "%zu", &output);
        return output;
    };

    /* Public */

    void Csv::push_row(std::vector<std::string> values){
        _test_values_size(values);

        std::string key_value = values[_key_column_number];

        if(_get_key_position(key_value) != -1)
            throw invalid_argument("The key " + key_value + " does already exist");

        _c_data->add_line(_values_to_line(values));
    }

    void Csv::replace_row(size_t row_number, std::vector<std::string> values){
        _test_values_size(values);

        std::string key_value = values[_key_column_number];
        size_t key_position = _get_key_position(key_value);

        if(!(key_position == row_number || key_position == -1))
            throw invalid_argument("The key " + key_value + " does already exist");

        _c_data->replace_line(row_number+HEADER_SIZE, _values_to_line(values));
    }

    std::string Csv::get_cell(size_t row_number, size_t column_number){
        if(column_number >= _number_of_columns)
            throw invalid_argument("Column number " 
                + std::to_string(column_number) 
                + " greater that the number of columns " 
                + std::to_string(_number_of_columns));
        if((int) row_number+HEADER_SIZE < HEADER_SIZE)
            throw invalid_argument("Row number " 
                + std::to_string((int) row_number) 
                + " smaller that the minimal value " 
                + std::to_string(HEADER_SIZE));
        
        std::string line = _c_data->get_line(row_number+HEADER_SIZE);

        return _get_cell_from_row(line, column_number);
    }

    size_t Csv::get_row_number(std::string key_value){
        return _get_key_position(key_value);
    }

    size_t Csv::get_column_number(std::string column_name){
        std::string header = _c_data->get_line(0);

        return _get_column_position_from_header(header, column_name);
    }

    std::vector<std::string> Csv::get_all_keys(){
        return get_all_keys_with_value(0, "");
    }

    std::vector<std::string> Csv::get_all_keys_with_value(size_t column_number, std::string value){
        if(column_number >= _number_of_columns)
            throw invalid_argument("Column number " 
                + std::to_string(column_number) 
                + " greater that the number of columns " 
                + std::to_string(_number_of_columns));

        std::vector<std::string> keys;

        size_t i = HEADER_SIZE;

        while(1){
            std::string line = _c_data->get_line(i);
            if(line == "")
                break;
            if(value == "" || _get_cell_from_row(line, column_number) == value)
                keys.push_back(_get_cell_from_row(line, _key_column_number));
            ++i;
        };

        return keys;
    }
}