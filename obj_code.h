#ifndef OBJ_CODE_H
#define OBJ_CODE_H

#include <cstdint>
#include <string>
#include <stdexcept>

/*
    ObjectCode

    This class mainly just works as a handler for editing the object code, no logic pass1/2 - wise
    here, just provides the tools to modify the bits as needed with helper functions

    Upon object creation:
    Format is already known because the correct subclass is being created directly
*/

class ObjectCode {
public:
    virtual ~ObjectCode() = default;

    // get raw binary value of object code
    uint32_t getRawValue() const;

    // get num of bits
    int getBitCount() const;

    // get hex value of object code
    uint32_t getHexValue() const;

protected:
    explicit ObjectCode(int bitCount);

    // set all bits to 0
    void clear();

    // edit one bit (index 0)
    void setBit(int logicalIndex, bool value);

    // get one bit (index 0)
    bool getBit(int logicalIndex) const;

    // edit a field of bits at once
    void setField(int logicalStart, int width, uint32_t value);

    // get field of bits at once
    uint32_t getField(int logicalStart, int width) const;

    // hex helper (validates hex can fit in nibbles)
    uint32_t normalizeHexValue(uint32_t value, int hexDigits) const;

    // lets a derived class overwrite the object-code length if needed
    void setBitCount(int bitCount);

private:
    uint32_t data_;
    int bitCount_;

private:
    int logicalToShift(int logicalIndex) const;
    uint32_t widthMask() const;

    //make sure that the values make sense before converting, simple error checks
    void validateBitIndex(int logicalIndex) const;
    void validateFieldRange(int logicalStart, int width) const;
    void validateValueFits(int width, uint32_t value) const;
};


// FORMAT 1
// 8 bits (opcode only)
class Format1Code : public ObjectCode {
public:
    Format1Code();

    void setOpcode(uint32_t opcodeHex);
    uint32_t getOpcode() const;
};


// FORMAT 2
// 16 bits (opcode + r1 + r2)
class Format2Code : public ObjectCode {
public:
    Format2Code();

    void setOpcode(uint32_t opcodeHex);
    void setR1(uint32_t r1Hex);
    void setR2(uint32_t r2Hex);

    uint32_t getOpcode() const;
    uint32_t getR1() const;
    uint32_t getR2() const;
};


// FORMAT 3
// 24 bits (opcode + nixbpe + disp)
class Format3Code : public ObjectCode {
public:
    Format3Code();

    void setOpcode(uint32_t opcodeHex);

    void setN(bool value);
    void setI(bool value);
    void setX(bool value);
    void setB(bool value);
    void setP(bool value);
    virtual void setE(bool value);   // 0 when format 3

    virtual void setAddressField(uint32_t addressHex);

    uint32_t getOpcode() const;
    bool getN() const;
    bool getI() const;
    bool getX() const;
    bool getB() const;
    bool getP() const;
    bool getE() const;
    virtual uint32_t getAddressField() const;

protected:
    // changes depending on 3/4
    virtual int getAddressFieldWidth() const;
};


// FORMAT 4
// 32 bits (opcode + nixbpe + addr)
// reuses a lot of format 3 elements for simplicity sake
class Format4Code : public Format3Code {
public:
    Format4Code();

    void setE(bool value) override;   // always 1 for format 4

    void setAddressField(uint32_t addressHex) override;

    uint32_t getAddressField() const override;

protected:
    int getAddressFieldWidth() const override;
};

#endif