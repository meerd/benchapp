////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WjCryptLib_RC4
//
//  An implementation of RC4 stream cipher
//
//  This is free and unencumbered software released into the public domain - June 2013 waterjuice.org
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  IMPORTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  TYPES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Rc4Context - This must be initialised using Rc4Initialised. Do not modify the contents of this structure directly.
typedef struct
{
    uint32_t     i;
    uint32_t     j;
    uint8_t      S[256];
} Rc4Context;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Rc4Initialise
//
//  Initialises an RC4 cipher and discards the specified number of first bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Rc4Initialise
    (
        Rc4Context*     Context,        // [out]
        void const*     Key,            // [in]
        uint32_t        KeySize,        // [in]
        uint32_t        DropN           // [in]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Rc4Output
//
//  Outputs the requested number of bytes from the RC4 stream
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Rc4Output
    (
        Rc4Context*     Context,        // [in out]
        void*           Buffer,         // [out]
        uint32_t        Size            // [in]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Rc4Xor
//
//  XORs the RC4 stream with an input buffer and puts the results in an output buffer. This is used for encrypting
//  and decrypting data. InBuffer and OutBuffer can point to the same location for inplace encrypting/decrypting
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Rc4Xor
    (
        Rc4Context*     Context,        // [in out]
        void const*     InBuffer,       // [in]
        void*           OutBuffer,      // [out]
        uint32_t        Size            // [in]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Rc4XorWithKey
//
//  This function combines Rc4Initialise and Rc4Xor. This is suitable when encrypting/decrypting data in one go with a
//  key that is not going to be reused.
//  InBuffer and OutBuffer can point to the same location for inplace encrypting/decrypting
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Rc4XorWithKey
    (
        uint8_t const*      Key,                    // [in]
        uint32_t            KeySize,                // [in]
        uint32_t            DropN,                  // [in]
        void const*         InBuffer,               // [in]
        void*               OutBuffer,              // [out]
        uint32_t            BufferSize              // [in]
    );
