#include "Extract.h"
#include "../Include/json.hpp" 
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void replace_all(string& str, const string& from, const string& to) {
    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

vector<string> split_table_row(const string& row) {
    vector<string> cells;
    stringstream ss(row);
    string segment;
    while (getline(ss, segment, '|')) {
        string cleaned = trim(segment);
        if (!cleaned.empty()) {
            cells.push_back(cleaned);
        }
    }
    return cells;
}

string render_table(const vector<string>& raw_lines) {
    if (raw_lines.empty()) return "";

    vector<vector<string>> rows;
    size_t max_col1_width = 0;

    for (const string& line : raw_lines) {
        if (line.find("---") != string::npos) continue;
        vector<string> cells = split_table_row(line);
        if (cells.size() < 2) continue; 

        if (cells[0].length() > max_col1_width) {
            max_col1_width = cells[0].length();
        }
        rows.push_back(cells);
    }

    if (max_col1_width > 25) max_col1_width = 25;

    stringstream ss;
    ss << "\n";
    for (const auto& row : rows) {
        ss << "| " << left << setw(max_col1_width) << row[0] << " | ";
        for (size_t i = 1; i < row.size(); ++i) {
            ss << row[i];
            if (i < row.size() - 1) ss << " | ";
        }
        ss << " |\n";
        if (&row == &rows[0]) {
            ss << "| " << string(max_col1_width, '-') << " | " << string(40, '-') << " |\n";
        }
    }
    return ss.str();
}

string extract_response(string json_text) {
    try {
        auto j = json::parse(json_text);
        if (j.contains("response")) {
            return j["response"].get<string>();
        }
        return "Error: No response found.";
    } catch (json::parse_error& e) {
        return "JSON Error: " + string(e.what());
    }
}

string clean_and_format(string text) {
    
    replace_all(text, "**", ""); 
    
    stringstream ss(text);
    string line;
    string final_output = "";
    vector<string> table_buffer;
    bool in_table = false;
    string last_line = ""; 

    while (getline(ss, line, '\n')) {
        string trimmed_line = trim(line);
        if (trimmed_line.empty()) continue;
        
        // Chống lặp dòng (Optional)
        if (trimmed_line == last_line) continue;
        last_line = trimmed_line;

        if (trimmed_line.front() == '|' && trimmed_line.back() == '|') {
            in_table = true;
            table_buffer.push_back(trimmed_line);
        } 
        else {
            if (in_table) {
                final_output += render_table(table_buffer);
                table_buffer.clear();
                in_table = false;
            }

            if (line.find("## ") != string::npos) {
                replace_all(line, "#", "");
                final_output += "\n========================================\n";
                final_output += "   " + trim(line) + "\n";
                final_output += "========================================\n";
            } else {
                final_output += line + "\n";
            }
        }
    }
    if (in_table && !table_buffer.empty()) {
        final_output += render_table(table_buffer);
    }

    return final_output;
}