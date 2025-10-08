#ifndef TFIDF_HPP
#define TFIDF_HPP
#include <vector>
#include <string>
#include <unordered_map>

using Vocab =  std::vector<std::string>;

Vocab build_vocab(const std::vector<std::vector<std::string>>& docs);
std::unordered_map<std::string, float> computerIDF(const std::vector<std::vector<std::string>>& docs);
std::unordered_map<std::string, float> computerTF(const std::vector<std::string>& tokens);


#endif // TFIDF_HPP
