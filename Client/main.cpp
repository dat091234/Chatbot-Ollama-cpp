#include <iostream>
#include <windows.h>
#include "Execute.h"
#include "Extract.h"
#include "../Include/json.hpp" 

// Use json namespace for brevity
using json = nlohmann::json;
using std::cout, std::cin, std::endl, std::string;

int main() {
    // 1. Setup Console for UTF-8 (Support special characters)
    SetConsoleOutputCP(65001); 
    SetConsoleCP(65001);

    string user_question;
    cout << "Enter your question: ";
    getline(cin, user_question);

    // Make sure this URL is active from your Kaggle/Ngrok session
    string ngrok_url = "https://struttingly-nonaddicting-aisha.ngrok-free.dev"; // My link
    // string ngrok_url = "https://your-link.ngrok-free.app"; // Your link here

    // --- USE JSON.HPP TO CONSTRUCT DATA ---
    
    // Create a JSON object instead of manual string concatenation
    json body_json;
    body_json["model"] = "gpt-oss:20b";
    body_json["prompt"] = user_question; // Automatically handles special characters
    body_json["stream"] = false;
    
    // Optional: Add penalty for repetitive text
    body_json["options"]["repeat_penalty"] = 1.3;

    // Convert JSON Object to String (Stringify)
    string json_string = body_json.dump();

    // Escape double quotes (") to (\") for the terminal command
    // This step is mandatory when passing JSON via command line (curl)
    string escaped_json = "";
    for (char c : json_string) {
        if (c == '"') escaped_json += "\\\"";
        else escaped_json += c;
    }

    // Construct the command string (Cleaner and safer)
    string command = "curl -s -L -k " 
                     "-H \"ngrok-skip-browser-warning: 69420\" " 
                     "-H \"Content-Type: application/json\" " 
                     + ngrok_url + "/api/generate " 
                     "-d \"" + escaped_json + "\"";

    try {
        cout << "Thinking..." << endl;

        // Execute command
        string raw_response = exec(command.c_str());

        // --- USE JSON.HPP TO PARSE RESULT ---
        
        // Parse the raw string response into a JSON Object
        auto response_json = json::parse(raw_response);

        // Extract the answer safely
        if (response_json.contains("response")) {
            string ai_text = response_json["response"].get<string>();
            
            // Apply formatting (Markdown/Table cleaning)
            string pretty_answer = clean_and_format(ai_text);
            
            cout << "\n=== RESPONSE === \n" << endl;
            cout << pretty_answer << endl;
        } else {
            cout << "Error: Server returned no content." << endl;
            // Print detailed error if available
            if (response_json.contains("error")) {
                cout << "Details: " << response_json["error"] << endl;
            }
        }

    } catch (json::parse_error& e) {
        cout << "JSON Parse Error: " << e.what() << endl;
        cout << "Possible causes: Ngrok server is offline or URL is incorrect." << endl;
    } catch (...) {
        cout << "System Error!" << endl;
    }

    return 0;
}