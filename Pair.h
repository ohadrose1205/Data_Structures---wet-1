//
// Created by shach on 11/30/2022.
//

#ifndef MIVNEWET1_PAIR_H
#define MIVNEWET1_PAIR_H

template<class T, class K>
struct Pair{
private:
    T* m_data;
    K* m_key;
public:
    Pair(): m_data(nullptr), m_key(nullptr){}
    Pair(const T& data,const K& key):  m_data(new T(data)), m_key(new K(key)){}
    Pair(const Pair<T,K>& p):m_data(nullptr), m_key(nullptr){
        if(!p.empty()) {
            m_key = new K(*p.m_key);
            m_data = new T(*p.m_data);
        }
    }
    Pair& operator= (const Pair& p){
        if(this == &p){
            return *this;
        }
        K *oldKey, *newKey;
        T *oldData, *newData;
        if(p.empty()){
            delete m_data;
            delete m_key;
            m_data = nullptr;
            m_key = nullptr;
            return *this;
        }
        newKey = new K(*p.m_key);
        newData = new T(*p.m_data);
        oldKey = this->m_key;
        oldData = this->m_data;
        this->m_data = newData;
        this->m_key = newKey;
        delete oldData;
        delete oldKey;
        return *this;
    }
    bool empty() const{
        return (!m_data || !m_key);
    }
    ~Pair(){
        delete m_data;
        delete m_key;
    }
    T& data(){
        return *m_data;
    }
    const T& data() const{
        return *m_data;
    }
    const K& key() const{
        return *m_key;
    }
};
#endif //MIVNEWET1_PAIR_H
