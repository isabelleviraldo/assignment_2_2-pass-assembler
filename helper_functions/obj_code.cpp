/*
* Purpose: obj_code.cpp source file
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
* 
* Description: helps with pass 2 of our program. Allows us to get nixpbe bits and formulate addresses for each
*/

#include "obj_code.h"


// ObjectCode Class Section
ObjectCode::ObjectCode(int bitCount)
    : data_(0), bitCount_(bitCount) {
}

// get raw binary value of object code
uint32_t ObjectCode::getRawValue() const {
    return data_ & widthMask();
}

// get num of bits
int ObjectCode::getBitCount() const {
    return bitCount_;
}

// get hex value of object code
uint32_t ObjectCode::getHexValue() const
{
    return data_ & widthMask();
}

// set all bits to 0
void ObjectCode::clear() {
    data_ = 0;
}

// edit one bit (index 0)
void ObjectCode::setBit(int logicalIndex, bool value) {
    validateBitIndex(logicalIndex);

    int shift = logicalToShift(logicalIndex);
    uint32_t mask = (1u << shift);

    if (value) {
        data_ |= mask;
    } else {
        data_ &= ~mask;
    }

    data_ &= widthMask();
}

// get one bit (index 0)
bool ObjectCode::getBit(int logicalIndex) const {
    validateBitIndex(logicalIndex);
    int shift = logicalToShift(logicalIndex);
    return ((data_ >> shift) & 1u) != 0;
}

// edit a field of bits at once
void ObjectCode::setField(int logicalStart, int width, uint32_t value) {
    validateFieldRange(logicalStart, width);
    validateValueFits(width, value);

    // Convert logical field position into shift count for the least-significant
    // bit of the inserted field.
    int shift = bitCount_ - (logicalStart + width);

    uint32_t mask;
    if (width == 32) {
        mask = 0xFFFFFFFFu;
    } else {
        mask = (1u << width) - 1u;
    }

    // Clear field region.
    data_ &= ~(mask << shift);

    // Insert new field bits.
    data_ |= ((value & mask) << shift);

    // Remove any bits outside instruction width.
    data_ &= widthMask();
}

// get field of bits at once
uint32_t ObjectCode::getField(int logicalStart, int width) const {
    validateFieldRange(logicalStart, width);

    int shift = bitCount_ - (logicalStart + width);

    uint32_t mask;
    if (width == 32) {
        mask = 0xFFFFFFFFu;
    } else {
        mask = (1u << width) - 1u;
    }

    return (data_ >> shift) & mask;
}

// hex helper (validates hex can fit in nibbles)
uint32_t ObjectCode::normalizeHexValue(uint32_t value, int hexDigits) const {
    int width = hexDigits * 4;
    validateValueFits(width, value);
    return value;
}

// lets a derived class overwrite the object-code length if needed
void ObjectCode::setBitCount(int bitCount) {
    if (bitCount != 8 && bitCount != 16 && bitCount != 24 && bitCount != 32) {
        throw std::invalid_argument("invalid object-code bit count");
    }

    bitCount_ = bitCount;

    data_ &= widthMask();
}

// Converts a logical bit index to its corresponding shift position
int ObjectCode::logicalToShift(int logicalIndex) const {
    validateBitIndex(logicalIndex);
    return bitCount_ - 1 - logicalIndex;
}

// Generates a bitmask covering the active bit width of the object code
uint32_t ObjectCode::widthMask() const {
    if (bitCount_ == 32) {
        return 0xFFFFFFFFu;
    }
    return (1u << bitCount_) - 1u;
}

//make sure that the values make sense before converting, simple error checks
void ObjectCode::validateBitIndex(int logicalIndex) const {
    if (logicalIndex < 0 || logicalIndex >= bitCount_) {
        throw std::out_of_range("bit index out of range");
    }
}

void ObjectCode::validateFieldRange(int logicalStart, int width) const {
    if (width <= 0 || width > bitCount_) {
        throw std::invalid_argument("invalid field width");
    }

    if (logicalStart < 0 || logicalStart >= bitCount_) {
        throw std::out_of_range("field start out of range");
    }

    if (logicalStart + width > bitCount_) {
        throw std::out_of_range("field exceeds instruction width");
    }
}

