#ifndef STORAGE_LEVELDB_DB_MESSAGE_H_
#define STORAGE_LEVELDB_DB_MESSAGE_H_

#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;
#define sem_t_num 4

class Message{
	public:
		int messageID;           //当前任务所处的阶段
		int messageMaxID;        //任务所能处的最终阶段，作为判断任务是否完成的依据 
		bool p_finish[sem_t_num];//子线程各阶段是否已经完成 
		sem_t sem_ts[sem_t_num]; //主线程是否允许各阶段执行 
    	sem_t sem_p3;            //本任务对下一个任务的限制，用来保证两个任务之间的数据依赖（特指P3）
    	sem_t sem_write;         //传递信号，允许被本任务限制写回的任务再次检查并继续执行（特指P4） 
		
		Message(){
    		for(int i = 0; i < sem_t_num; i++){
    			sem_init(&sem_ts[i],0,0);
			} 
			for(int i = 0; i < sem_t_num; i++){
    			p_finish[i] = false;
			}
			sem_init(&sem_p3,0,0);
			messageMaxID = sem_t_num-1;
			sem_init(&sem_write,0,0);
		}
};
#endif  // STORAGE_LEVELDB_DB_MESSAGE_H_


