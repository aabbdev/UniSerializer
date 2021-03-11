#include "UniSerializer.h"
static UniCallbacks callbacks = { malloc, realloc, free };
inline int is_big_endian(void)
{
	union {
		uint32_t i;
		char c[4];
	} e = { 0x01000000 };

	return e.c[0];
}
UniBuffer Uni_init(size_t InitialSize, bool autoResize, bool isBigEndian) {
	UniBuffer ctx;
	ctx.autoResize = autoResize;
	ctx.position = 0;
	ctx.length = InitialSize;
	ctx.buffer = (uint8_t*)callbacks.malloc(InitialSize * sizeof(uint8_t));
	// to be reworked
	ctx.systemBigEndian = is_big_endian();
	ctx.desiredBigEndian = (isBigEndian == false && ctx.systemBigEndian == false) || ctx.systemBigEndian;
	return ctx;
}
UniBuffer Uni_init_with_callbacks(size_t InitialSize, bool autoResize, bool isBigEndian, const UniCallbacks* inits)
{
	if (inits->malloc != NULL && inits->free != NULL)
	{
		callbacks.malloc = inits->malloc;
		callbacks.free = inits->free;
		if (inits->realloc == NULL) {
			autoResize = false;
			callbacks.realloc = NULL;
		}
		else {
			callbacks.realloc = inits->realloc;
		}
	}
	return Uni_init(InitialSize, autoResize, isBigEndian);
}
UniBuffer Uni_from(uint8_t* buffer, uint8_t Size, bool isBigEndian) {
	UniBuffer ctx;
	ctx.autoResize = false;
	ctx.position = 0;
	ctx.length = Size;
	ctx.buffer = buffer;
	ctx.desiredBigEndian = isBigEndian;
	ctx.systemBigEndian = is_big_endian();
	return ctx;
}
bool ResizeIfNeed(UniBuffer* ctx, size_t Size) {
	size_t newSize = ctx->position + Size;
	if (newSize > ctx->length) {
		if (ctx->autoResize) {
			size_t len = ctx->length;
			// TODO: To be replaced with faster, next power of 2 (log)
			while (newSize>len) {
				len *= 2;
			}
			uint8_t* tmp = (uint8_t *)realloc(ctx->buffer, sizeof(uint8_t)*len);
			if (tmp == NULL) {
				return false;
			}
			ctx->length = len;
			ctx->buffer = tmp;
			return true;
		}
		return false;
	}
	return true;
}
inline bool VerifyEntrySize(UniBuffer* ctx, size_t Size) {
	return (ctx->position + Size) > ctx->length;
}
bool Uni_encode_8(UniBuffer* ctx, uint8_t value) {
	if (ResizeIfNeed(ctx, 1) == false) {
		return false;
	}
	*(ctx->buffer + ctx->position) = value & 0xFF;
	ctx->position += 1;
	return true;
}
bool Uni_encode_16(UniBuffer* ctx, uint16_t value) {
	if (ResizeIfNeed(ctx, 2) == false) {
		return false;
	}
	if (is_big_endian()) {
		*(ctx->buffer + ctx->position) = value & 0xff;
		*(ctx->buffer + ctx->position + 1) = (value >> 8) & 0xff;
	}else {
		*(ctx->buffer + ctx->position + 1) = value & 0xff;
		*(ctx->buffer + ctx->position) = (value >> 8) & 0xff;
	}

	ctx->position += 2;

	return true;
}
bool Uni_encode_32(UniBuffer* ctx, uint32_t value) {
	if (ResizeIfNeed(ctx, 4) == false) {
		return false;
	}

	if (is_big_endian()) {
		*(ctx->buffer + ctx->position) = value & 0xff;
		*(ctx->buffer + ctx->position + 1) = (value >> 8) & 0xFF;
		*(ctx->buffer + ctx->position + 2) = (value >> 16) & 0xFF;
		*(ctx->buffer + ctx->position + 3) = (value >> 24) & 0xff;
	}else{
		*(ctx->buffer + ctx->position + 3) = value & 0xff;
		*(ctx->buffer + ctx->position + 2) = (value >> 8) & 0xFF;
		*(ctx->buffer + ctx->position + 1) = (value >> 16) & 0xFF;
		*(ctx->buffer + ctx->position) = (value >> 24) & 0xff;
	}
	ctx->position += 4;

	return true;
}
bool Uni_encode_64(UniBuffer* ctx, uint64_t value) {
	if (ResizeIfNeed(ctx, 8) == false) {
		return false;
	}
	if (is_big_endian()) {
		*(ctx->buffer + ctx->position) = value & 0xff;
		*(ctx->buffer + ctx->position + 1) = (value >> 8) & 0xFF;
		*(ctx->buffer + ctx->position + 2) = (value >> 16) & 0xFF;
		*(ctx->buffer + ctx->position + 3) = (value >> 24) & 0xFF;
		*(ctx->buffer + ctx->position + 4) = (value >> 32) & 0xFF;
		*(ctx->buffer + ctx->position + 5) = (value >> 40) & 0xFF;
		*(ctx->buffer + ctx->position + 6) = (value >> 48) & 0xFF;
		*(ctx->buffer + ctx->position + 7) = (value >> 56) & 0xff;
	}
	else {
		*(ctx->buffer + ctx->position + 7) = value & 0xff;
		*(ctx->buffer + ctx->position + 6) = (value >> 8) & 0xFF;
		*(ctx->buffer + ctx->position + 5) = (value >> 16) & 0xFF;
		*(ctx->buffer + ctx->position + 4) = (value >> 24) & 0xFF;
		*(ctx->buffer + ctx->position + 3) = (value >> 32) & 0xFF;
		*(ctx->buffer + ctx->position + 2) = (value >> 40) & 0xFF;
		*(ctx->buffer + ctx->position + 1) = (value >> 48) & 0xFF;
		*(ctx->buffer + ctx->position) = (value >> 56) & 0xff;
	}

	ctx->position += 8;

	return true;
}
bool Uni_encode_Bytes(UniBuffer* ctx, uint8_t* value, size_t length)
{
	if (ResizeIfNeed(ctx, length) == false) {
		return false;
	}
	memcpy(ctx->buffer + ctx->position, value, length);
	ctx->position += length;
	return true;
}
bool Uni_encode_UInt8(UniBuffer* ctx, uint8_t value)
{
	return Uni_encode_8(ctx, (uint8_t)value);
}
bool Uni_encode_UInt16(UniBuffer* ctx, uint16_t value)
{
	return Uni_encode_16(ctx, (uint16_t)value);
}
bool Uni_encode_UInt32(UniBuffer* ctx, uint32_t value)
{
	return Uni_encode_32(ctx, (uint32_t)value);
}
bool Uni_encode_UInt64(UniBuffer* ctx, uint64_t value)
{
	return Uni_encode_64(ctx, (uint64_t)value);
}
bool Uni_encode_Int8(UniBuffer* ctx, int8_t value)
{
	return Uni_encode_8(ctx, (uint8_t)value);
}
bool Uni_encode_Int16(UniBuffer* ctx, int16_t value)
{
	return Uni_encode_16(ctx, (uint16_t)value);
}
bool Uni_encode_Int32(UniBuffer* ctx, int32_t value)
{
	return Uni_encode_32(ctx, (uint32_t)value);
}
bool Uni_encode_Int64(UniBuffer* ctx, int64_t value)
{
	return Uni_encode_64(ctx, (uint64_t)value);
}
bool Uni_encode_Bool(UniBuffer* ctx, bool value)
{
	return Uni_encode_8(ctx, value ? 1:0);
}
bool Uni_encode_String(UniBuffer* ctx, const char* value)
{
	size_t length = strlen(value) + 1;
	return Uni_encode_Bytes(ctx, (uint8_t*)value, length);
}
uint8_t Uni_decode_8(UniBuffer* ctx, bool* error)
{
	if (VerifyEntrySize(ctx, 1)) {
		*error = true;
		return 0;
	}

	uint8_t value = *(ctx->buffer + ctx->position) & 0xff;
	ctx->position += 1;
	return value;
}
uint16_t Uni_decode_16(UniBuffer* ctx, bool* error)
{
	if (VerifyEntrySize(ctx, 2)) {
		*error = true;
		return 0;
	}

	uint16_t value;
	if (ctx->desiredBigEndian) {
		value = *(ctx->buffer + ctx->position) & 0xff;
		value |= (((uint16_t)(*(ctx->buffer + ctx->position + 1))) << 8) & 0xff;
	}
	else {
		value = *(ctx->buffer + ctx->position + 1) & 0xff;
		value |= (((uint16_t)(*(ctx->buffer + ctx->position))) << 8) & 0xff;
	}
	ctx->position += 2;
	return value;
}
uint32_t Uni_decode_32(UniBuffer* ctx, bool* error)
{
	if (VerifyEntrySize(ctx, 4)) {
		*error = true;
		return 0;
	}

	uint32_t value;
	if (ctx->desiredBigEndian) {
		value = *(ctx->buffer + ctx->position) & 0xff;
		value |= (((uint32_t)(*(ctx->buffer + ctx->position + 1))) << 8) & 0xff;
		value |= (((uint32_t)(*(ctx->buffer + ctx->position + 2))) << 16) & 0xff;
		value |= (((uint32_t)(*(ctx->buffer + ctx->position + 3))) << 24) & 0xff;
	}
	else {
		value = *(ctx->buffer + ctx->position + 3) & 0xff;
		value |= (((uint32_t)(*(ctx->buffer + ctx->position + 2))) << 8) & 0xff;
		value |= (((uint32_t)(*(ctx->buffer + ctx->position + 1))) << 16) & 0xff;
		value |= (((uint32_t)(*(ctx->buffer + ctx->position))) << 24) & 0xff;
	}
	ctx->position += 4;
	return value;
}
uint64_t Uni_decode_64(UniBuffer* ctx, bool* error)
{
	if (VerifyEntrySize(ctx, 8)) {
		*error = true;
		return 0;
	}

	uint64_t value;
	if (ctx->desiredBigEndian) {
		value = *(ctx->buffer + ctx->position) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 1))) << 8) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 2))) << 16) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 3))) << 24) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 4))) << 32) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 5))) << 40) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 6))) << 48) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 7))) << 56) & 0xff;
	}
	else {
		value = *(ctx->buffer + ctx->position + 7) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 6))) << 8) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 5))) << 16) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 4))) << 24) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 3))) << 32) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 2))) << 40) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position + 1))) << 48) & 0xff;
		value |= (((uint64_t)(*(ctx->buffer + ctx->position))) << 56) & 0xff;
	}

	ctx->position += 8;
	return value;
}
uint8_t* Uni_decode_Bytes(UniBuffer* ctx, size_t length, uint8_t* output, bool* error)
{
	if (VerifyEntrySize(ctx, length)) {
		*error = true;
		return 0;
	}
	uint8_t* tmp;
	if (output == NULL) {
		tmp = (uint8_t*)malloc(sizeof(uint8_t) * length);
		if (tmp == NULL) {
			*error = true;
			return NULL;
		}
	}
	else {
		tmp = output;
	}
	memcpy(tmp, ctx->buffer + ctx->position, length);
	ctx->position += length;
	return tmp;
}
uint8_t Uni_decode_UInt8(UniBuffer* ctx, bool* error) {
	return (uint8_t)Uni_decode_8(ctx, error);
}
uint16_t Uni_decode_UInt16(UniBuffer* ctx, bool* error) {
	return (uint16_t)Uni_decode_16(ctx, error);
}
uint32_t Uni_decode_UInt32(UniBuffer* ctx, bool* error) {
	return (uint32_t)Uni_decode_32(ctx, error);
}
uint64_t Uni_decode_UInt64(UniBuffer* ctx, bool* error) {
	return (uint64_t)Uni_decode_64(ctx, error);
}
int8_t Uni_decode_Int8(UniBuffer* ctx, bool* error) {
	return (int8_t)Uni_decode_8(ctx, error);
}
int16_t Uni_decode_Int16(UniBuffer* ctx, bool* error) {
	return (int16_t)Uni_decode_16(ctx, error);
}
int32_t Uni_decode_Int32(UniBuffer* ctx, bool* error) {
	return (int32_t)Uni_decode_32(ctx, error);
}
int64_t Uni_decode_Int64(UniBuffer* ctx, bool* error) {
	return (int64_t)Uni_decode_64(ctx, error);
}
bool Uni_decode_Bool(UniBuffer* ctx, bool* error)
{
	return ((uint8_t)Uni_decode_8(ctx, error) == 1) ? true:false;
}
char* Uni_decode_String(UniBuffer* ctx, /*char* output,*/ bool* error)
{
	bool found = true;
	size_t length;
	char* base = ctx->buffer + ctx->position;
	for (size_t i = 0; i < ctx->length; ++i) {
		if (*(base + i) == '\0') {
			found = false;
			length = i+1;
			break;
		}
	}
	if (found) {
		*error = true;
		return NULL;
	}

	ctx->position += length;
	return base;
	//size_t length = strnlen(ctx->buffer + ctx->position, ctx->length) + 1;
	//return (char*)Uni_decode_Bytes(ctx, length, (uint8_t*)output, error);
}
void Uni_reset(UniBuffer* ctx) {
	ctx->position = 0;
}
void Uni_free(UniBuffer* ctx) {
	free(ctx->buffer);
	ctx->length = 0;
	ctx->position = 0;
}