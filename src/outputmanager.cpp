
#include <iostream>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <algorithm>
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/prettywriter.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/error/en.h"

rapidjson::MemoryPoolAllocator<> jsonAlloc;

namespace ElkOutput
{
    class OutputManager
    {
        private:
            // Number of spaces to print default to 4 spaces.
            unsigned int indent = 4;

        public:

            // Static instance of this class.
            static OutputManager* s_instance;


            // Default Constructor
            OutputManager() {}

            // Returns the single instance of this class.
            static OutputManager* getInstance()
            {
                if(!s_instance) {
                    s_instance = new OutputManager;
                }

                return s_instance;
            }

            // Returns the JSON string from the Document object.
            std::string getJsonString(rapidjson::Document& document, bool pretty_string = true)
            {
                rapidjson::StringBuffer buffer;

                if(pretty_string)
                {
                    rapidjson::PrettyWriter<rapidjson::StringBuffer> pretty_writer(buffer);
                    pretty_writer.SetIndent(' ', this->getIndent());
                    document.Accept(pretty_writer);
                }
                else
                {
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    document.Accept(writer);
                }

                return buffer.GetString();
            }

            // Returns the response as a JSON string.
            std::string getResponseString(std::string status, std::string message)
            {
                rapidjson::Document doc;
                std::string response = "{\"status\": \"" + status + "\", \"message\": \"" + message + "\"}";
                doc.Parse(response.c_str());

                return getJsonString(doc);
            }

            unsigned int getIndent()
            {
                return this->indent;
            }

            void setIndent(int indent)
            {
                if(indent <= 16 && indent >= 2 && indent % 2 == 0)
                {
                    this->indent = (unsigned int) indent;
                    std::cout << this->getResponseString("success", "The ident is now " + std::to_string(indent) + " spaces.") << std::endl;
                }
                else
                {
                    std::cerr << this->getResponseString("error", "The indent value should be an even integer between 2 and 16.") << std::endl;
                }

            }
    };
}
