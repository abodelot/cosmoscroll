
#include <cstring>
#include <cassert>
#include <iostream>

#include "Password.hpp"

									  
const std::string Password::ALPHABET("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz?- ");

Password::Password() {
    std::memset(data, 0, PASSWORD_LENGTH_BYTES);
    encode();
}

Password::Password(const std::string &password, bool fixChecksum) {
    if (password.length() != PASSWORD_LENGTH) {
        std::cerr << "Longueur invalide\n";
    }
    decode(password, fixChecksum);
    // reencode just in case the checksum was fixed
    encode();
}

bool Password::getBit(int bit) const {
    assert((bit >= 0) && (bit < PASSWORD_BITS));
    
    int byte = bit / 8;
    return (data[byte] & (1 << (bit % 8)));
}

void Password::setBit(int bit, bool value) {
    assert((bit >= 0) && (bit < PASSWORD_BITS));
    
    int byte = bit / 8;
    
    if (value)
    {
        data[byte] |= (1 << (bit % 8));
    }
    else
    {
        data[byte] &= ~(1 << (bit % 8));
    }    
    encode();
}

void Password::setLives(unsigned char lives) {
    data[LIVES_COUNT_BYTE] = lives;    
    encode();
}

void Password::setLevel(unsigned char level) {
    data[LEVEL_COUNT_BYTE] = level;
    encode();
}

void Password::setShield(unsigned char shield) {
    data[SHIELD_COUNT_BYTE] = shield;
    encode();
}

void Password::setCoolers(unsigned char coolers) {
    data[COOLER_COUNT_BYTE] = coolers;
    encode();
}

void Password::setShift(unsigned char shift) {
    data[SHIFT_BYTE] = shift;
    encode();
}

unsigned char Password::calcChecksum() const {
    unsigned char checksum = 0;
    
    for (int i = 16; i >= 0; --i) {
        checksum += data[i];
    }
    
    return checksum;
}

void Password::decode(const std::string &password, bool fixChecksum) {
    // convert the password string to the metroid alphabet
    unsigned char metroid[PASSWORD_LENGTH];
    
    for (unsigned int i = 0; i < PASSWORD_LENGTH; ++i) {
        if (password[i] == ' ') {
            metroid[i] = SPACE_VALUE;
        } else {
            int pos = ALPHABET.find_first_of(password[i]);
            
            if (pos < 0) {
                std::cerr <<"Invalid Character in Password\n";
            }
            
            metroid[i] = pos;
        }
    }
    
    // convert metroid password to encoded password data
    unsigned char *ptr = data;
    
    for (int i = 0; i < 6; ++i) {
        unsigned char *temp = metroid + (i * 4);
        
        *ptr = (*temp << 2) | (*(temp + 1) >> 4);
        ++ptr;
        
        *ptr = (*(temp + 1) << 4) | (*(temp + 2) >> 2);
        ++ptr;
        
        *ptr = (*(temp + 2) << 6) | *(temp + 3);
        ++ptr;
    }
    
    // decode the password data
    short carry = 1, carryTemp;
    int count = data[SHIFT_BYTE];
    
    for (int i = 0; i < count; ++i) {
        unsigned char temp = data[15];
        
        for (int j = 15; j >= 0; --j) {
            carryTemp = (data[j] & 0x80) >> 7;
            data[j] <<= 1;
            data[j] |= (carry & 0x1);
            carry = carryTemp;
        }
        
        carryTemp = (temp & 0x80) >> 7;
        temp <<= 1;
        temp |= (carry & 0x1);
        carry = carryTemp;
        
        data[15] = temp;
    }
    
    // confirm the checksum and fix if requested
    unsigned char checksum = calcChecksum();
    
    if (data[CHECKSUM_BYTE] != checksum) {
        if (!fixChecksum) {
            std::cerr << "Checksum Verification Failed\n";
        }
    }
}

void Password::encode() {
    // calculate the checksum
    data[CHECKSUM_BYTE] = calcChecksum();
    
    // copy the raw password data
    unsigned char raw[PASSWORD_LENGTH_BYTES];
    std::memcpy(raw, data, PASSWORD_LENGTH_BYTES);
    
    // encode the password data
    short carry = 1, carryTemp;
    int count = raw[SHIFT_BYTE];
    
    for (int i = 0; i < count; ++i) {
        unsigned char temp = *raw;
        
        for (int j = 0; j < 16; ++j) {
            carryTemp = raw[j] & 0x1;
            raw[j] >>= 1;
            raw[j] |= (carry & 0x1) << 7;
            carry = carryTemp;
        }
        
        carryTemp = temp & 0x1;
        temp >>= 1;
        temp |= (carry & 0x1) << 7;
        carry = carryTemp;
        
        *raw = temp;
    }
    
    // convert it to the metroid alphabet
    unsigned char metroid[PASSWORD_LENGTH];
    unsigned char *ptr = metroid;
    
    for (int i = 0; i < 6; ++i) {
        unsigned char *temp = raw + (i * 3);
        
        *ptr = *temp >> 2;
        ++ptr;
        
        *ptr = ((*temp & 0x3) << 4) | (*(temp + 1) >> 4);
        ++ptr;
        
        *ptr = ((*(temp + 1) & 0xF) << 2) | (*(temp + 2) >> 6);
        ++ptr;
        
        *ptr = (*(temp + 2) & 0x3F);
        ++ptr;
    }
    
    // convert metroid password to wxString
    encoded.clear();
    
    for (unsigned int i = 0; i < PASSWORD_LENGTH; ++i) {
        encoded.push_back(ALPHABET[metroid[i]]);
    }
}

