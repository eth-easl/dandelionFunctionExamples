#include "dandelion/runtime.h"

// #include <fstream>
#include "embedded_tokenizer_data.h"
// #include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <set>
#include <cstdint>

#define USER                882
#define ASSISTANT           78191
#define DOUBLE_NEW_LINE     271

#define START_HEADER        128006
#define END_HEADER          128007
#define EOT                 128009
#define PAD_TOKEN           128010

std::string base64_decode(const std::string &in) {
    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) {
        T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    }

    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

class Tokenizer {
public:
    bool load_vocab(const std::vector<std::string>& data) {
        for (const auto& line : data) {
            std::istringstream iss(line);
            std::string base64_token;
            int id;

            if (!(iss >> base64_token >> id)) continue;

            std::string token = base64_decode(base64_token);
            token_to_id[token] = id;
            id_to_token[id] = token;
            token_set.insert(token);
        }

        // Create a list of all token lengths (to optimize longest-match search)
        for (const auto& pair : token_to_id) {
            token_lengths.insert(pair.first.length());
        }

        return true;
    }

    void add_special_token(int id, const std::string& display) {
        special_token_map[id] = display;
    }

    std::vector<int> encode(const std::string& input, const int padded_length) {
        std::vector<int> ids;

        add_header(ids);

        size_t pos = 0;
        while (pos < input.size()) {
            int matched_id = -1;
            size_t matched_len = 0;

            // Try all token lengths (longest to shortest)
            for (auto len_it = token_lengths.rbegin(); len_it != token_lengths.rend(); ++len_it) {
                size_t len = *len_it;
                if (pos + len > input.size()) continue;

                std::string substr = input.substr(pos, len);
                auto it = token_to_id.find(substr);
                if (it != token_to_id.end()) {
                    matched_id = it->second;
                    matched_len = len;
                    break;
                }
            }

            if (matched_id != -1) {
                ids.push_back(matched_id);
                pos += matched_len;
            } else {
                std::cerr << "Unknown token starting at: " << input.substr(pos, 10) << std::endl;
                ids.push_back(-1);
                pos += 1;
            }
        }

        add_trailer(ids);

        while (ids.size() < padded_length) {
            ids.push_back(PAD_TOKEN);
        }

        return ids;
    }

    std::string decode(const std::vector<int>& ids) {
        std::ostringstream oss;
        for (int id : ids) {
            if (special_token_map.count(id)) {
                oss << special_token_map[id];
            } else if (id_to_token.count(id)) {
                oss << id_to_token[id];
            } else {
                oss << "<|UNKNOWN|>";
            }
        }
        return oss.str();
    }

private:
    std::unordered_map<std::string, int> token_to_id;
    std::unordered_map<int, std::string> id_to_token;
    std::unordered_map<int, std::string> special_token_map;
    std::set<std::string> token_set;
    std::set<size_t> token_lengths;

    void add_header(std::vector<int>& ids) {
        ids.push_back(START_HEADER);
        ids.push_back(USER);
        ids.push_back(END_HEADER);
        ids.push_back(DOUBLE_NEW_LINE);
    }

    void add_trailer(std::vector<int>& ids) {
        ids.push_back(EOT);
        ids.push_back(START_HEADER);
        ids.push_back(ASSISTANT);
        ids.push_back(END_HEADER);
        ids.push_back(DOUBLE_NEW_LINE);
    }
};

void write_vector_to_file(const std::vector<int>& data, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) throw std::runtime_error("Failed to open file for writing.");
    out.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(int32_t));
}

int test(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <padded_length> <string>" << std::endl;
        return 1;
    }

    Tokenizer tokenizer;
    tokenizer.load_vocab(embedded_tokenizer_data);

    tokenizer.add_special_token(START_HEADER, "<|start_header_id|>");
    tokenizer.add_special_token(END_HEADER, "<|end_header_id|>");
    tokenizer.add_special_token(EOT, "<|eot_id|>");
    tokenizer.add_special_token(PAD_TOKEN, "<|pad_token_id|>");

    int padded_length = std::atoi(argv[1]);
    std::string input = argv[2];
    std::vector<int> encoded = tokenizer.encode(input, padded_length);

    write_vector_to_file(encoded, "input_ids.bin");
    
    /*std::cout << "[";
    for (int i = 0; i < encoded.size(); i++) {
        std::cout << encoded[i];
        if (i != encoded.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;*/

    return 0;
}

void _start(void) {
    dandelion_init();

    Tokenizer tokenizer;
    tokenizer.load_vocab(embedded_tokenizer_data);

    tokenizer.add_special_token(START_HEADER, "<|start_header_id|>");
    tokenizer.add_special_token(END_HEADER, "<|end_header_id|>");
    tokenizer.add_special_token(EOT, "<|eot_id|>");
    tokenizer.add_special_token(PAD_TOKEN, "<|pad_token_id|>");

    /*int padded_length = 16;
    std::string input = "Hi, how are you?";
    std::vector<int> encoded = tokenizer.encode(input, padded_length);
    std::cout << encoded << std::endl;*/

    dandelion_exit(0);
}
