
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PACKET_DATA_LENGTH (50)
#define CRC_POLYNOMIAL 0x1021



typedef struct data_packet_t {
    uint8_t id;
    uint8_t data_length;
    uint8_t data[MAX_PACKET_DATA_LENGTH];
    uint16_t crc;
} data_packet_t;


//Count the number of data indexes filled with 0's
int countZeros(const data_packet_t *packet) {
    int count = 0;
    for (int i = 0; i < packet->data_length; i++) {
        for (int j = 0; j < 8; j++) {
            if (((packet->data[i] >> j) & 1) == 0) {
                count++;
            }
        }
    }
    return count;
}

//Count the number of data indexes filled with 1's
int countOnes(const data_packet_t *packet) {
    int count = 0;
    for (int i = 0; i < packet->data_length; i++) {
        for (int j = 0; j < 8; j++) {
            if (((packet->data[i] >> j) & 1) == 1) {
                count++;
            }
        }
    }
    return count;
}

//Function to calculate Cyclic Redundancy Check
uint16_t calculateCRC(uint8_t* data, uint8_t length) {
    uint16_t crc = 0xFFFF;

    for (uint8_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;

        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            else
                crc <<= 1;
        }
    }

    return crc;
}


bool isPacketCorrupted(data_packet_t* packet) {
    uint16_t calculatedCRC = calculateCRC(packet->data, packet->data_length);
    return (calculatedCRC != packet->crc);
}


int main() {

    data_packet_t packet;
    packet.id = 1;
    /*
    packet.data_length = 6;
    packet.data[0] = 'B';
    packet.data[1] = 'A';
    packet.data[2] = 'T';
    packet.data[3] = 'M';
    packet.data[4] = 'A';
    packet.data[5] = 'N';
    */


    packet.data_length = 3;
    packet.data[0] = 0b00001111;
    packet.data[1] = 0b11110000;
    packet.data[2] = 0b01010101;
    
    
    

    
    packet.crc = calculateCRC(packet.data, packet.data_length);

    int zeroCount = countZeros(&packet);
    int oneCount = countOnes(&packet);

    printf("Number of 0's: %d\n", zeroCount);
    printf("Number of 1's: %d\n", oneCount);

    bool isCorrupted = isPacketCorrupted(&packet);
    if (isCorrupted) {
        printf("Packet is corrupted.\n");
    } else {
        printf("Packet is not corrupted.\n");
    }


    return 0;
}