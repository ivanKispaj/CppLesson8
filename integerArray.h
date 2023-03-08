//
// integerArray.h
// 2023-02-03
// Ivan Konishchev
//

#pragma once
#include <iostream>
#include <optional>
#include "BadAnswer.h"
#include <typeinfo>

template <typename T>
class Array
{
    // private property
    int _size{0};
    std::unique_ptr<T[]> _array = nullptr;
    bool _isEmpty{true};
    T _returnValue;

    // private method
    // erase array ( private method )
    /*
        This method resets the array
    */
    void erase()
    {
        _array = nullptr;
        _isEmpty = true;
        _returnValue = T();
    }

    // private method sort
    Array<T> sortThis(Array<T> arr)
    {
        if (arr.count() == 0)
        {
            return arr;
        }
        T pivot = arr[arr.count() / 2];
        Array<T> less = arr.filter('<', pivot);
        Array<T> equal = arr.filter('=', pivot);
        Array<T> great = arr.filter('>', pivot);
        return sortThis(less) + equal + sortThis(great);
    }

public:
    // Init default
    Array<T>() : _array(nullptr), _size(0), _isEmpty(true), _returnValue(T()) {};


    // Init with size
    /*
        Negative value size, will throw an exception and create an empty array
        Using:
        integerArray array; - this create epmty array!
        integerArray array(0) / integerArray array(-1) - this create epmty array! and will throw an exception message!
        integerArray array(5) - create array leinght 5 and default value - 0
    */
   explicit Array<T>(int size)
    {
        try
        {
            if (size <= 0)
            {
                throw BadAnswer(INCORRECT_ARRAY_LEINGHT);
            }
            else
            {
                _array = new T[size];
                _size = size;
                _isEmpty = false;
                _returnValue = T();
            }
        }
        catch (const BadAnswer &error)
        {
            _array = nullptr;
            _size = 0;
            _isEmpty = true;
            _returnValue = T();
            std::cerr << error.showError();
        }
    }

    //  Copy construction
    Array<T>(const Array &array)
    {
        erase();
        _isEmpty = array._isEmpty;
        _size = array._size;
        _array = std::make_unique<T[]>(array._size);
        for (int i = 0; i < _size; i++)
        {
            _array[i] = array._array[i];
        }
    }

    // Deinit
    ~Array<T>() = default;

    // return array leinght
    int count() const
    {
        return _size;
    }

    /*
    This method returns true if the array is empty, and false if the values exist
    */
    bool isEmpty() const
    {
        return _isEmpty;
    }

    // append an element to end of the array
    /*
        This method adds a value to the end of the array,
         and increases the size of the array
    */
    void append(const T& value) // append an element to end of the array
    {
        _size++;
        std::unique_ptr<T[]>  newArray = std::make_unique<T[]>(_size);

        if (_size <= 1)
        {
            newArray[0] = value;
        }
        else
        {
            for (int i = 0; i < _size - 1; i++)
            {
                newArray[i] = _array[i];
            }
            newArray[_size - 1] = value;
        }

        erase();

        _isEmpty = false;
        _array = std::move(newArray);
    }

    // Inserts an element into an array by index
    /*
        This method adds a value to the passed index and expands the array!
        You need to make sure that the index exists,
        otherwise the value will not be added and an exception will occur
        - Error message: "Array is empty!" if array is empty.
        - Error message: "Index out of range!" if array dont contain an index
    */
    void insertAt(int index, const T& value) // Inserts an element into an array by index
    {
        try
        {
            if (index < 0 || index > _size)
            {
                throw BadAnswer(INDEX_OUT_OF_RANGE);
            }
            else if (_isEmpty && index != 0)
            {
                throw BadAnswer(ARRAY_IS_EMPTY);
            }

            _size++;
            std::unique_ptr<T[]> newArray = std::make_unique<T[]>(_size);

            for (int i = 0; i < index; i++)
            {
                newArray[i] = _array[i];
            }

            newArray[index] = value;

            if (index + 1 != _size)
            {
                for (int i = index; i < _size; i++)
                {
                    newArray[i + 1] = _array[i];
                }
            }

            erase();
            _isEmpty = false;
            _array = std::move(newArray);
        }
        catch (const BadAnswer &error)
        {
            std::cerr << error.showError();
        }
    }

    // return first element from array
    /*
        This method returns a pointer to the first element of the array.
        - Error message: "Array is empty!" if array is empty.
    */
    std::optional<T> first() const // return first element from array
    {
        try
        {
            if (_size > 0)
            {
                return _array[0];
            }
            throw BadAnswer(ARRAY_IS_EMPTY);
        }
        catch (const BadAnswer &error)
        {
            std::cerr << error.showError();
            return std::nullopt;
        }
    }

