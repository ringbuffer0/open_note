#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mem_hash.h"

using namespace mem_hash;


//---hashnode数据部分大小
//const uint32_t BUF_LEN = 5376;
const uint32_t BUF_LEN = 1;

MemHash *mem;

void test_for_mem_hash_stat()
{
	uint32_t node_used_perct = 0;
	uint32_t block_used_perct = 0;
	mem->Stat(node_used_perct, block_used_perct);
	printf("node_used_perct = %u block_used_perct = %u\n", node_used_perct, block_used_perct);
}

void test_for_mem_hash_foreachkey()
{
	uint64_t key = 0;
	while(mem->ForEachKey(key))

		printf("%lu\n", key);
}

void test_for_mem_hash_meta()
{
	uint32_t bucket_time, bucket_len, max_block;
	mem->Meta("zfz.memhash", bucket_time, bucket_len, max_block);
	printf("bucket_time = %u bucket_len = %u max_block = %u\n", bucket_time, bucket_len, max_block);
}

void test_for_mem_hash_set(uint64_t key, char *p)
{
	int ret=mem->Set(key, p, BUF_LEN);
    if(ret<0){
        printf("insert error,%d\n",ret);
    }
    else{
     //   printf("insert succ\n");
    }
}

void test_for_mem_hash_del(uint64_t key)
{
	int ret=mem->Del(key);
    if(ret<0){
         printf("delete error\n");
    }
}

int main ()
{
	char data[BUF_LEN];
	memset(data, 'a', BUF_LEN);
	data[BUF_LEN - 1] = 0;


	//这里的话,多阶hashtable的管理节点也是在堆上分布的
	//实际上存储空间在mmap共享内存里
	mem = new MemHash();
	
	//
	mem->Init("zfz.memhash", 3600, mem_hash::OPEN_MLOCK, 10000, MS_ASYNC, 50, 100000, 500000);

	for (uint64_t i = 1; i <= 100000; i++) {
		//uint64_t rand_key = (uint64_t)rand();
		test_for_mem_hash_set(i, data);
	}

	for(uint64_t i = 1; i <= 100000; i++) {
		////test_for_mem_hash_del(i);
	}


    printf("here\n");

	test_for_mem_hash_meta();
	test_for_mem_hash_stat();

    printf("get key....\n");
    sleep(5);
	test_for_mem_hash_foreachkey();

	delete mem;
	return 0;
}
