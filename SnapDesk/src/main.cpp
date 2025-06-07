/**
 * @file main.cpp
 * @author Pagano Florian
 * @brief This is the main file. It contains the main loop of snapdesk.
 * @version 0.1
 * @date 2025-05-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "decoder/frame.hpp"
#include "os_communicator/os_communicator.hpp"
#include "compiler/node.hpp"
#include "compiler/function_node.hpp"
#include "compiler/compiler.hpp"
#include "database/core.hpp"

// Args values (to determine how to get them afteward)
#define CHARACTER_DEVICE_FILE "/dev/beacon-sniffer-0"
#define SCRIPT_FILE "./code.txt"
#define PERIOD 1

/**
 * @brief The true main function. This exist to permit the program to rerun itself when crashing
 * 
 * @return int: The same return than the main function
 */
int run(){
    os_communicator::Communicator::create_folder(DATABASE_ROOT);

    os_communicator::Communicator *c_frame = new os_communicator::Communicator(CHARACTER_DEVICE_FILE);
    os_communicator::Communicator *c_script = new os_communicator::Communicator(SCRIPT_FILE);
    decoder::Frame *beacon_frame = new decoder::Frame(c_frame);

    compiler::Compiler *compiler = new compiler::Compiler(c_script);
    executable_tree::Node *tree = compiler->get_executable_tree();
    printf("----- tree -----\n");
    printf("%s", tree->to_string(0).c_str());
    printf("----------------\n");

    database::Database *database = nullptr;  
    std::string current_ssid = "";

    try{
        while(1){
            os_communicator::Communicator::sleep(PERIOD);

            beacon_frame->update_raw_data();

            //beacon_frame->print_raw_data();

            beacon_frame->decode();

            beacon_frame->print();

            std::string output = tree->get_value(beacon_frame);

            if(beacon_frame->get_value("0").is_null())
                continue;

            if(beacon_frame->get_value("0").char_string() != current_ssid){
                current_ssid = beacon_frame->get_value("0").char_string();
                delete database;
                database = nullptr;
            }

            if(database == nullptr)
                database = new database::Database(beacon_frame->get_value("0").char_string(), "snappy.txt");

            if(database->get_key_of_entries("output", output).empty()){
                database->add_entry({
                    output, 
                    os_communicator::Communicator::get_current_date(), 
                    os_communicator::Communicator::get_current_date()});
                os_communicator::Communicator::notify("A new AP as been detected on SSID: " + current_ssid);
            }
            else
                database->replace_entry(output, {
                    output, 
                    database->get_cell(output, "creation_date"),
                    os_communicator::Communicator::get_current_date()});

        }
    } catch(const std::exception &e){
        delete beacon_frame;
        delete c_frame;
        delete c_script;
        delete compiler;
        delete tree;
        delete database;

        throw;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    while (true){
        try{
            run();
        }
        catch(const std::exception &e){
            fprintf(stderr, "Error: %s\n", e.what());
        }
        catch(...){
            fprintf(stderr, "Error: unknown\n");
        }
        os_communicator::Communicator::sleep(PERIOD);
    }
}