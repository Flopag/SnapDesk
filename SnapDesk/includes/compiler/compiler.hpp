#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "os_communicator/os_communicator.hpp"
#include "compiler/node.hpp"
#include "compiler/function_node.hpp"
#include "decoder/frame.hpp"

namespace compiler {
    /**
     * @brief Tranform the source code into a executable tree
     * @class Compiler
     * 
     */
    class Compiler {
        private:
            const os_communicator::Communicator *_communicator; ///<Communicator to the source code

            // return the next line to read and fill the current_node
            /**
             * @brief Contruct the executable tree
             * 
             * @param current_node the node to attach the result
             * @param next_line the next line number to read
             * @return size_t the next line to read
             */
            size_t read_line(executable_tree::Node *current_node, size_t next_line) const;

        public:
            /**
             * @brief Construct a new Compiler object
             * 
             * @param communicator Communicator to source code
             */
            Compiler(const os_communicator::Communicator *communicator) 
            : _communicator(communicator) {
                if(!_communicator)
                    throw invalid_argument("No communicator given");
            };

            /**
             * @brief Compile the source code to get the executable tree
             * 
             * @return executable_tree::Node* the executable tree corresponding to the source code
             */
            executable_tree::Node *get_executable_tree() const;
    };
}

#endif