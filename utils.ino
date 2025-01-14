// 16 bit data access (little endian / big endian)

// This code is meant to run on the ftDuino/AVR and thus is sure to 
// run on a little endian
#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error "Unexpected endianess"
#endif

void set16BitLE(unsigned char *pData, UINT16 value) {
    *(UINT16*)pData = value;
}

UINT16 get16BitLE(unsigned char *pData) {
    return *(UINT16*)pData;
}

void set16BitBE(unsigned char *pData, UINT16 value) {
    *(pData+0) = (value >> 8) & 0xFF;
    *(pData+1) = value & 0xFF;
}

UINT16 get16BitBE(unsigned char *pData) {
    UINT16 value = 0;
    value  = (*(pData+0) << 8) & 0xFF00;
    value += *(pData+1);
    return value;
}

// 32 bit data access (little endian / big endian)

void set32BitLE(unsigned char *pData, UINT32 value) {
    *(UINT32*)pData = value;     
}

UINT32 get32BitLE(unsigned char *pData) {
    return *(UINT32*)pData;
}

void set32BitBE(unsigned char *pData, UINT32 value) {
    *(pData+0) = (value >> 24) & 0xFF;
    *(pData+1) = (value >> 16) & 0xFF;
    *(pData+2) = (value >> 8) & 0xFF;
    *(pData+3) = value & 0xFF;
}

// Checksum calculation

unsigned short calcCheckSum(unsigned char Message[], int nBytes) {
    unsigned short  sum = 0;
    
    while(nBytes-- > 0) {
      sum += *(Message++);
    }
    sum = (~sum);
    sum++;

    return sum;
}
