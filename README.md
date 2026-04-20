# Ollama C++ Chatbot Client
MSSV: **24120034**  
Họ và tên: *Đoàn Phước Đạt*

## Compile and Run the Chatbot Ollama HPP (Standing at the root directory)
```Bash
linux  
$ g++ -std=c++23 Client/*.cpp Include/json.hpp  -o out/run 
windows
$ g++ -std=c++23 (ls Client/*.cpp) Include/json.hpp  -o out/run 
```
```Bash
$ ./out/run
```
##How to set up chatbot Ollama HPP:  
**1️Step 1: Set up Ollama server**
- Navigate to the 'Server/ ' in this repo and download the Ollama_Server_Setup.ipynb file.
- Go to Kaggle.
- Click "New Notebook" -> Select File -> Import Notebook -> Upload the .ipynb file you just downloaded.
- In the right sidebar, under Session Options, set Accelerator to GPU T4 x2.
- Sign up at Ngrok to get your Authtoken.
- Paste your Token into the NGROK_TOKEN variable in the code block.
- Click Run All.
Wait for 3-5 minutes, then copy the public URL (e.g., https://...ngrok-free.app) generated at the end of the output.   

**Step 2: Run the C++ client**
- Open the 'Client/ ' in VS Code.
- Open main.cpp file
- Find the ngrok_url variable and paste the link you copied from Kaggle
- Compile and run the project as shown above.

## Notice:
- For table rendering, the information is rendered respectively to the order of columns that seperated by | in the response.   
  EX: | a |  b | c |  
      | information a | information b | information c |
- Can ask anything you want with any language, the bot will try to answer you as best as it can.

## Project Development
### How I created this project:
- Run Ollama server on kaggle notebook by following lecturer's instruction. After that, I got the API link to call the server.
- Honestly I used chatbot, particularly ChatGPT for C++ coding part because I don't know how to start with C++ coding for API call ollama server and process the untidy json response.
- I applied Postman to test server response, API call.

### Challenges:
- I still haven't promt the bot to make the cleaner table and easy to see the response.
- The chatbot can't maintain the information in long term conversation, it always forget what user said before.

### What I want to improve next:
- Improve the cleaning function to make cleaner table from raw response.
- Optimize the thinking time of the bot (make it shorter).
- Create the algorith to maintain the context of the conversation.
- Create GUI for better experience.
