
#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns false.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;

    void printValues();
    void fillEmpty();

private:
    HashFunction hashFunction;

    struct Node
    {
        ElementType data;
        Node* next = nullptr;
    };

    Node** arrPtr; 

    unsigned int capacity;
    unsigned int currSize;

    void addNode(const ElementType& element);


};


namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        std::hash<ElementType> myHash;

        return myHash(element);
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}, capacity{DEFAULT_CAPACITY}, currSize{0}
{
    arrPtr = new Node*[capacity];
    fillEmpty();
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}, capacity{s.capacity}, currSize{s.currSize}
{
    arrPtr = new Node*[capacity];
    fillEmpty();

    for(unsigned i = 0; i < capacity; i++)
    {
        Node* temp = s.arrPtr[i];

        while(temp != nullptr)
        {
            addNode(temp->data);
            temp = temp->next;
        }
    }

}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    this->hashFunction = std::move(s.hashFunction);
    this->capacity = std::move(s.capacity);
    this->currSize = std::move(s.currSize);
    
    this->arrPtr = new Node*[capacity];
    fillEmpty();

    for(unsigned i = 0; i < capacity; i++)
    {
        Node* temp = std::move(s.arrPtr[i]);

        while(temp != nullptr)
        {
            addNode(temp->data);
            temp = temp->next;
        }
    }
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if (this != &s)
    {
        this->hashFunction = s.hashFunction;
        this->capacity = s.capacity;
        this->currSize = s.currSize;

        this->arrPtr = new Node*[capacity];
        fillEmpty();

        for(unsigned i = 0; i < capacity; i++)
        {
            Node* temp = s.arrPtr[i];

            while(temp != nullptr)
            {
                addNode(temp->data);
                temp = temp->next;
            }
        }
    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    if(this != &s)
    {
        this->hashFunction = std::move(s.hashFunction);
        this->capacity = std::move(s.capacity);
        this->currSize = std::move(s.currSize);
        
        arrPtr = new Node*[capacity];
        fillEmpty();

        for(unsigned i = 0; i < capacity; i++)
        {
            Node* temp = std::move(s.arrPtr[i]);

            while(temp != nullptr)
            {
                addNode(temp->data);
                temp = temp->next;
            }
        }

    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if (!HashSet<ElementType>::contains(element))
    {
        if (currSize/capacity > 0.8)
        {
            addNode(element);

            Node** badArrPtr = arrPtr;

            unsigned int badCap = capacity;
            capacity = capacity * 2;

            arrPtr = new Node*[capacity];
            fillEmpty();
            currSize = 0;

            for (unsigned i = 0; i < badCap; ++i)
            {
                
                Node* temp3 = badArrPtr[i];
            
                while (temp3 != nullptr)
                {   
                    addNode(temp3->data);
                    temp3 = temp3->next;
                }
            }

            for (unsigned i = 0; i < badCap; ++i)
            {
                if (badArrPtr[i] != nullptr)
                {
                    delete badArrPtr[i];
                }
            }
            delete[] badArrPtr;
        }
        else
        {
            addNode(element);
        }
    }
}

template <typename ElementType>
void HashSet<ElementType>::fillEmpty()
{
    for (unsigned i = 0; i < capacity; ++i)
    {
        arrPtr[i] = nullptr;
    }
}
template <typename ElementType>
void HashSet<ElementType>::addNode(const ElementType& element)
{

    Node* temp = arrPtr[((unsigned int) hashFunction(element)) % capacity];
   
    if (temp == nullptr)
    {     
        arrPtr[((unsigned int) hashFunction(element)) % capacity] = new Node{element};
    }
    else
    {        
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = new Node{element};
    } 
    currSize += 1; 
}

template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    Node* temp = arrPtr[((unsigned int) hashFunction(element)) % capacity];
    
    if (temp == nullptr)
    {
        return false;
    }

    while (temp != nullptr)
    {
        if (temp->data == element)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return currSize;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
     if (index >= capacity)
    {
        return 0;
    }

    Node* temp = arrPtr[index];

    if (temp == nullptr)
    {
        return 0;
    }
    unsigned int count = 0;
    while (temp != nullptr)
    {
        count += 1;
        temp = temp->next;
    }
    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{

    if (index >= capacity)
    {
        return false;
    }

    Node* temp = arrPtr[index];

    if (temp == nullptr)
    {
        return false;
    }

    while (temp != nullptr)
    {
        if (temp->data == element)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;

}


template <typename ElementType>
void HashSet<ElementType>::printValues()
{
    for (unsigned i = 0; i < capacity; ++i)
    {
        Node* temp = arrPtr[i];
        if (temp != nullptr)
        {
            std::cout << "Bucket Content: ";
            while(temp != nullptr)
            {
                std::cout << temp->data  << ", ";
                temp = temp->next;
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "EMPTY BUCKET" << std::endl;
        }
    }
}


#endif

