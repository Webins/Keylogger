#include "../headers/Base64.hpp"

const std::string BASE_64_CODE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const std::string &SALT1 = "LM::TB::BB";
const std::string &SALT2 = "__:/__77";
const std::string &SALT3 = "line=wowC++";

std::string Base64::EncryptB64(std::string s){

        s = SALT1 + s + SALT2 + SALT3;
        s = Base64::base64_enconde(s);
        s.insert (7, SALT3);
        s += SALT1;
        s = base64_enconde(s);
        s = SALT2 + SALT3 + s + SALT1;
        s = Base64::base64_enconde(s);
        s.insert (1, "L");
        s.insert (7, "M");
        return s;
}

std::string Base64::base64_enconde(const std::string &s){
        std::string ret; // the output string
        int val = 0; // index to map input to our table
        int bits = -6; // to represent number of bits to a subgroup
        const unsigned int B63 = 0x3F;
        for(const auto &c: s){
            val = (val << 8) + c;  // similar to: val = val * 2^8;
            bits += 8;
            while(bits >= 0){ // in the resulting string, it basically inserts the mapped number
                ret.push_back(BASE_64_CODE[(val >> bits) & B63]);
                bits-=6;
            }
        }
        if(bits > -6)  // so at least 1 character was inserted
            ret.push_back(BASE_64_CODE[((val << 8) >> (bits+8) &B63)]);

        while(ret.size() % 4) // this is evaluated as a true (it's not 0) or false (it's 0)
            ret.push_back('=');

        return ret;
     }
