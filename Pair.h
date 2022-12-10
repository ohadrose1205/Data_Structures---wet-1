//
// Created by shach on 11/30/2022.
//

#ifndef MIVNEWET1_PAIR_H
#define MIVNEWET1_PAIR_H

template<class T, class K>
struct Pair{
private:
    T m_data;
    K m_key;
public:
    Pair(const T& data,const K& key):  m_data(data), m_key(key){}
    Pair(const Pair<T,K>& p):m_data(p.m_data), m_key(p.m_key){}
    Pair& operator= (const Pair& p) = default;
    ~Pair() = default;
    T& data(){
        return m_data;
    }
    const T& data() const{
        return m_data;
    }
    const K& key() const{
        return m_key;
    }
};
#endif //MIVNEWET1_PAIR_H
