#ifndef __KEYCONSTANTS__H
#define __KEYCONSTANTS__H

#include <map>
#include <string>

class KeyPair{
public:
    std::string VKname;
    std::string name;
    KeyPair(const std::string &vk, const std::string &name);
    KeyPair() = default;
};

class Keys{
public:
    static std::map<int, KeyPair> KEYS;
};


#endif
