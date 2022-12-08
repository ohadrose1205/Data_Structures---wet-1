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

    Pair(const T& data,const K& key):  m_data(nullptr), m_key(nullptr){
        m_data = new T(data);
        try{
            m_key = new K(key);
        }catch(std::bad_alloc& a){
            delete m_data;
            throw std::bad_alloc();
        }
    }
    Pair(const Pair<T,K>& p):m_data(nullptr), m_key(nullptr){
        if(!p.empty()) {
            m_key = new K(p.key());
            try{
                m_data = new T(p.data());
            }catch(std::bad_alloc& a){
                delete m_key;
                throw std::bad_alloc();
            }
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
        newKey = new K(p.key());
        try{
            newData = new T(p.data());
        }catch(std::bad_alloc& a){
            delete newKey;
            throw std::bad_alloc();
        }
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
        m_data = nullptr;
        m_key = nullptr;
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
