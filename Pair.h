//
// Created by shach on 11/30/2022.
//

#ifndef MIVNEWET1_PAIR_H
#define MIVNEWET1_PAIR_H

template<class T, class K>
struct Pair{
private:
    T* m_data;
    const K* m_key;
public:
    Pair(): m_data(nullptr), m_key(nullptr){}
    Pair(const T& data,const K& key):  m_data(new T(data)), m_key(new K(key)){}
    Pair(const Pair<T,K>& p):m_data(new T(*p.m_data)), m_key(new K(*p.m_key)){}
    Pair& operator= (const Pair& p){
        T* newData = new T(*p.m_data);
        T* oldData = this->m_data;
        this->m_data = newData;
        delete oldData;
        return *this;
    }
    ~Pair(){
        delete m_data;
        delete m_key;
    }
    T* Data() const{
        return m_data;
    }
    const K* Key() const{
        return m_key;
    }
};
#endif //MIVNEWET1_PAIR_H
