#include "preprocessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <omp.h>

// Function to process special lines
std::string processSpecialLine(const std::string& blockContent) {
    std::string processedContent = blockContent;

    // Transform operations with proper lambda function handling and variable capturing
    processedContent = std::regex_replace(processedContent, std::regex(R"(\.OrderBy\[(.*?)\])"), ".OrderBy([&](auto value){ return $1; })");
    processedContent = std::regex_replace(processedContent, std::regex(R"(\.GroupBy\[(.*?)\])"), ".GroupBy([&](auto value){ return $1; })");
    processedContent = std::regex_replace(processedContent, std::regex(R"(\.Select\[(.*?)\])"), ".Select([&](auto value){ return $1; })");
    processedContent = std::regex_replace(processedContent, std::regex(R"(\.Where\[(.*?)\])"), ".Where([&](auto value){ return $1; })");
    processedContent = std::regex_replace(processedContent, std::regex(R"(\.All\[(.*?)\])"), ".All([&](auto value){ return $1; })");
    processedContent = std::regex_replace(processedContent, std::regex(R"(\.Any\[(.*?)\])"), ".Any([&](auto value){ return $1; })");

    return processedContent;
}

// Function to process special blocks
std::string processSpecialBlock(const std::vector<std::string>& blockContent) {
    std::string processedContent = "{\n";
    // Use OpenMP to parallelize the processing of each line
    // Write the processed lines to the processedContent string in the correct order
    // Set number of threads to 4
    // Split vector processing among threads
    std::vector<std::string> processedLines(blockContent.size());
    std::vector<std::vector<int>> threadAssignments(4);
#pragma omp parallel for num_threads(4) default(none) shared(blockContent, threadAssignments, processedLines)
    for (int i = 0; i < blockContent.size(); i++) {
#pragma omp critical
        {
            threadAssignments[omp_get_thread_num()].push_back(i);
        }
        processedLines[i] = processSpecialLine(blockContent[i]);
    }

    for (const auto& line : processedLines) {
        processedContent += line + "\n";
    }
    std::cout << "-----------------------------------------------------" << std::endl;
    for (int i = 0; i < threadAssignments.size(); i++) {
        for (const auto &lineIndex: threadAssignments[i]) {
            std::cout << "Thread " << i << " processed line " << lineIndex << std::endl;
        }
    }
    std::cout << "-----------------------------------------------------" << std::endl;

    processedContent += "}\n";
    return processedContent;
}

// Function for preprocessing code
void preprocessCode(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream infile(inputFile);
    std::ofstream outfile(outputFile);

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    outfile << "#include \"laic.h\"\n";
    outfile << "#include <iostream>\n";
    outfile << "#include <vector>\n";
    outfile << "#include <cmath>\n"; // For the sqrt function

    std::cout << "Preprocessing file..." << std::endl;

    std::vector<std::string> specialBlock;

    std::string line;
    bool inSpecialBlock = false;

    while (std::getline(infile, line)) {
        if (line == "#ssb") {
            inSpecialBlock = true;
        } else if (line == "#esb") {
            inSpecialBlock = false;
            // Process the content of the special block
            outfile << processSpecialBlock(specialBlock);
            specialBlock.clear();
        } else if (inSpecialBlock) {
            specialBlock.push_back(line);
        } else {
            outfile << line << std::endl;
        }
    }

    infile.close();
    outfile.close();
}
