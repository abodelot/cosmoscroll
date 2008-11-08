//  PASS: 00000000000000W20000000A  <- Fin du lvl 2

// A la base, extrait du  source du projet mpg (Metroid Password Generator), Sous licence GPL v2
/*

[ ... ]
 * Copyright (C) 2005 emuWorks
 * http://games.technoplaza.net/
 
 [...]
 
 */


#ifndef _PASSWORD_HH_
#define _PASSWORD_HH_

#include <string>



    const int PASSWORD_BITS = 128;
    const unsigned int PASSWORD_LENGTH = 24;
    const int PASSWORD_LENGTH_BYTES = 18;
    

    

    class Password {
    private:
        
		std::string encoded;
        unsigned char data[18];
        
        static const int LIVES_COUNT_BYTE = 10;
		static const int LEVEL_COUNT_BYTE = 11;
        static const int SHIFT_BYTE = 16;
        static const int CHECKSUM_BYTE = 17;
        static const unsigned char SPACE_VALUE = 0xFF;

        /**
         * Calculates the checksum value for this Password.
         *
         * @return The checksum.
         */
        unsigned char calcChecksum() const;
        
        /**
         * Decodes a  password into our raw format.
         *
         * @param password The password to decode.
         * @param fixChecksum true if the checksum should be fixed to force a
         *                    valid password; false to accept only valid
         *                    passwords.
         *
         */
        void decode(const std::string &password, bool fixChecksum);
        
        /**
         * Encodes the raw password data into a password.
         */
        void encode();

    public:
        static const std::string ALPHABET;
    
        /**
         * Creates a new blank password.
         */
        Password();
        
        /**
         * Creates a password from a  password.
         *
         * @param password The password.
         * @param fixChecksum true if the checksum should be fixed to force a
         *                    valid password; false to accept only valid
         *                    passwords.
         *
         */
        Password(const std::string &password, bool fixChecksum = false);
        
        /**
         * Gets a bit from the raw password data.
         *
         * @param bit The bit to get (1-128).
         *
         * @return true if the bit is set; false otherwise.
         */
        bool getBit(int bit) const;
        
        /**
         * Sets a bit in the raw password data.
         *
         * @param bit the bit to set (1-128).
         * @param value true to set the bit; false to clear it.
         */
        void setBit(int bit, bool value = true);
        
        /**
         * Gets the encoded password encapsulated by this Password.
         *
         * @return The password.
         */
        const std::string &getEncoded() const;
        
 
        unsigned char getLives() const;
		
		unsigned char getLevel() const;
        
        /**

         * Gets the shift byte used by this Password.
         *
         * @return The shift byte.
         */
        unsigned char getShift() const;
        
        /**
         * Sets the shift byte used by this Password.
         *
         * @param shift The shift byte.
         */
        void setShift(unsigned char shift);
		
		void setLives(unsigned char lives);
		
		void setLevel(unsigned char level);
    };
    
    inline const std::string& Password::getEncoded() const { return encoded; }
    inline unsigned char Password::getLives() const
        { return data[LIVES_COUNT_BYTE]; }
	inline unsigned char Password::getLevel() const
        { return data[LEVEL_COUNT_BYTE]; }
    inline unsigned char Password::getShift() const { return data[SHIFT_BYTE]; }


#endif

