/*
 * conficnvm.h
 *
 *  Created on: 28.10.2015
 *      Author: User
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PE_Types.h"

#ifndef SOURCES_CONFICNVM_H_
#define SOURCES_CONFICNVM_H_

#define NVMC_FLASH_START_ADDR    0x1FC00 /* NVRM_Config, start address of configuration data in flash */
#define NVMC_FLASH_ERASED_UINT8  0xFF
#define NVMC_FLASH_ERASED_UINT16 0xFFFF

#define NVMC_REFLECTANCE_DATA_START_ADDR  (NVMC_FLASH_START_ADDR)
#define NVMC_REFLECTANCE_DATA_SIZE        (8*2*2) /* maximum of 8 sensors (min and max) values with 16 bits */
#define NVMC_REFLECTANCE_END_ADDR         (NVMC_REFLECTANCE_DATA_START_ADDR+NVMC_REFLECTANCE_DATA_SIZE)

#define NVMC_SUMO_DATA_START_ADDR         (NVMC_REFLECTANCE_END_ADDR)
#define NVMC_SUMO_DATA_SIZE               (4) /* 4 bytes of data */
#define NVMC_SUMO_END_ADDR                (NVMC_SUMO_DATA_START_ADDR+NVMC_SUMO_DATA_SIZE)

uint8_t NVMC_SaveReflectanceData(void *data, uint16_t dataSize);
void *NVMC_GetReflectanceData(void);

uint8_t NVMC_SaveSumoData(void *data, uint16_t dataSize);
void *NVMC_GetSumoData(void);



#endif /* SOURCES_CONFICNVM_H_ */
