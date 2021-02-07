import sys, ctypes

class Deserializer:
    def __init__(self, entry):
        self.buffer = None
        self.position = 0
        self.frombuffer(entry)
    def __VerifyEntrySize(self, size):
        assert(isinstance(size, int))
        return len(self.buffer) < (self.position + size)
    def decode_8(self):
        if self.__VerifyEntrySize(1):
            return None
        value = self.buffer[self.position]
        self.position += 1
        return value
    def decode_16(self):
        if self.__VerifyEntrySize(2):
            return None
        value = None
        if sys.byteorder == "big":
            value = self.buffer[self.position]
            value |= self.buffer[self.position + 1] << 8
        else:
            value = self.buffer[self.position + 1]
            value |= self.buffer[self.position] << 8
        self.position += 2
        return value
    def decode_32(self):
        if self.__VerifyEntrySize(4):
            return None
        value = None
        if sys.byteorder == "big":
            value = self.buffer[self.position]
            value |= (self.buffer[self.position + 1] << 8)
            value |= (self.buffer[self.position + 2] << 16)
            value |= (self.buffer[self.position + 3] << 24)
        else:
            value = self.buffer[self.position + 3]
            value |= (self.buffer[self.position + 2] << 8)
            value |= (self.buffer[self.position + 1] << 16)
            value |= (self.buffer[self.position] << 24)
        self.position += 4
        return value
    def decode_48(self):
        if self.__VerifyEntrySize(6):
            return None
        value = None
        if sys.byteorder == "big":
            value = self.buffer[self.position]
            value |= (self.buffer[self.position + 1] << 8)
            value |= (self.buffer[self.position + 2] << 16)
            value |= (self.buffer[self.position + 3] << 24)
            value |= (self.buffer[self.position + 4] << 32)
            value |= (self.buffer[self.position + 5] << 40)
        else:
            value = self.buffer[self.position + 5]
            value |= (self.buffer[self.position + 4] << 8)
            value |= (self.buffer[self.position + 3] << 16)
            value |= (self.buffer[self.position + 2] << 24)
            value |= (self.buffer[self.position + 1] << 32)
            value |= (self.buffer[self.position] << 40)
            
        self.position += 6
        return value
    def decode_64(self):
        if self.__VerifyEntrySize(8):
            return None
        value = None
        if sys.byteorder == "big":
            value = self.buffer[self.position]
            value |= (self.buffer[self.position + 1] << 8)
            value |= (self.buffer[self.position + 2] << 16)
            value |= (self.buffer[self.position + 3] << 24)
            value |= (self.buffer[self.position + 4] << 32)
            value |= (self.buffer[self.position + 5] << 40)
            value |= (self.buffer[self.position + 6] << 48)
            value |= (self.buffer[self.position + 7] << 56)
        else:
            value = self.buffer[self.position + 7]
            value |= (self.buffer[self.position + 6] << 8)
            value |= (self.buffer[self.position + 5] << 16)
            value |= (self.buffer[self.position + 4] << 24)
            value |= (self.buffer[self.position + 3] << 32)
            value |= (self.buffer[self.position + 2] << 40)
            value |= (self.buffer[self.position + 1] << 48)
            value |= (self.buffer[self.position] << 56)
        self.position += 8
        return value
    def decode_Int8(self):
        val = self.decode_8()
        if val == None:
            return None
        return ctypes.c_int8(val).value
    def decode_Int16(self):
        val = self.decode_16()
        if val == None:
            return None
        return ctypes.c_int16(val).value
    def decode_Int32(self):
        val = self.decode_32()
        if val == None:
            return None
        return ctypes.c_int32(val).value
    def decode_Int64(self):
        val = self.decode_64()
        if val == None:
            return None
        return ctypes.c_int64(val).value
    def decode_UInt8(self):
        val = self.decode_8()
        if val == None:
            return None
        return ctypes.c_uint8(val).value
    def decode_UInt16(self):
        val = self.decode_16()
        if val == None:
            return None
        return ctypes.c_uint16(val).value
    def decode_UInt32(self):
        val = self.decode_32()
        if val == None:
            return None
        return ctypes.c_uint32(val).value
    def decode_UInt64(self):
        val = self.decode_64()
        if val == None:
            return None
        return ctypes.c_uint64(val).value
    def decode_String(self):
        found = True
        length = 0
        for i in range(self.position, len(self.buffer)):
            length += 1
            if self.buffer[i] == 0:
                found = False
                break
        if found:
            return None
        value = ''.join(list(map(chr, self.buffer[self.position:self.position + length - 1])))
        self.position += length
        return value
    def decode_Bool(self):
        val = self.decode_8()
        if val == None:
            return None
        return ctypes.c_bool(val).value
    def decode_Bytes(self, length):
        if self.__VerifyEntrySize(length):
            return None
        buffer = memoryview(self.buffer[self.position:self.position + length])
        self.position += length
        return buffer
    def frombuffer(self, entry):
        assert(isinstance(entry, bytearray)
                or isinstance(entry, bytes)
                or isinstance(entry, memoryview)
                or isinstance(entry, Deserializer))
                #or isinstance(entry, Serializer))
        if isinstance(entry, type(self)):
            self.buffer = entry.buffer
            self.position = entry.position
        #elif isinstance(entry, Serializer):
        #    self.buffer = entry.tobuffer()
        #    self.reset()
        else:
            self.buffer = memoryview(entry)
            self.reset()
    def reset(self):
        self.position = 0
