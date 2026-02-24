#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cstdlib>
#include "json.h"
#include "httplib.h"

using json = nlohmann::json;

class Tool;
class ToolRegistry;
class LLMAgent;

class Tool {
public:
    std::string name;
    std::string description;
    json parameters;
    
    Tool(const std::string& name, const std::string& description, const json& params)
        : name(name), description(description), parameters(params) {}
};

class ToolRegistry {
public:
    void register_tool(const Tool& tool, std::function<json(const json&)> func) {
        tools_[tool.name] = tool;
        executors_[tool.name] = func;
    }
    
    json get_tool_definition(const std::string& name) const {
        if (tools_.find(name) == tools_.end()) {
            return json();
        }
        
        json tool_def;
        tool_def["type"] = "function";
        tool_def["function"]["name"] = tools_.at(name).name;
        tool_def["function"]["description"] = tools_.at(name).description;
        tool_def["function"]["parameters"] = tools_.at(name).parameters;
        return tool_def;
    }
    
    json get_all_tool_definitions() const {
        json tools_array = json::array();
        for (const auto& pair : tools_) {
            tools_array.push_back(get_tool_definition(pair.first));
        }
        return tools_array;
    }
    
    json execute(const std::string& tool_name, const json& arguments) {
        if (executors_.find(tool_name) == executors_.end()) {
            json error;
            error["error"] = "Tool not found: " + tool_name;
            return error;
        }
        
        try {
            return executors_.at(tool_name)(arguments);
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Execution error: ") + e.what();
            return error;
        }
    }
    
private:
    std::map<std::string, Tool> tools_;
    std::map<std::string, std::function<json(const json&)>> executors_;
};

class LLMAgent {
public:
    LLMAgent(const std::string& api_key) 
        : api_key_(api_key), client_("api.openai.com"), max_iterations_(10) {
        client_.enable_server_certificate_verification(false);
        setup_tools();
    }
    
    std::string query(const std::string& user_query) {
        messages_.clear();
        
        json user_msg;
        user_msg["role"] = "user";
        user_msg["content"] = user_query;
        messages_.push_back(user_msg);
        
        for (int iteration = 0; iteration < max_iterations_; ++iteration) {
            json response = call_openai_api();
            
            if (response.is_null() || !response.contains("choices") || 
                response["choices"].size() == 0) {
                return "Error: Invalid response from OpenAI API";
            }
            
            json choice = response["choices"][0];
            json message = choice["message"];
            
            messages_.push_back(message);
            
            if (message.contains("tool_calls") && message["tool_calls"].size() > 0) {
                for (const auto& tool_call : message["tool_calls"]) {
                    std::string tool_name = tool_call["function"]["name"];
                    std::string tool_args_str = tool_call["function"]["arguments"];
                    
                    json tool_args = json::parse(tool_args_str);
                    
                    json tool_result = registry_.execute(tool_name, tool_args);
                    
                    json tool_msg;
                    tool_msg["role"] = "tool";
                    tool_msg["tool_call_id"] = tool_call["id"];
                    tool_msg["name"] = tool_name;
                    tool_msg["content"] = tool_result.dump();
                    
                    messages_.push_back(tool_msg);
                }
            } else {
                if (message.contains("content") && !message["content"].is_null()) {
                    return message["content"].get<std::string>();
                } else {
                    return "Error: No content in response";
                }
            }
        }
        
        return "Error: Maximum iterations reached";
    }
    
private:
    std::string api_key_;
    httplib::SSLClient client_;
    ToolRegistry registry_;
    json messages_;
    int max_iterations_;
    
