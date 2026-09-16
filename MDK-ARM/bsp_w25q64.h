#ifndef __BSP_W25Q64_H
#define __BSP_W25Q64_H

#include <stdint.h>

uint8_t W25Q64_ReadJEDEC_ID(uint8_t *manufacturer,
                            uint8_t *memory_type,
                            uint8_t *capacity);

uint8_t W25Q64_WriteEnable(void);

uint8_t W25Q64_ReadStatus1(uint8_t *status_reg);

uint8_t W25Q64_WaitBusy(void);

uint8_t W25Q64_SectorErase(uint32_t address);

uint8_t W25Q64_ReadData(uint32_t address,
                        uint8_t *data,
                        uint16_t length);

uint8_t W25Q64_PageProgram(uint32_t address,
                           uint8_t *data,
                           uint16_t length);

#endif