#ifndef STORAGE_LEVELDB_DB_MESSAGE_H_
#define STORAGE_LEVELDB_DB_MESSAGE_H_

#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;
#define sem_t_num 4

class Message{
	public:
		int messageID;           //��ǰ���������Ľ׶�
		int messageMaxID;        //�������ܴ������ս׶Σ���Ϊ�ж������Ƿ���ɵ����� 
		bool p_finish[sem_t_num];//���̸߳��׶��Ƿ��Ѿ���� 
		sem_t sem_ts[sem_t_num]; //���߳��Ƿ�������׶�ִ�� 
    	sem_t sem_p3;            //���������һ����������ƣ�������֤��������֮���������������ָP3��
    	sem_t sem_write;         //�����źţ���������������д�ص������ٴμ�鲢����ִ�У���ָP4�� 
		
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