    void setup_tools() {
        json calc_params;
        calc_params["type"] = "object";
        calc_params["properties"]["operation"]["type"] = "string";
        calc_params["properties"]["operation"]["enum"] = json::array({"add", "subtract", "multiply", "divide"});
        calc_params["properties"]["a"]["type"] = "number";
        calc_params["properties"]["b"]["type"] = "number";
        calc_params["required"] = json::array({"operation", "a", "b"});
        
        Tool calc_tool("calculator", 
                      "Performs arithmetic operations (add, subtract, multiply, divide) on two numbers",
                      calc_params);
        
        registry_.register_tool(calc_tool, [](const json& args) -> json {
            json result;
            std::string op = args["operation"];
            double a = args["a"];
            double b = args["b"];
            
            if (op == "add") {
                result["result"] = a + b;
            } else if (op == "subtract") {
                result["result"] = a - b;
            } else if (op == "multiply") {
                result["result"] = a * b;
            } else if (op == "divide") {
                if (b == 0) {
                    result["error"] = "Division by zero";
                } else {
                    result["result"] = a / b;
                }
            } else {
                result["error"] = "Unknown operation: " + op;
            }
            return result;
        });
        
        json stats_params;
        stats_params["type"] = "object";
        stats_params["properties"]["numbers"]["type"] = "array";
        stats_params["properties"]["numbers"]["items"]["type"] = "number";
        stats_params["properties"]["operation"]["type"] = "string";
        stats_params["properties"]["operation"]["enum"] = json::array({"mean", "stddev", "median"});
        stats_params["required"] = json::array({"numbers", "operation"});
        
        Tool stats_tool("statistics",
                        "Calculates statistical measures (mean, standard deviation, median) of a list of numbers",
                        stats_params);
        
        registry_.register_tool(stats_tool, [](const json& args) -> json {
            json result;
            std::string op = args["operation"];
            json numbers_json = args["numbers"];
            
            std::vector<double> numbers;
            for (const auto& num : numbers_json) {
                numbers.push_back(num.get<double>());
            }
            
            if (numbers.empty()) {
                result["error"] = "Empty number list";
                return result;
            }
            
            if (op == "mean") {
                double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
                result["result"] = sum / numbers.size();
            } else if (op == "stddev") {
                double mean = std::accumulate(numbers.begin(), numbers.end(), 0.0) / numbers.size();
                double variance = 0.0;
                for (double num : numbers) {
                    variance += (num - mean) * (num - mean);
                }
                variance /= numbers.size();
                result["result"] = std::sqrt(variance);
            } else if (op == "median") {
                std::vector<double> sorted = numbers;
                std::sort(sorted.begin(), sorted.end());
                size_t n = sorted.size();
                if (n % 2 == 0) {
                    result["result"] = (sorted[n/2 - 1] + sorted[n/2]) / 2.0;
                } else {
                    result["result"] = sorted[n/2];
                }
            } else {
                result["error"] = "Unknown operation: " + op;
            }
            return result;
        });
        
        json fib_params;
        fib_params["type"] = "object";
        fib_params["properties"]["count"]["type"] = "integer";
        fib_params["properties"]["count"]["description"] = "Number of Fibonacci numbers to generate";
        fib_params["required"] = json::array({"count"});
        
        Tool fib_tool("fibonacci",
                     "Generates a sequence of Fibonacci numbers",
                     fib_params);
        
        registry_.register_tool(fib_tool, [](const json& args) -> json {
            json result;
            int count = args["count"];
            
            if (count <= 0) {
                result["error"] = "Count must be positive";
                return result;
            }
            
            json sequence = json::array();
            if (count >= 1) sequence.push_back(0);
            if (count >= 2) sequence.push_back(1);
            
            long long a = 0, b = 1;
            for (int i = 2; i < count; ++i) {
                long long next = a + b;
                sequence.push_back(next);
                a = b;
                b = next;
            }
            
            result["sequence"] = sequence;
            return result;
        });
    }
    
    json call_openai_api() {
        json request;
        request["model"] = "gpt-5-nano";
        request["messages"] = messages_;
        request["tools"] = registry_.get_all_tool_definitions();
        request["tool_choice"] = "auto";
        
        std::string request_body = request.dump();
        
        httplib::Headers headers = {
            {"Authorization", "Bearer " + api_key_},
            {"Content-Type", "application/json"}
        };
        
        auto res = client_.Post("/v1/chat/completions", headers, request_body, "application/json");
        
        if (!res || res->status != 200) {
            std::cerr << "Error: HTTP " << (res ? res->status : -1) << std::endl;
            if (res) {
                std::cerr << "Response: " << res->body << std::endl;
            }
            return json();
        }
        
        try {
            return json::parse(res->body);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
            return json();
        }
    }
};

int main(int argc, char* argv[]) {
    const char* api_key_env = std::getenv("OPENAI_API_KEY");
    if (!api_key_env) {
        std::cerr << "Error: OPENAI_API_KEY environment variable not set" << std::endl;
        return 1;
    }
    
    std::string api_key = api_key_env;
    LLMAgent agent(api_key);
    
    std::vector<std::string> test_queries = {
        "What's 2847 * 3921?",
        "Find the mean and standard deviation of: 23, 45, 67, 12, 89, 34, 56, 78",
        "Generate the first 20 Fibonacci numbers, then calculate their mean and median."
    };
    
    for (const auto& query : test_queries) {
        std::cout << "\n=== Query: " << query << " ===" << std::endl;
        std::string response = agent.query(query);
        std::cout << "Response: " << response << std::endl;
    }
    
    return 0;
}
