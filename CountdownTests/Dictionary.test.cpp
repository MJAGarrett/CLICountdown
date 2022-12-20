#include "pch.h"
#include "../Countdown/Dictionary.h"
#include <map>
#include <fstream>
#include <algorithm>

namespace DictTestHelpers
{
  bool hasAllSameCharacters(const std::string& word, const std::unordered_map<char, int>& charCount)
  {
    std::unordered_map<char, int> comparisonMap;
    std::set<char> lettersToCheck;

    for (auto& entry : charCount)
    {
      lettersToCheck.insert(entry.first);
    }

    for (auto letter : word)
    {
      comparisonMap[letter]++;
      lettersToCheck.insert(letter);
    }

    for (auto& letter : lettersToCheck)
    {
      auto originalEntry = charCount.find(letter);
      auto comparisonEntry = comparisonMap.find(letter);
      if (originalEntry == charCount.end() || comparisonEntry == comparisonMap.end()) return false;
      if ((*originalEntry).second != (*comparisonEntry).second) return false;
    }

    return true;
  }

  /**
   * @brief Checks if a potential conundrum is 9 letters long and made up of some combination of words that are at least 3 letters long
   * and also stored in the given dictionary.
   */
  bool isWordConundrum(std::string word, Dictionary* dict)
  {
    std::unordered_map<char, int> letterMap;
    for (auto letter : word)
    {
      letterMap[letter]++;
    }

    if (!hasAllSameCharacters(dict->getConundrumBaseWord(), letterMap)) return false;
    if (dict->lookUpWord(word.substr(0, 3)) && dict->lookUpWord(word.substr(3)))
      return true;
    if (dict->lookUpWord(word.substr(0, 4)) && dict->lookUpWord(word.substr(4)))
      return true;
    if (dict->lookUpWord(word.substr(0, 5)) && dict->lookUpWord(word.substr(5)))
      return true;
    if (dict->lookUpWord(word.substr(0, 6)) && dict->lookUpWord(word.substr(6)))
      return true;
    else if (dict->lookUpWord(word.substr(0, 3)) 
      && dict->lookUpWord(word.substr(3, 3)) 
      && dict->lookUpWord(word.substr(6)))
        return true;

    return false;
  }
}

class DictionaryFixture : public ::testing::Test
{
public:
  Dictionary* dict = new Dictionary();

  virtual ~DictionaryFixture()
  {
    delete dict;
  }
};

TEST_F(DictionaryFixture, CanFindXLengthWords)
{
  const std::vector<std::string> fiveLetterWords = { "hello", "times", "sauna", "habit", "sunny", "sting" };
  const std::vector<std::string> sevenLetterWords = { "abalone", "dabbles", "habitat" };

  for (const std::string& word : fiveLetterWords)
  {
    dict->addWord(word);
  }
  for (const std::string& word : sevenLetterWords)
  {
    dict->addWord(word);
  }

  std::vector<std::string> resultsForSeven = dict->findXLengthWords(7);
  EXPECT_EQ(sevenLetterWords.size(), resultsForSeven.size());

  std::vector<std::string> resultsForFive = dict->findXLengthWords(5);
  EXPECT_EQ(fiveLetterWords.size(), resultsForFive.size());

  for (const auto& result : resultsForFive)
  {
    EXPECT_TRUE(std::find(fiveLetterWords.begin(), fiveLetterWords.end(), result) != fiveLetterWords.end());
  }
  for (const auto& result : resultsForSeven)
  {
    EXPECT_TRUE(std::find(sevenLetterWords.begin(), sevenLetterWords.end(), result) != sevenLetterWords.end());
  }
}

TEST_F(DictionaryFixture, CanLookUpAWordInTheDictionary) {
  dict->addWord("hello");
  bool hasWord = dict->lookUpWord("hello");

  // It should return true if a word is in the dictionary.
  EXPECT_TRUE(hasWord);
  // It should return false if a word is not in the dictionary.
  EXPECT_FALSE(dict->lookUpWord("supercalifragilisticexpialidocious"));
}

TEST_F(DictionaryFixture, CanGetAllWordsWithPrefix)
{
  std::vector<std::string> validWords = { "prescient", "premature", "prevalent", "precognition" };
  std::vector<std::string> invalidWords = { "hello", "sapient", "tallow", "tacit", "monstruous" };

  dict->addWord(validWords);
  dict->addWord(invalidWords);

  std::vector<std::string> foundWords = dict->getWordsWithPrefix("pre");

  // It should return all words beginning with the given prefix.

  EXPECT_EQ(validWords.size(), foundWords.size());

  for (auto& word : foundWords)
  {
    EXPECT_TRUE(std::find(validWords.begin(), validWords.end(), word) != validWords.end());
  }
}

