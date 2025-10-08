#ifndef TFIDF_HPP
#define TFIDF_HPP
#include <vector>
#include <string>
#include <unordered_map>

using Vocab =  std::vector<std::string>;

struct LabeledDoc {
  std::string label;
  std::string text;
  std::vector<std::string> tokens;
  std::vector<float> vector;
};

Vocab build_vocab(const std::vector<std::vector<std::string>>& docs);
std::unordered_map<std::string, float> computerIDF(const std::vector<std::vector<std::string>>& docs);
std::unordered_map<std::string, float> computerTF(const std::vector<std::string>& tokens);
std::vector<float> vectorize(const std::vector<std::string>& tokens,
                            const Vocab& vocab,
                            const std::unordered_map<std::string, float>& ifd);
float cosine(const std::vector<float>& a, const std::vector<float>& b);
void print_vector(const std::vector<float>& vec, const Vocab& vocab);
LabeledDoc get_most_similar(const std::string& input,
                            const std::vector<LabeledDoc>& corpus,
                            const Vocab vocab,
                            const std::unordered_map<std::string, float>& idf);

#endif // TFIDF_HPP
