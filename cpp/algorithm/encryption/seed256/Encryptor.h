#include "SEED_256_KISA.h"

class Encryptor
{
    public:
        Encryptor();

    public:
        bool encode( BYTE* data );
        bool decode( BYTE* data );

    private:
        static DWORD roundKey_[48];
        static BYTE secretKey_[32];
}; // end - class Encryptor