void ObjectCode::validateValueFits(int width, uint32_t value) const {
    if (width == 32) {
        return;
    }

    uint32_t maxValue = (1u << width) - 1u;
    if (value > maxValue) {
        throw std::out_of_range("value does not fit in field width");
    }
}


// Format 1
Format1Code::Format1Code()
    : ObjectCode(8) {
}

void Format1Code::setOpcode(uint32_t opcodeHex) {
    // Format 1 stores a full 8-bit opcode.
    opcodeHex = normalizeHexValue(opcodeHex, 2);
    setField(0, 8, opcodeHex);
}

uint32_t Format1Code::getOpcode() const {
    return getField(0, 8);
}


// Format 2
Format2Code::Format2Code()
    : ObjectCode(16) {
}

void Format2Code::setOpcode(uint32_t opcodeHex) {
    // Full first byte is opcode.
    opcodeHex = normalizeHexValue(opcodeHex, 2);
    setField(0, 8, opcodeHex);
}

void Format2Code::setR1(uint32_t r1Hex) {
    // Register field is exactly one nibble = 4 bits.
    r1Hex = normalizeHexValue(r1Hex, 1);
    setField(8, 4, r1Hex);
}

void Format2Code::setR2(uint32_t r2Hex) {
    r2Hex = normalizeHexValue(r2Hex, 1);
    setField(12, 4, r2Hex);
}

uint32_t Format2Code::getOpcode() const {
    return getField(0, 8);
}

uint32_t Format2Code::getR1() const {
    return getField(8, 4);
}

uint32_t Format2Code::getR2() const {
    return getField(12, 4);
}


// Format 3
Format3Code::Format3Code()
    : ObjectCode(24) {
    // format 3 normally has e = 0
    setE(false);
}

void Format3Code::setOpcode(uint32_t opcodeHex) {
    // Opcode is supplied as hex, e.g. 0x48.
    // For SIC/XE format 3/4, only the top 6 bits belong to opcode.
    // The low 2 bits of the first byte are used for n and i.
    opcodeHex = normalizeHexValue(opcodeHex, 2);

    // Keep only the upper 6 bits by shifting away the low 2 bits.
    setField(0, 6, opcodeHex >> 2);
}

void Format3Code::setN(bool value) { setBit(6, value); }
void Format3Code::setI(bool value) { setBit(7, value); }
void Format3Code::setX(bool value) { setBit(8, value); }
void Format3Code::setB(bool value) { setBit(9, value); }
void Format3Code::setP(bool value) { setBit(10, value); }
void Format3Code::setE(bool value) { setBit(11, value); }

void Format3Code::setAddressField(uint32_t addressHex) {
    // Format 3 trailing field is 12 bits = 3 hex digits.
    // Format 4 reuses this function but overrides the width helper to 20 bits.
    int fieldWidth = getAddressFieldWidth();
    int hexDigits = fieldWidth / 4;

    addressHex = normalizeHexValue(addressHex, hexDigits);
    setField(12, fieldWidth, addressHex);
}

uint32_t Format3Code::getOpcode() const { return getField(0, 6); }
bool Format3Code::getN() const { return getBit(6); }
bool Format3Code::getI() const { return getBit(7); }
bool Format3Code::getX() const { return getBit(8); }
bool Format3Code::getB() const { return getBit(9); }
bool Format3Code::getP() const { return getBit(10); }
bool Format3Code::getE() const { return getBit(11); }
uint32_t Format3Code::getAddressField() const { return getField(12, getAddressFieldWidth()); }

int Format3Code::getAddressFieldWidth() const {
    return 12;
}


// Format 4
Format4Code::Format4Code()
    : Format3Code() {
    // reuse format 3 setup
    setBitCount(32);
    
    // e = 1
    Format3Code::setE(true);
}

void Format4Code::setE(bool value) {
    // always 1 for format 4
    (void)value;
    Format3Code::setE(true);
}

void Format4Code::setAddressField(uint32_t addressHex) {
    // reuse format 3
    Format3Code::setAddressField(addressHex);
}

uint32_t Format4Code::getAddressField() const {
    return Format3Code::getAddressField();
}

int Format4Code::getAddressFieldWidth() const {
    return 20;
}