    // return last element from array
    /*
        This method returns a pointer to the last value of the array.
        - Error message: "Array is empty!" if array is empty.
    */
    std::optional<T> last() const // return last element from array
    {
        try
        {
            if (_size > 0)
            {
                return _array[_size - 1];
            }
            throw BadAnswer(ARRAY_IS_EMPTY);
        }
        catch (const BadAnswer &error)
        {
            std::cerr << error.showError();
            return std::nullopt;
        }
    }

    // Removes the first element from the array and returns it
    /*
        This method deletes the first value of the array,
        reduces the size of the array, and returns a pointer to the deleted value.
        - Error message: "Array is empty!" if array is empty.
    */
    std::optional<T> removeFirst() // Removes the first element from the array and returns it
    {
        try
        {
            if (_size <= 0)
            {
                erase();
                throw BadAnswer(ARRAY_IS_EMPTY);
            }

            _size--;
            _returnValue = _array[0];

            if (_size == 0)
            {
                erase();
            }
            else
            {
                std::unique_ptr<T[]> newArray = std::make_unique<T[]>(_size);
                for (int i = 0; i < _size; i++)
                {
                    newArray[i] = _array[i + 1];
                }
                erase();
                _isEmpty = false;
                _array = std::move(newArray);
            }

            return _returnValue;
        }
        catch (const BadAnswer &error)
        {
            std::cerr << error.showError();
            return std::nullopt;
        }
    }

    //  Removes the last element from the array and returns it
    /*
        This method deletes the last value of the array,
        reduces the size of the array, and returns a pointer to the deleted value.
        - Error message: "Array is empty!" if array is empty.
    */
    std::optional<T> removeLast()
    {
        try
        {
            if (_size <= 0)
            {
                erase();
                throw BadAnswer(ARRAY_IS_EMPTY);
            }
            _size--;
            _returnValue = _array[_size];
            if (_size == 0)
            {
                erase();
            }

            return _returnValue;
        }
        catch (const BadAnswer &error)
        {
            std::cerr << error.showError();
            return std::nullopt;
        }
    }

    // remove value by index
    /*
        This method deletes the value of the array by index,
        reduces the size of the array, and returns a pointer to the deleted value.

        It is necessary to make sure that the index exists,
        If the index is missing or the array is empty, an exception is thrown.

        - Error message: "Array is empty!" if array is empty.
        - Error message: "Index out of range!" if array dont contain an index
    */
    std::optional<T> removeAt(const int index)
    {
        try
        {
            if (_isEmpty)
            {
                throw BadAnswer(ARRAY_IS_EMPTY);
            }
            if (index > _size || index < 0)
            {
                throw BadAnswer(INDEX_OUT_OF_RANGE);
            }

            _returnValue = _array[index];
            int *newArray = new int[_size - 1];
            for (int i = 0; i < index; i++)
            {
                newArray[i] = _array[i];
            }
            for (int i = index + 1; i < _size; i++)
            {
                newArray[i - 1] = _array[i];
            }

            erase();
            _isEmpty = false;
            _size--;
            _array = newArray;
            return _returnValue;
        }
        catch (const BadAnswer &error)
        {
            std::cerr << error.showError();
            return std::nullopt;
        }
    }

    // Array search by value
    /*
        This method searches the array for the passed value,
        and returns a pointer to the index.
        Otherwise it will return nullptr.
        - Returns a pointer to the first index of the value or nullptr if the value is not found.
        - throw Error if array is empty and return nullptr
    */
    std::optional<int> getFirstIndexWhere(const T& value) // Array search by value
    {
        try
        {
            if (_size == 0)
            {
                throw BadAnswer(ARRAY_IS_EMPTY);
            }
            for (int i = 0; i < _size; i++)
            {
                if (_array[i] == value)
                {
                    return i;
                }
            }
            return std::nullopt;
        }
        catch (const BadAnswer &error)
        {
            std::cerr << error.showError();
            return std::nullopt;
        }
    }

    // insert value at thebeginning
    /*
        this method isert value to the beginning of the array
    */
    void inserFirst(const T& value) // Adding an element to the beginning of the array
    {
        insertAt(0, value);
    }

    // This method printing the array
    void printArray() const
    {
        for (int i = 0; i < _size; i++)
        {
            std::cout << "index: " << i << " значение: " << _array[i] << std::endl;
        }
    }

