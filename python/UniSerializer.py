import sys, ctypes
class UniSerializer:
    def __init__(self, buffer):
        self.frombuffer(buffer)
    def __VerifyEntrySize(self, size):
        assert(type(size) == int)
        return len(self.buffer) < (self.position + size)
    def encode_8(self, value):
        assert(type(value) == int)
        assert(value < 0x100)

        val = value & 0xff
        if self.__VerifyEntrySize(1):
            self.buffer.append(val)
        else:
            self.buffer[self.position] = val
        self.position += 1
    def encode_16(self, value):
        assert(type(value) == int)
        assert(value < 0x10000)

        val = [0,0]
        if sys.byteorder == "big":
            val[0] = value & 0xff
            val[1] = (value >> 8) & 0xff
        else:
            val[1] = value & 0xff
            val[0] = (value >> 8) & 0xff
        if self.__VerifyEntrySize(2):
            self.buffer.append(val[0])
            self.buffer.append(val[1])
        else:
            self.buffer[self.position] = val[0]
            self.buffer[self.position + 1] = val[1]
        self.position += 2
    def encode_32(self, value):
        assert(type(value) == int)
        assert(value < 0x100000000)

        val = [0,0,0,0]
        if sys.byteorder == "big":
            val[0] = value & 0xff
            val[1] = (value >> 8) & 0xFF
            val[2] = (value >> 16) & 0xFF
            val[3] = (value >> 24) & 0xFF
        else:
            val[3] = value & 0xff
            val[2] = (value >> 8) & 0xFF
            val[1] = (value >> 16) & 0xFF
            val[0] = (value >> 24) & 0xFF
        if self.__VerifyEntrySize(4):
            self.buffer.append(val[0])
            self.buffer.append(val[1])
            self.buffer.append(val[2])
            self.buffer.append(val[3])
        else:
            self.buffer[self.position] = val[0]
            self.buffer[self.position + 1] = val[1]
            self.buffer[self.position + 2] = val[2]
            self.buffer[self.position + 3] = val[3]
        self.position += 4
    def encode_64(self, value):
        assert(type(value) == int)
        assert(value < 0x10000000000000000)

        val = [0,0,0,0,0,0,0,0]
        if sys.byteorder == "big":
            val[0] = value & 0xff
            val[1] = (value >> 8) & 0xFF
            val[2] = (value >> 16) & 0xFF
            val[3] = (value >> 24) & 0xFF
            val[4] = (value >> 32) & 0xFF
            val[5] = (value >> 40) & 0xFF
            val[6] = (value >> 48) & 0xFF
            val[7] = (value >> 56) & 0xFF
        else:
            val[7] = value & 0xff
            val[6] = (value >> 8) & 0xFF
            val[5] = (value >> 16) & 0xFF
            val[4] = (value >> 24) & 0xFF
            val[3] = (value >> 32) & 0xFF
            val[2] = (value >> 40) & 0xFF
            val[1] = (value >> 48) & 0xFF
            val[0] = (value >> 56) & 0xFF
    
        if self.__VerifyEntrySize(8):
            self.buffer.append(val[0])
            self.buffer.append(val[1])
            self.buffer.append(val[2])
            self.buffer.append(val[3])
            self.buffer.append(val[4])
            self.buffer.append(val[5])
            self.buffer.append(val[6])
            self.buffer.append(val[7])
        else:
            self.buffer[self.position] = val[0]
            self.buffer[self.position + 1] = val[1]
            self.buffer[self.position + 2] = val[2]
            self.buffer[self.position + 3] = val[3]
            self.buffer[self.position + 4] = val[4]
            self.buffer[self.position + 5] = val[5]
            self.buffer[self.position + 6] = val[6]
            self.buffer[self.position + 7] = val[7]
        self.position += 8
    def encode_String(self, value):
        assert(type(value) == str)
        
        length = len(value)+1
        val = list(map(ord, value))
        val.append(0)
        if self.__VerifyEntrySize(length):
            self.buffer.extend(list(range((self.position + length)-len(self.buffer))))
        for i in range(length):
            self.buffer[self.position+i] = val[i]
        self.position += length
    def encode_Bytes(self, value):
        assert(type(value) == bytearray or type(value) == bytes)
        val = memoryview(value)
        length = len(val)
        if self.__VerifyEntrySize(length):
            self.buffer.extend(list(range((self.position + length)-len(self.buffer))))
        for i in range(length):
            self.buffer[self.position+i] = val[i] & 0xff
        self.position += length
    def encode_Bool(self, value):
        assert(type(value) == bool)
        self.encode_8(int(bool(value)))
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
        found_position = 0
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
    def frombuffer(self, buffer):
        assert(type(buffer) == bytearray or type(buffer) == list or type(buffer) == bytes)
        self.buffer = list(buffer)
        self.reset()
    def reset(self):
        self.position = 0
