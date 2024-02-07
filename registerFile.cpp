#include "registerFile.hpp"
#include "utilBytes.hpp"
#include <stdint.h>

int8_t RegisterFile::readRegister(int8_t registerIndex) {
    return *(registerMap[registerIndex]);
}

void RegisterFile::writeRegister(int8_t registerIndex, int8_t data) {
    *(registerMap[registerIndex]) = data;
}

void RegisterFile::incrementPC(int8_t offset = 1) {
    PC += offset;
}

int16_t RegisterFile::readAF() {
    return mergeBytes(A, F);
}

int16_t RegisterFile::readBC() {
    return mergeBytes(B, C);
}

int16_t RegisterFile::readDE() {
    return mergeBytes(D, E);
}

int16_t RegisterFile::readHL() {
    return mergeBytes(H, L);
}

uint16_t RegisterFile::readSP() {
    return SP;
}

uint16_t RegisterFile::readPC() {
    return PC;
}

void RegisterFile::writeAF(int16_t data) {
    A = (data >> 8);
    F = data;
}

void RegisterFile::writeBC(int16_t data) {
    B = (data >> 8);
    C = data;
}

void RegisterFile::writeDE(int16_t data) {
    D = (data >> 8);
    E = data;
}

void RegisterFile::writeHL(int16_t data) {
    H = (data >> 8);
    L = data;
}

void RegisterFile::writeSP(uint16_t data) {
    SP = data;
}
void RegisterFile::writePC(uint16_t data) {
    PC = data;
}

void RegisterFile::incrementHL(int8_t offset) {
    int16_t HL = readHL();
    HL += offset;
    writeHL(HL);
}

void RegisterFile::incrementSP(int16_t offset) {
    SP = (int16_t) SP + offset;
}

void RegisterFile::setFlagZ(bool value) {
    int8_t mask;
    mask = value ? 1 << 7 : 0;
    F |= mask;
}

void RegisterFile::setFlagN(bool value) {
    int8_t mask;
    mask = value ? 1 << 6 : 0;
    F |= mask;
}   

void RegisterFile::setFlagH(bool value) {
    int8_t mask;
    mask = value ? 1 << 5 : 0;
    F |= mask;
}   

void RegisterFile::setFlagC(bool value) {
    int8_t mask;
    mask = value ? 1 << 4 : 0;
    F |= mask;
}    

int8_t RegisterFile::readFlagC() {
    return (F & (1 << 4)) ? 1 : 0;
}

int8_t RegisterFile::readFlagN() {
    return (F & (1 << 6)) ? 1 : 0;
}

int8_t RegisterFile::readFlagH() {
    return (F & (1 << 5)) ? 1 : 0;
}

int8_t RegisterFile::readFlagZ() {
    return (F & (1 << 7)) ? 1 : 0;
}