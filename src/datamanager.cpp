

// using namespace std;
// using namespace rapidjson;

namespace DataManager
{
    // Define the key-value (document) pair dictionary.
    typedef std::unordered_map<std::string, std::string> Dictionary;

    // Define the bucket to documents dictionary.
    typedef std::unordered_map<std::string, Dictionary> BucketMap;

    class BucketManager
    {
        public:
            // Static instance of this class.
            static BucketManager* s_instance;

            // Buckets hash table.
            // inline static std::unordered_map<std::string, Dictionary> s_buckets;
            inline static BucketMap s_buckets;

            // ElkOutput::OutputManager* output_manager;

            // Default Constructor
            BucketManager() {}

            // Default Constructor
            // BucketManager(ElkOutput::OutputManager* output_manager)
            // {
            //     this->output_manager = output_manager;
            // }

            // Returns the single instance of this class.
            static BucketManager* getInstance()
            {
                if(!s_instance) {
                    s_instance = new BucketManager;
                }

                return s_instance;
            }

            // static BucketManager* getInstance(ElkOutput::OutputManager* output_manager = NULL)
            // {
            //     if(!s_instance) {
            //         s_instance = new BucketManager(output_manager);
            //     }

            //     return s_instance;
            // }

            // Returns the bucket.
            // static std::unordered_map<std::string, Dictionary> getBuckets()
            static BucketMap getBuckets()
            {
                return s_buckets;
            }

            // Creates a new bucket.
            void createBucket(std::string bucket_name)
            {
                if(this->bucketExists(bucket_name)) {
                    std::cerr << ElkOutput::OutputManager::getInstance()->getResponseString("error", "Bucket `" + bucket_name + "` alerady exists.") << std::endl;
                    return;
                }

                Dictionary temp;

                BucketManager::s_buckets.insert(std::pair<std::string, Dictionary>(bucket_name, temp));

                std::cout << ElkOutput::OutputManager::getInstance()->getResponseString("success", "Bucket `" + bucket_name + "` Successfully created.") << std::endl;
            }

            // Returns whether the bucket exists or not.
            bool bucketExists(std::string bucket_name)
            {

                if(BucketManager::s_buckets.find(bucket_name) != BucketManager::s_buckets.end()) {
                    return true;
                }

                return false;
            }


            Dictionary& getBucket(std::string bucket_name)
            {
                return BucketManager::s_buckets[bucket_name];
            }

            void selectBucket(std::string keys, std::string bucket_name)
            {

                // select * from scraped_results;

                if(!this->bucketExists(bucket_name)) {
                    std::cout << "Bucket `" + bucket_name + "` does not exists" << std::endl;
                    return;
                }

                // rapidjson::Document d;
                rapidjson::Document response;
                std::string response_query = "{\"" + bucket_name + "\": {}}";
                response.Parse(response_query.c_str());

                for(auto const &doc : this->getBucket(bucket_name))
                {
                    rapidjson::Document x;
                    x.Parse(doc.second.c_str());

                    // std::cout << doc.second << std::endl;
                    response[bucket_name.c_str()].AddMember(rapidjson::StringRef(doc.first.c_str()), x, response.GetAllocator());
                }


                std::cout << ElkOutput::OutputManager::getInstance()->getJsonString(response) << std::endl;
                // getResponseString("success", buffer.G)
                // rapidjson::StringBuffer buffer;
                // rapidjson::PrettyWriter<rapidjson::StringBuffer> pretty_writer(buffer);
                // pretty_writer.SetIndent(' ', 4);
                // response.Accept(pretty_writer);

                // std::cout << buffer.GetString() << std::endl;
            }

            void listBuckets()
            {
                if(this->getBuckets().bucket_count() <= 1) {
                    std::cout << " No buckets found." << std::endl;
                    return;
                }

                int padding = 20;
                std::string header_bn = "Bucket Name";
                header_bn.append(padding - header_bn.length(), ' ');

                std::string header_dc = "Number of Documents";
                header_dc.append(padding - header_dc.length(), ' ');

                std::cout << " " << header_bn << "  " << header_dc << std::endl;
                std::cout << " " << std::string(header_bn.length(), '-') << "  " << std::string(header_bn.length(), '-') << std::endl;

                for(auto const &a : this->getBuckets()) {
                    std::string bname = a.first;
                    bname.append(padding - bname.length(), ' ');

                    unsigned num_documents = 0;
                    unsigned count = a.second.bucket_count();

                    for(int i = 0; i < count; i++) {
                        num_documents += a.second.bucket_size(i);
                    }

                    std::string ndocs = std::to_string(num_documents);
                    ndocs.append(padding - ndocs.length(), ' ');

                    std::cout << " " << bname << "  " << ndocs << std::endl;
                }
            }

            // Inserts a document into the bucket.
            void insertIntoBucket(std::string bucket_name, std::string id, std::string value)
            {
                // Trims the the quotes from both ends.
                this->trimQuotes(value);
                rapidjson::Document d;
                std::string parsable_json = "{\"" + id + "\": " + value + "}";

                rapidjson::ParseResult ok = d.Parse(parsable_json.c_str());

                if(!ok) {
                    std::string parse_error_message = "JSON parse error: " + std::string(rapidjson::GetParseError_En(ok.Code())) + " (" + std::to_string(ok.Offset()) + ")";
                    std::cerr << ElkOutput::OutputManager::getInstance()->getResponseString("error", parse_error_message) << std::endl;;
                    return;
                }

                if(!this->bucketExists(bucket_name)) {
                    std::cerr << ElkOutput::OutputManager::getInstance()->getResponseString("error", "Bucket Not Found.") << std::endl;
                    return;
                }


                Dictionary& bucket = this->getBucket(bucket_name);

                if(!this->documentExists(bucket, id))
                {
                    this->getBucket(bucket_name).insert(std::pair<std::string, std::string>(id, value));

                    std::cout << ElkOutput::OutputManager::getInstance()->getResponseString("success", "Document inserted successfully.") << std::endl;
                } else {
                    std::cerr << ElkOutput::OutputManager::getInstance()->getResponseString("error", "Document already exists.") << std::endl;
                    return;
                }
            }

            // Checks whether the document exists in the bucket.
            bool documentExists(Dictionary& bucket, std::string document_id)
            {

                if(bucket.find(document_id) != bucket.end())
                {
                    return true;
                }

                return false;
            }
        private:
            void trimQuotes(std::string& str)
            {
                std::replace(str.begin(), str.end(), '\"', '"');
            }


    };


    // class DocumentManager
    // {
    //     public:

    //         // Static instance of this class.
    //         static DocumentManager* s_instance;

    //         // Default Constructor
    //         DocumentManager() {}

    //         // Returns the single instance of this class.
    //         static DocumentManager* getInstance()
    //         {
    //             if(!s_instance) {
    //                 s_instance = new DocumentManager;
    //             }

    //             return s_instance;
    //         }

    //         // Checks whether the document exists in the bucket.
    //         bool documentExists(Dictionary& bucket, std::string document_id)
    //         {

    //             if(bucket.find(document_id) != bucket.end())
    //             {
    //                 return true;
    //             }

    //             return false;
    //         }

    // };
}

