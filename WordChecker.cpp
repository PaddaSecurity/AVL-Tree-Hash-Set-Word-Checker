// WordChecker.cpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <iostream>
#include <algorithm>


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{   
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    // Nonsensical code because the compiler requires the member variables
    // 'words' to be used somewhere, or else it becomes a warning (which
    // turns into an error).
    words.isImplemented();
    
    std::vector<std::string> allSuggestionsVector;

    //Delete
    for (unsigned i = 0; i < word.size(); ++i)
    {
        std::string bob = word;
        bob.erase(bob.begin() + i);

        if (words.contains(bob))
        {
            if (!(std::find(allSuggestionsVector.begin(), allSuggestionsVector.end(), bob) != allSuggestionsVector.end()))
            {
                allSuggestionsVector.push_back(bob);
            }
        }
    }

    //Replace each char
    std::string allLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (unsigned i = 0; i < word.size(); ++i)
    {
        for (unsigned j = 0; j < allLetters.length(); ++j)
        {
            std::string bob = word;
            bob[i] = allLetters[j];

            if (words.contains(bob))
            {
                if (!(std::find(allSuggestionsVector.begin(), allSuggestionsVector.end(), bob) != allSuggestionsVector.end()))
                {
                    allSuggestionsVector.push_back(bob);
                }
            }
        }    
    }

    //Swap each pair
    for (unsigned i = 0; i < word.size(); ++i)
    {
        std::string bob = word;
        char tim = bob[i];
        bob[i] = bob[i + 1];
        bob[i + 1] = tim;

        if (words.contains(bob))
        {
            if (!(std::find(allSuggestionsVector.begin(), allSuggestionsVector.end(), bob) != allSuggestionsVector.end()))
            {
                allSuggestionsVector.push_back(bob);
            }
        }
    }

    //Insert
    for (unsigned i = 0; i < word.size(); ++i)
    {   
        for (unsigned j = 0; j < allLetters.size(); ++j)
        {
            std::string bob = word;
            bob.insert(i, 1,allLetters[j]);
            
            if (words.contains(bob))
            {
                if (!(std::find(allSuggestionsVector.begin(), allSuggestionsVector.end(), bob) != allSuggestionsVector.end()))
                {
                    allSuggestionsVector.push_back(bob);
                }
            }
        }
    }

    //ADD Space
    for (unsigned i = 0; i < word.size() - 1; ++i)
    {
        std::string bob = word.substr(i, i + 1);
        std::string bob2 = word.substr(i + 1, word.length());

        if (words.contains(bob2) && words.contains(bob))
        {
            if (!(std::find(allSuggestionsVector.begin(), allSuggestionsVector.end(), bob) != allSuggestionsVector.end()))
            {
                allSuggestionsVector.push_back(bob);
            }
            if (!(std::find(allSuggestionsVector.begin(), allSuggestionsVector.end(), bob2) != allSuggestionsVector.end()))
            {
                allSuggestionsVector.push_back(bob2);
            }
        }

    }
    
    return allSuggestionsVector;
}

