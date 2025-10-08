#ifndef TFIDF_HPP
#define TFIDF_HPP
#include <vector>
#include <string>

using Vocab =  std::vector<std::string>;

Vocab build_vocab(const std::vector<std::vector<std::string>> docs);

#endif // TFIDF_HPP