TEST_F(DictionaryFixture, CanRemoveAWordWithoutSideEffects)
{
  std::vector<std::string> words = { "hello", "helios", "helicopter", "halo", "happy", "sandy", "sapiens" };

  dict->addWord(words);
  
  dict->removeWord("sandy");
  EXPECT_FALSE(dict->lookUpWord("sandy"));

  auto remainingWords = dict->getAllWords();

  EXPECT_EQ(dict->numberOfWords(), words.size() - 1);

  for (auto& word : remainingWords)
  {
    EXPECT_NE(word, "sandy");
    EXPECT_TRUE(std::find(words.begin(), words.end(), word) != words.end());
  }
}

TEST_F(DictionaryFixture, CanClearDictionary)
{
  std::vector<std::string> words = { "sample", "test", "other", "word", "no", "more", "ideas" };
  dict->addWord(words);

  dict->clearDictionary();

  EXPECT_EQ(dict->numberOfWords(), 0);

  for (auto& word : words)
  {
    EXPECT_FALSE(dict->lookUpWord(word));
  }
}

TEST_F(DictionaryFixture, CanFindAllValidWordsGivenASetOfLetters)
{
  std::vector<std::string> allWords = { "stop", "statute", "hello", "test" };
  std::vector<std::string> targetWords = { "stop", "statute", "test" };
  std::vector<char> letters = { 'o', 'p', 't', 's', 'e', 't', 'a', 'u', 't'};

  dict->addWord(allWords);

  auto foundWords = dict->findPossibleWordsInSet(letters);

  EXPECT_EQ(targetWords.size(), foundWords.size());

  for (auto& word : targetWords)
  {
    EXPECT_TRUE(std::find(foundWords.begin(), foundWords.end(), word) != foundWords.end());
  }

}

TEST_F(DictionaryFixture, CanFindLongestWordsPossibleGivenSetOfLetters)
{
  std::vector<std::string> words = {"happy", "peppy", "xylophone", "samaritan", "porcupine", "apple", "ha", "pep", "lap"};
  std::vector<std::string> targetWords = { "happy", "peppy", "apple" };
  std::vector<char> letters = { 'h', 'a', 'p', 'p', 'y', 'e', 'p', 'a', 'l' };

  dict->addWord(words);

  auto found = dict->findLongestPossibleWordsInSet(letters);

  EXPECT_EQ(targetWords.size(), found.size());

  for (auto& word : targetWords)
  {
    EXPECT_TRUE(std::find(found.begin(), found.end(), word) != found.end());
  }
}

TEST_F(DictionaryFixture, CanMakeConundrum)
{
  delete dict;
  dict = new Dictionary("Words.txt");
  const auto conundrum = dict->makeConundrum();

  EXPECT_TRUE(DictTestHelpers::isWordConundrum(conundrum, dict));
}

TEST_F(DictionaryFixture, CanGetConundrumBaseWord)
{
  dict->addWord("china");
  dict->addWord("lung");
  dict->addWord("launching");
  dict->makeConundrum();

  std::string conundrumBase = dict->getConundrumBaseWord();
  EXPECT_TRUE(conundrumBase != "");
  EXPECT_TRUE(conundrumBase == "launching");
}

TEST_F(DictionaryFixture, CanSaveWordsToFile)
{
  std::vector<std::string> wordsToAdd = { "henna", "pinapple", "saturn", "taco" };
  dict->addWord(wordsToAdd);

  dict->saveWords("test.txt");

  std::ifstream file("test.txt");

  ASSERT_FALSE(file.fail());
  
  std::vector<std::string> loadedWords;
  std::string line;
  while (!file.eof())
  {
    std::getline(file, line, ',');
    if (line != "")
      loadedWords.push_back(line);
  }
  file.close();
  std::remove("test.txt");

  std::vector<std::string> symmetricDiff;
  std::sort(loadedWords.begin(), loadedWords.end());

  std::set_symmetric_difference(loadedWords.begin(), loadedWords.end(), wordsToAdd.begin(), wordsToAdd.end(), std::back_inserter(symmetricDiff));

  EXPECT_TRUE(symmetricDiff.size() == 0);
}