    // Public sort method
    /*
        Sorting Operators char -> '>'  and  '<'
        Вefault operator '>'
        incorrect operator enable sorting by default '>'
    */
    void sort(char operation = '>') // sort method
    {
        Array<T> res = sortThis(*this);
        std::unique_ptr<T[]> newArray = std::make_unique<T[]>(res.count());
        switch (operation)
        {
        case '>':
            for (int i = 0; i < res.count(); i++)
            {
                newArray[i] = res[i];
            }
            break;
        case '<':
            int revers = res.count() - 1;
            for (int i = 0; i < res.count(); i++)
            {
                newArray[revers - i] = res[i];
            }
            break;
        }
        _array = std::move(newArray);
    }

    // Method for filtering an array
    /*
        Method for filtering an array.
        operators: type char :  '<' '>' '='
        Returns a container with elements filtered by one of 3 rules:
        - '>' Returns a container with elements larger than the value
        - '<' Returns a container with elements less than the value
        - '=' Returns a container with elements equal to the value

    */
    Array<T> filter(char operation, const T& value) const // Method for filtering an array
    {
        Array<T> retArr;
        switch (operation)
        {
        case '<':
            for (int i = 0; i < _size; i++)
            {
                if (_array[i] < value)
                {
                    retArr.append(_array[i]);
                }
            }
            return retArr;
        case '>':
            for (int i = 0; i < _size; i++)
            {
                if (_array[i] > value)
                {
                    retArr.append(_array[i]);
                }
            }
            return retArr;
        case '=':
            for (int i = 0; i < _size; i++)
            {
                if (_array[i] == value)
                {
                    retArr.append(_array[i]);
                }
            }
            return retArr;

        default:
            throw std::overflow_error("Incorrect operator!");
        }
    }

    /*
    overloaded index taking operator.
    Returns a pointer to the array value by index,
    nullptr if array is empry or index out if range..
    */
    T &operator[](const int index)
    {

        if (index >= 0 && index < _size)
        {
            return _array[index];
        }
        if (_isEmpty)
        {
            throw BadAnswer(ARRAY_IS_EMPTY).showError();
        }
        throw std::overflow_error("Index out of range!!!");
    }

    T &operator[](const int index) const
    {

        if (index >= 0 && index < _size)
        {
            return _array[index];
        }
        if (_isEmpty)
        {
            throw BadAnswer(ARRAY_IS_EMPTY).showError();
        }
        throw std::overflow_error("Index out of range!!!");
    }


    // array concatenation
    Array<T> operator+(const Array<T> &array)
    {
        if (_isEmpty && array._isEmpty)
        {
            return *this;
        }

        _isEmpty = false;
        std::unique_ptr<T[]> newArray = std::make_unique<T[]>(_size + array._size);

        for (int i = 0; i < _size; i++)
        {
            newArray[i] = _array[i];
        }

        for (int i = 0; i < array._size; i++)
        {
            newArray[i + (_size)] = array._array[i];
        }

        erase();
        _isEmpty = false;
        _size += array._size;
        _array = std::move(newArray);
        return *this;
    }

    // array concatenation
    Array<T> operator+(Array<T> &&array)
    {
        if (_isEmpty && array._isEmpty)
        {
            return *this;
        }

        _isEmpty = false;
        std::unique_ptr<T[]> newArray = std::make_unique<T[]>(_size + array._size);

        for (int i = 0; i < _size; i++)
        {
            newArray[i] = _array[i];
        }

        for (int i = 0; i < array._size; i++)
        {
            newArray[i + (_size)] = array._array[i];
        }

        erase();
        _isEmpty = false;
        _size += array._size;
        _array = std::move(newArray);
        return *this;
    }

    Array<T> &operator=(const Array<T> &array)
    {
        if (this == &array)
        {
            return *this;
        }
        if (_array != nullptr)
        {
            erase();
        }
        _array = new T[array._size];
        for (int i = 0; i < array._size; i++)
        {
            _array[i] = array._array[i];
        }

        _size = array._size;
        _isEmpty = array._isEmpty;
        return *this;
    }

    // In the case of comparing different types of arrays
    template <typename T2>
    bool operator==(const Array<T2> &array)
    {
       return false;
    }

    // comparison of similar arrays
    bool operator==(const Array<T> &array)
    {

        if (_isEmpty == array.isEmpty())
        {
            if (_size == array.count())
            {
                for (int i = 0; i < _size; i++)
                {
                    if (this->_array[i] != array[i])
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }
};