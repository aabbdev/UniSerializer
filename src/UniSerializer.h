#ifndef __UNIVERSALSERIALIZER__
#define __UNIVERSALSERIALIZER__
#include <stdlib.h>
#include <string.h> //memset
#include <stdint.h>
#include <stdbool.h>
typedef struct _UniBuffer {
	uint8_t* buffer;
	uint32_t position;
	size_t length;
	bool autoResize;
} UniBuffer;
typedef struct _UniCallbacks
{
	void* (* malloc) (size_t size);
	void* (*realloc) (void* _Block, size_t size);
	void (* free) (void* memory);
} UniCallbacks;

UniBuffer Uni_init(size_t InitialSize, bool autoResize);
UniBuffer Uni_init_with_callbacks(size_t InitialSize, bool autoResize, const UniCallbacks* inits);
UniBuffer Uni_from(uint8_t* buffer, uint8_t Size);


// ** ################### ENCODER ###################


bool Uni_encode_8(UniBuffer* ctx, uint8_t value);
bool Uni_encode_16(UniBuffer* ctx, uint16_t value);
bool Uni_encode_32(UniBuffer* ctx, uint32_t value);
bool Uni_encode_64(UniBuffer* ctx, uint64_t value);
bool Uni_encode_Bytes(UniBuffer* ctx, uint8_t* value, size_t length);
// Middleware
bool Uni_encode_UInt8(UniBuffer* ctx, uint8_t value);
bool Uni_encode_UInt16(UniBuffer* ctx, uint16_t value);
bool Uni_encode_UInt32(UniBuffer* ctx, uint32_t value);
bool Uni_encode_UInt64(UniBuffer* ctx, uint64_t value);
bool Uni_encode_Int8(UniBuffer* ctx, int8_t value);
bool Uni_encode_Int16(UniBuffer* ctx, int16_t value);
bool Uni_encode_Int32(UniBuffer* ctx, int32_t value);
bool Uni_encode_Int64(UniBuffer* ctx, int64_t value);
bool Uni_encode_Bool(UniBuffer* ctx, bool value);
bool Uni_encode_String(UniBuffer* ctx, const char* value);


// ** ################### DECODER ###################


uint8_t Uni_decode_8(UniBuffer* ctx, bool* error);
uint16_t Uni_decode_16(UniBuffer* ctx, bool* error);
uint32_t Uni_decode_32(UniBuffer* ctx, bool* error);
uint64_t Uni_decode_64(UniBuffer* ctx, bool* error);
uint8_t* Uni_decode_Bytes(UniBuffer* ctx, size_t length, uint8_t* output, bool* error);
// Middleware
uint8_t Uni_decode_UInt8(UniBuffer* ctx, bool* error);
uint16_t Uni_decode_UInt16(UniBuffer* ctx, bool* error);
uint32_t Uni_decode_UInt32(UniBuffer* ctx, bool* error);
uint64_t Uni_decode_UInt64(UniBuffer* ctx, bool* error);
int8_t Uni_decode_Int8(UniBuffer* ctx, bool* error);
int16_t Uni_decode_Int16(UniBuffer* ctx, bool* error);
int32_t Uni_decode_Int32(UniBuffer* ctx, bool* error);
int64_t Uni_decode_Int64(UniBuffer* ctx, bool* error);
bool Uni_decode_Bool(UniBuffer* ctx, bool* error);
char* Uni_decode_String(UniBuffer* ctx, /*char* output,*/ bool* error);

void Uni_reset(UniBuffer* ctx);
void Uni_free(UniBuffer* ctx);
#endif /*__TRIM__*/
