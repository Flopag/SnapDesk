/**
 * @file node.hpp
 * @author Pagano Florian
 * @brief This file contains base class of a executable tree
 * @version 0.1
 * @date 2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef EXECUTABLE_TREE_HPP
#define EXECUTABLE_TREE_HPP

#include "decoder/frame.hpp"

#include <vector>
#include <string>

namespace executable_tree{
    /**
     * @brief Represents a node of the executable tree
     * @class Node
     * 
     */
    class Node {       
        public:
            /**
             * @brief Get the value of the node
             * 
             * @param target_frame the frame that is analysed by the executable tree
             * @return std::string the value returned by the node
             */
            virtual std::string get_value(const decoder::Frame *target_frame) const;

            /**
             * @brief Add a child node to the current node
             * 
             * @param arg The node to add
             */
            virtual void add_node(Node* arg);

            /**
             * @brief get the string representing the node
             * 
             * @param depth the depth of the node in the executable tree
             * @return std::string the string representing the node
             */
            virtual std::string to_string(size_t depth) const;
    };

    /**
     * @brief The node that represent the root of the executable tree, it does not do anything but have one child
     * @class Root
     * 
     */
    class Root : public Node {
        private:
            Node* _first_node = nullptr; ///<The child that will output the result

        public:
            /**
             * @brief Destroy the Root object
             * 
             */
            ~Root(){
                if(_first_node)
                    delete _first_node;
            };

            std::string get_value(const decoder::Frame *target_frame) const override;

            void add_node(Node* arg) override;

            std::string to_string(size_t depth) const override;
    };

    /**
     * @brief Node that execute a function (that must be implemented as a child of Function class)
     * @class Function
     * 
     */
    class Function : public Node {
        protected:
            std::vector<Node*> args; ///<The vector of arguments of the function

        public:
            /**
             * @brief Destroy the Function object
             * 
             */
            ~Function(){
                for (Node* arg : args)
                    delete arg;

                args.clear();
            }

            void add_node(Node* arg) override;

            std::string to_string(size_t depth) const override;
    };

    /**
     * @brief Node that has a static value
     * @class Value
     * 
     */
    class Value : public Node {
        private:
            std::string value; ///<The fixed value of the Node

        public:
            /**
             * @brief Construct a new Value object
             * 
             * @param value the fixed value of the Node
             */
            Value(std::string value) : value(value) {};

            std::string get_value(const decoder::Frame *target_frame) const override;

            void add_node(Node* arg) override;

            std::string to_string(size_t depth) const override;
    };

    /**
     * @brief Node that get dynamic information in the frame
     * @class Getter
     * 
     */
    class Getter : public Node {
        private:
            const std::string field_name; ///<The field where the information will come from

        public:
            /**
             * @brief Construct a new Getter object
             * 
             * @param field_name the name of the field where to get information
             */
            Getter(const std::string field_name) 
                : field_name(field_name){};

            std::string get_value(const decoder::Frame *target_frame) const override;

            void add_node(Node* arg) override;

            std::string to_string(size_t depth) const override;
    };
}

#endif