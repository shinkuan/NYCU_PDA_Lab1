#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include "outline.h"


int main(int argc, char const *argv[]) {
    //================================================================//
    //                   Parse the input commands                     //
    //================================================================//
    std::list<std::string> commands;
    if (argc == 1) {
        std::string command;
        while (std::getline(std::cin, command)) {
            commands.push_back(command);
        }
    } else
    if (argc == 2) {
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        exit(1);
    } else
    if (argc == 3) {
        std::ifstream input_file(argv[1]);
        if (!input_file.is_open()) {
            std::cerr << "Error: Unable to open input file" << std::endl;
            exit(1);
        }

        std::string command;
        while (std::getline(input_file, command)) {
            commands.push_back(command);
        }
    } else {
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        exit(1);
    }

    //================================================================//
    //                     Parse the outline size                     //
    //================================================================//
    int outline_width, outline_height;
    std::string command = commands.front();
    commands.pop_front();
    std::istringstream iss(command);
    iss >> outline_width >> outline_height;
    Outline outline(outline_width, outline_height);

    //================================================================//
    //                     Parse the input commands                   //
    //================================================================//
    std::list<Point> find_point_command_answers;
    for (std::string command : commands) {
        std::istringstream iss(command);
        std::string first_word;
        iss >> first_word;
        if (first_word == "P") {
            int x, y;
            iss >> x >> y;
            Point point = {x, y};
            Tile* tile = outline.findTileatPoint(outline.start, point);
            find_point_command_answers.push_back(tile->getRect().bottom_left);
        } else {
            int id;
            id = std::stoi(first_word);
            int x, y, w, h;
            iss >> x >> y >> w >> h;
            Rect rect = {{x + w, y + h}, {x, y}};
            outline.createBlock(rect, id);
        }
    }

    //================================================================//
    //                     Write the output to file                   //
    //================================================================//
    if (argc == 1) {
        std::cout << outline.blocks.size() << std::endl;
        outline.blocks.sort([](Tile* a, Tile* b) {
            return a->getId() < b->getId();
        });
        for (Tile* block : outline.blocks) {
            if (block->getId() == -1) {
                break;
            }
            NeighborCount neighbor_count = outline.neighborFinding(block);
            std::cout << block->getId() << " " << neighbor_count.solid_count << " " << neighbor_count.space_count << std::endl;
        }
        for (Point point : find_point_command_answers) {
            std::cout << point.x << " " << point.y << std::endl;
        }
    } else
    if (argc == 3) {
        std::ofstream output_file(argv[2]);
        if (!output_file.is_open()) {
            std::cerr << "Error: Unable to open output file" << std::endl;
            exit(1);
        }

        output_file << outline.blocks.size() << std::endl;
        outline.blocks.sort([](Tile* a, Tile* b) {
            return a->getId() < b->getId();
        });
        for (Tile* block : outline.blocks) {
            if (block->getId() == -1) {
                continue;;
            }
            NeighborCount neighbor_count = outline.neighborFinding(block);
            output_file << block->getId() << " " << neighbor_count.solid_count << " " << neighbor_count.space_count << std::endl;
        }
        for (Point point : find_point_command_answers) {
            output_file << point.x << " " << point.y << std::endl;
        }
    }

    // Optional: Write the drawing to a file
    //================================================================//
    //                           Drawing                              //
    //================================================================//
    std::string draw_file_name = std::string(argv[2]) + "_drawing.txt";
    std::ofstream drawing_file(draw_file_name);
    if (!drawing_file.is_open()) {
        std::cerr << "Error: Unable to open drawing file" << std::endl;
        exit(1);
    }

    drawing_file << outline.blocks.size() << std::endl;
    drawing_file << outline.getWidth() << " " << outline.getHeight() << std::endl;
    for (Tile* block : outline.blocks) {
        Rect rect = block->getRect();
        int id = block->getId();
        int x, y, w, h;
        x = rect.bottom_left.x;
        y = rect.bottom_left.y;
        w = rect.top_right.x - rect.bottom_left.x;
        h = rect.top_right.y - rect.bottom_left.y;
        drawing_file << id << " " << x << " " << y << " " << w << " " << h << std::endl;
    }

    return 0;
}
