#ifndef WORDS_H
#define WORDS_H

#include <iostream>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

struct TrieNode
{
    char letter;
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    std::string word;
    TrieNode() {}
    TrieNode(char c) : letter(c) {}
};

struct RectangleCell
{
    char letter;
    std::shared_ptr<TrieNode> node;
    std::unordered_map<char, std::shared_ptr<TrieNode>> vertPoss;
    std::unordered_map<char, std::shared_ptr<TrieNode>> horizPoss;
};

class Words
{
    private:
        std::shared_ptr<TrieNode> trieRoot;
        std::vector<std::unordered_set<std::string>> wordsBySize;

    public:
        Words(const std::unordered_set<std::string>& words);
        ~Words() {};
        std::vector<std::string> getMaxRectangle();

    private:
        std::vector<std::string> getWordsFromRectangle(const std::vector<std::vector<RectangleCell>>& rectangle);
        std::unordered_map<char, std::shared_ptr<TrieNode>> intersectMaps(const std::unordered_map<char, std::shared_ptr<TrieNode>>& map1,
                                                                          const std::unordered_map<char, std::shared_ptr<TrieNode>>& map2);
        bool checkRectangle(std::vector<std::vector<RectangleCell>>& rectangle, const size_t curRow, const size_t curCol);
        void makeTrie(const std::unordered_set<std::string>& words);
        void makeWords(const std::unordered_set<std::string>& words);
        void printRectangle(const std::vector<std::vector<RectangleCell>>& rectangle);
};

#endif // WORDS_H
