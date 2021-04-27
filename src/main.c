
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "utilities.c"
#include "outputmanager.cpp"
#include "datamanager.cpp"

using namespace DataManager;
using namespace ElkOutput;

// Initialize the pointers to zero.
DataManager::BucketManager* BucketManager::s_instance = 0;
OutputManager* OutputManager::s_instance = 0;

// Get instances of the Manager objects.
OutputManager* output_manager = OutputManager::getInstance();
DataManager::BucketManager* bucket_manager = BucketManager::getInstance();

#include "datahandler.cpp"

#include "sql.tab.c"
#include "lex.yy.c"
#include "custom_reader.h"

static char* query = (char*) NULL;

char* get_query()
{
    // Free the memory if the query is already allocated.
    if (query)
    {
        free(query);
        query = (char *)NULL;
    }

    // Get the query.
    query = readline ("elkdb> ");

    // Add the query to the history.
    if (query && *query) {
        add_history (query);
    }

  return query;
}

void printHelp()
{
    std::cout << "Usage: " << std::endl;
    std::cout << "\t" << "elkdb [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << "\t" << "-h --help" << "\t" << "Print the help documentation" << std::endl;
    std::cout << "\t" << "-f --file FILE" << "\t" << "Execute the commands from the FILE" << std::endl;
    std::cout << std::endl;
}

void parseArguments(int argc, char* argv[])
{
    bool file = false;
    bool continue_execution = true;
    std::string filename;

    if(argc >= 2)
    {
        continue_execution = false;
        for(int i = 1; i < argc; i++)
        {
            if(file)
            {
                filename = std::string(argv[i]);
                break;
            }

            if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            {
                printHelp();
                break;
            }

            if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
            {
                file = true;
            }
        }

        if(file)
        {
            std::string line;
            std::fstream infile(filename);

            while(std::getline(infile, line))
            {
                // std::cout << line << std::endl;
                // if(strncmp(line.c_str(), "\\exit", 5))
                // {
                //     break;
                // }

                if(strcmp(line.c_str(), "") == 0 || strcmp(line.c_str(), "\n") == 0 || strcmp(line.c_str(), "\t") == 0) {
                    continue;
                }
                parse_query(line.c_str());
            }

            infile.close();
            continue_execution = true;
        }
    }

    if(continue_execution)
    {
        char* q;
        q = get_query();

        while(strncmp(q, "\\exit", 5) != 0) {
            parse_query(q);

            q = get_query();
        }
    }
}

int main(int argc, char* argv[])
{
    std::cout << "  ______   _   _      _____    ____  " << std::endl;
    std::cout << " |  ____| | | | |    |  __ \\  |  _ \\ " << std::endl;
    std::cout << " | |__    | | | | __ | |  | | | |_) |" << std::endl;
    std::cout << " |  __|   | | | |/ / | |  | | |  _ < " << std::endl;
    std::cout << " | |____  | | |   <  | |__| | | |_) |" << std::endl;
    std::cout << " |______| |_| |_|\\_\\ |_____/  |____/ " << std::endl;
    std::cout << "" << std::endl;
    std::cout << "ElkDB Version 1.0.0 alpha (April 27, 2021)" << std::endl;
    std::cout << std::endl;

    parseArguments(argc, argv);

    // std::string document_string = "{\"CarName\":\"Sonata\",\"CarType\":\"Gasoline\",\"CarMaker\":\"Hyundai\",\"CarInCode\":\"000001\",\"CarPart\":{\"PubPartName\":\"Battery\",\"PopPartName\":\"Battery\",\"PartType\":\"Null\",\"PartNum\":1,\"PartWeight\":{\"PartFeWt\":\"0kg\",\"PartNonFeWt\":\"11.44kg\",\"PartPIWt\":\"4.40kg\",\"PartGumWt\":\"0kg\",\"PartGlassWt\":\"0kg\",\"PartSpongeWt\":\"0kg\",\"PartLiquidWt\":\"0kg\",\"PartWasteWt\":\"6.16kg\",\"TotalWt\":\"22kg\"},\"RecycleWt\":\"15.84kg\",\"RecycleRate\":\"72.0%\",\"ClassType1\":\"Basic\",\"ClassType2\":\"Basic Dismantling\",\"PartMark\":\"Good condition\"},\"Station\":\"10\",\"CarInform\":{\"Transmission\":\"Auto\",\"displacement\":\"1820CC\",\"CarPhoto\":\"Null\",\"InDate\":\"2015-01-04T00:00:00Z\",\"EndDate\":\"2015-01-08T00:00:00Z\",\"MoveYN\":\"8Y\",\"MoveKM\":\"53,000KM\",\"Colors\":\"White\",\"RecycleYN\":\"Y\",\"Weight\":\"1920kg\"},\"WorkerName\":\"Joohyoung\",\"CarMark\":{\"Note\":\"Good condition but front bumper damaged, defective transmissions. By Worker 00001 Joohyoung\"}}";

    // bucket_manager->createBucket("cars");
    // bucket_manager->insertIntoBucket("cars", "car_1", document_string);

    // create_bucket("scraped_results");
    // create_bucket("deduped_results");

    // insert_into_bucket("deduped_results", "1", "{\"hello\": \"world\"}");
    // insert_into_bucket("scraped_results", "45", "{\"bonjour\": \"monde\"}");
    // insert_into_bucket("scraped_results", "25", "{\"nihao\": \"world\"}");
    // insert_into_bucket("scraped_results", "30", "{\"hola\": \"test\"}");

    // insert into cars values ('car_1', '{"CarName":"Sonata","CarType":"Gasoline","CarMaker":"Hyundai","CarInCode":"000001","CarPart":{"PubPartName":"Battery","PopPartName":"Battery","PartType":"Null","PartNum":1,"PartWeight":{"PartFeWt":"0kg","PartNonFeWt":"11.44kg","PartPIWt":"4.40kg","PartGumWt":"0kg","PartGlassWt":"0kg","PartSpongeWt":"0kg","PartLiquidWt":"0kg","PartWasteWt":"6.16kg","TotalWt":"22kg"},"RecycleWt":"15.84kg","RecycleRate":"72.0%","ClassType1":"Basic","ClassType2":"Basic Dismantling","PartMark":"Good condition"},"Station":"10","CarInform":{"Transmission":"Auto","displacement":"1820CC","CarPhoto":"Null","InDate":"2015-01-04T00:00:00Z","EndDate":"2015-01-08T00:00:00Z","MoveYN":"8Y","MoveKM":"53,000KM","Colors":"White","RecycleYN":"Y","Weight":"1920kg"},"WorkerName":"Joohyoung","CarMark":{"Note":"Good condition but front bumper damaged, defective transmissions. By Worker 00001 Joohyoung"}}');

    return 0;

}