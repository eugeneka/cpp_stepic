/*
    https://stepik.org/lesson/563/step/6?unit=886

    ArrayTemplate.cpp : Simple Array Template Sample

    Реализуйте шаблонную версию класса Array. Список всех операций, которые должен поддерживать класс Array, приведен в шаблоне кода.
*/

#include <cstddef>
#include <iostream>

template <typename T>
class Array
{
public:
    // Список операций:

    explicit Array(size_t size = 0, const T& value = T()) 
        : size_(size)
        , data_(new T[size]) {
        //	 конструктор класса, который создает
        //   Array размера size, заполненный значениями
        //   value типа T. Считайте что у типа T есть
        //   конструктор, который можно вызвать без
        //   без параметров, либо он ему не нужен.
        //
        for (int i(0); i != size; ++i)
            data_[i] = value;
    };

    Array(const Array& copy)
        : size_(copy.size_)
        , data_(new T[copy.size_]) {
        //   конструктор копирования, который создает
        //   копию параметра. Считайте, что для типа
        //   T определен оператор присваивания.
        //
        for (int i(0); i != size_; ++i)
            data_[i] = copy[i];
    };

    ~Array() {
        //   деструктор, если он вам необходим.
        //
        delete[] data_;
    }

    Array<T>& operator=(const Array& right) {
        //   оператор присваивания.
        //
        if (this != &right)
        {      
            delete[] data_;
            size_ = right.size_;
            data_ = new T[size_];

            for (int i(0); i != size_; ++i)
                data_[i] = right[i];
        }

         return *this;
    };

    size_t size() const {
        //   возвращает размер массива (количество
        //                              элементов).
        //
        return size_;
    };

    T& operator[](size_t i) {
        return data_[i];
    };

    const T& operator[](size_t i) const {
        return data_[i];
    };
        //   две версии оператора доступа по индексу.

private:
    size_t size_;
    T* data_;
};

int main() {
    Array <int> ai(10, 1);
    Array <double> ad(20, 1.11);

    for (int i(0); i != ai.size(); ++i)
        std::cout << ai[i] << " ";
    std::cout << std::endl;

    for (int i(0); i != ad.size(); ++i)
        std::cout << ad[i] << " ";
    std::cout << std::endl;

    Array <int> bi(ai);
    Array <double> bd;

    bi[5] = 5;
    bd = ad;

    for (int i(0); i != bi.size(); ++i)
        std::cout << bi[i] << " ";
    std::cout << std::endl;

    for (int i(0); i != bd.size(); ++i)
        std::cout << bd[i] << " ";
    std::cout << std::endl;

    return 0;
}
