#include <stdint.h>
#include <time.h>
#include <sys/mman.h>
#include <math.h>
#include <stdio.h>

//多阶HASH阶数、每阶的长度以及最大BLOCK的个数
struct head_info {
	int32_t bucket_time;
	int32_t bucket_len;
	int32_t max_block;
};

typedef struct head_info  head;

const uint32_t PLOY            = 0x04C11DB7;
uint32_t crc32_table[256];

int IsPrime(uint32_t value);
uint32_t Crc32GetSummedPloys(uint32_t top_byte);

int GeneratePrimes(uint32_t* primes,
			    uint32_t  max,
		   	    uint32_t  num)
{
	uint32_t i = 0, j = 0;

	for (i = max; i > 1; i--) {
		if (IsPrime(i)) {
			primes[j] = i;
			j++;
		}

		if (j == num) return j;
	}

	return j;
}

int IsPrime(uint32_t value)
{
	uint32_t square = (uint32_t)sqrt(value);
	uint32_t i;

	for(i = 2; i <= square; i++) {
		if(value % i == 0)
			return 0;
	}

	return 1;
}


void Crc32CreateTable(uint32_t* crc32_table)
{
	for (uint32_t i = 0; i < 256; i++) {
		crc32_table[i] = Crc32GetSummedPloys(i);
	}       

	return; 
}

uint32_t Crc32GetSummedPloys(uint32_t top_byte)
{
	uint32_t summed_ploys = top_byte << 24;

	for (int i = 0; i < 8; i++) {
		uint32_t top_bit = (summed_ploys >> 31) & 0x01;
		summed_ploys = summed_ploys << 1;

		if (top_bit == 0x01)
			summed_ploys = summed_ploys ^ PLOY; 
	}       

	return summed_ploys;
}

uint32_t Crc32Compute(unsigned char* data, int len)
{
	int j=0;
    printf("str=%s,%d\n",data,len);
    for( ; j<len;j++){
        printf("%x ",data[j]&0xff);
        data[j]=data[j]&0xff;
    }

    for(j=0; j<len;j++){
         printf("%x ",data[j]);
    }

    printf("\n");
	uint32_t reg = 0;

	for(int i = 0; i < len; i++) {
		reg = (reg << 8) ^ crc32_table[(data[i] ^ ((reg >> 24)) & 0xFF)];
	}       

	return reg;
}
uint32_t Crc32Compute1(char* oridata, int len)
{
	
    uint32_t crc = 0xFFFFFFFF;

    const uint8_t * data = (const uint8_t *) oridata;
    const uint8_t * dataEnd = (const uint8_t *)&oridata[len];

    while (data < dataEnd)
    {
        crc = (crc >> 8) ^ crc32_table [(crc & 0xFF) ^ *data++];
    }

    //return ENET_HOST_TO_NET_32 (~ crc);
    return crc;
}


uint32_t Crc32Append(uint32_t crc32, const char* data, int len)
{
	uint32_t reg = crc32;

	for(int i = 0; i < len; i++) {
		reg = (reg << 8) ^ crc32_table[(data[i] ^ ((reg >> 24)) & 0xFF)];
	}       

	return reg;
}


int main(void){
//	uint32_t crc32_table[256];	
	//crc32
	Crc32CreateTable(crc32_table);
	
	head sthead;
	sthead.bucket_time=50;
	sthead.bucket_len=500000;
	sthead.max_block=100000;
	
    head sthead1;
    sthead1.bucket_time=50;
    sthead1.bucket_len=500000;
    sthead1.max_block=100000;
    

   // printf("%0x\n",Crc32Compute(( char *)&sthead, sizeof(sthead)));
	printf("%0x\n",Crc32Compute((unsigned char  *)&sthead, sizeof(sthead)));

	printf("%0x\n",Crc32Compute1((char  *)&sthead, sizeof(sthead)));
    
    printf("%0x\n",Crc32Compute1((char  *)&sthead1, sizeof(sthead1)));

	//printf("%0x\n",Crc32Compute2((char  *)&sthead, sizeof(sthead)));
    
    
	return 0;
}
