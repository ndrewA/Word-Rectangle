#include "Words.h"

constexpr int MAX_LEN = 10;

Words::Words(const std::unordered_set<std::string>& words) : wordsBySize(MAX_LEN)
{
    trieRoot = std::make_shared<TrieNode>();

    makeWords(words);
    makeTrie(words);
}

void Words::makeTrie(const std::unordered_set<std::string>& words)
{
    for(const auto& curWord : words) {
        std::shared_ptr<TrieNode> nodeIter = trieRoot;

        for(auto curLetter : curWord) {

            if(nodeIter->children.find(curLetter) == nodeIter->children.end())
                nodeIter->children.emplace(curLetter, std::make_shared<TrieNode>(curLetter));

            nodeIter = nodeIter->children[curLetter];
        }
        nodeIter->word = curWord;
    }
}

void Words::makeWords(const std::unordered_set<std::string>& words)
{
    for(const auto& curWord : words)
        wordsBySize[curWord.size()].insert(curWord);
}

std::vector<std::string> Words::getMaxRectangle()
{
    for(int slow = wordsBySize.size()-1; slow >= 0; slow--) {
        if(wordsBySize[slow].empty()) continue;
        for(int fast = wordsBySize.size()-1; fast >= slow; fast--) {
            if(wordsBySize[fast].empty()) continue;
            std::vector<std::vector<RectangleCell>> rectangle;
            for(size_t i = 0; i < (unsigned)slow; i++)
                rectangle.push_back(std::vector<RectangleCell>(fast));
            rectangle[0][0].vertPoss = trieRoot->children;
            if(checkRectangle(rectangle, 0, 0)) {
                printRectangle(rectangle);
                return getWordsFromRectangle(rectangle);
            }
        }
    }
    return std::vector<std::string>();
}

std::vector<std::string> Words::getWordsFromRectangle(const std::vector<std::vector<RectangleCell>>& rectangle)
{
    std::vector<std::string> rectangleWords;
    for(size_t i = 0; i < rectangle.size(); i++) {
        std::string curWord;
        for(size_t j = 0; j < rectangle[0].size(); j++)
            curWord += rectangle[i][j].node->letter;
        rectangleWords.push_back(curWord);
    }
    return rectangleWords;
}

std::unordered_map<char, std::shared_ptr<TrieNode>> Words::intersectMaps(const std::unordered_map<char, std::shared_ptr<TrieNode>>& map1,
                                                                  const std::unordered_map<char, std::shared_ptr<TrieNode>>& map2)
{
    std::unordered_map<char, std::shared_ptr<TrieNode>> intersection;
    for(const auto& iter : map1)
        if(map2.find(iter.first) != map2.end())
            intersection.insert(iter);
    return intersection;
}

bool Words::checkRectangle(std::vector<std::vector<RectangleCell>>& rectangle,
                           const size_t curRow, const size_t curCol)
{
    if(curRow == rectangle[0].size()-1 && curCol == rectangle.size()-1) {
        auto intersection = intersectMaps(rectangle[curRow][curCol].horizPoss, rectangle[curRow][curCol].vertPoss);
        if(!intersection.empty()) {
            rectangle[curRow][curCol].node = intersection.begin()->second;
            return true;
        } else {
            return false;
        }
    }
    if(curCol+1 > rectangle.size()) return checkRectangle(rectangle, curRow+1, 0);

    if(curRow == 0 && curCol == 0) {
        for(const auto& iter : rectangle[0][0].vertPoss) {
            std::shared_ptr<TrieNode> curNode = iter.second;
            rectangle[0][0].node = curNode;

            rectangle[0][1].horizPoss = curNode->children;
            rectangle[1][0].vertPoss = curNode->children;

            if(checkRectangle(rectangle, curRow, curCol+1)) return true;
        }
    } else if(curRow == 0) {
        if(rectangle[0][curCol].horizPoss.empty()) return false;
        for(const auto& iter : rectangle[0][curCol].horizPoss) {
            char curLetter = iter.first;
            std::shared_ptr<TrieNode> curNode = iter.second;

            rectangle[0][curCol].node = curNode;

            if(trieRoot->children.find(curLetter) == trieRoot->children.end())
                continue;
            else
                rectangle[1][curCol].vertPoss = trieRoot->children[curLetter]->children;

            if(curCol+1 < rectangle.size()) rectangle[0][curCol+1].horizPoss = curNode->children;
            if(checkRectangle(rectangle, curRow, curCol+1)) return true;
        }
    } else if(curCol == 0) {
        if(rectangle[curRow][0].vertPoss.empty()) return false;
        for(const auto& iter : rectangle[curRow][0].vertPoss) {
            char curLetter = iter.first;
            std::shared_ptr<TrieNode> curNode = iter.second;
            rectangle[curRow][0].node = curNode;

            if(trieRoot->children.find(curLetter) == trieRoot->children.end())
                continue;
            else
                rectangle[curRow][1].horizPoss = trieRoot->children[curLetter]->children;

            if(curRow+1 < rectangle[0].size()) rectangle[curRow+1][0].vertPoss = curNode->children;
            if(checkRectangle(rectangle, curRow, curCol+1)) return true;
        }
    } else {
        auto intersection = intersectMaps(rectangle[curRow][curCol].vertPoss, rectangle[curRow][curCol].horizPoss);
        if(intersection.empty()) return false;
        for(const auto& iter : intersection) {
            std::shared_ptr<TrieNode> curNode = iter.second;
            rectangle[curRow][curCol].node = curNode;

            if(curCol+1 < rectangle.size()) {
                rectangle[curRow][curCol+1].horizPoss = rectangle[curRow][curCol].horizPoss[curNode->letter]->children;
            }
            if(curRow+1 < rectangle[0].size()){
                rectangle[curRow+1][curCol].vertPoss = rectangle[curRow][curCol].vertPoss[curNode->letter]->children;
            }

            if(checkRectangle(rectangle, curRow, curCol+1)) return true;
        }
    }
    return false;
}

void Words::printRectangle(const std::vector<std::vector<RectangleCell>>& rectangle)
{
    for(size_t i = 0; i < rectangle.size(); i++) {
        for(size_t j = 0; j < rectangle[0].size(); j++) {
            if(rectangle[i][j].node != nullptr) {
                std::cout << "(" << rectangle[i][j].node->letter << ") ";
            } else {
                std::cout << "[";
                for(const auto& [letter, node] : rectangle[i][j].vertPoss)
                    std::cout << letter << ' ';
                std::cout << " | ";
                for(const auto& [letter, node] : rectangle[i][j].horizPoss)
                    std::cout << letter << ' ';
                std::cout << "] ";
            }
        }
        std::cout << '\n';
    }
}
