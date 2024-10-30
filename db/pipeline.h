#ifndef STORAGE_LEVELDB_DB_PIPELINE_H_
#define STORAGE_LEVELDB_DB_PIPELINE_H_
#include <iostream>
#include <thread>
#include "threadTask.h"
using namespace std;

class Pipeline{
	public:
		void pipelineStart(vector<ThreadTask*>* taskVector,vector<int>* threadFinished){
			cout<<"-------------------pipeline start-------------------"<<endl;
			// 创建第一个任务 0 
			ThreadTask* threadTask = new ThreadTask(); 
			threadTask->taskId = 1;
			Message* mes = new Message();
        	mes->messageID = 0;
			sem_post(&mes->sem_ts[0]);
			threadTask->mes = mes;
			thread t = threadTask->thread_creat(taskVector,threadFinished);
			threadTask->task_thread = move(t);
			taskVector->push_back(threadTask);
			//threadTask->thread_process();
			//cout<<"create thread,id----:" << t.get_id() << endl;
			cout<<"create task:"<<threadTask->taskId<<endl;
			threadNum++;
		} 
		void pipelineProcess(vector<ThreadTask*>* taskVector,vector<int>* threadFinished,int taskNum){
			// 创建一个新的任务 2/3 需要判断是否还需要创建一个新的任务
			if(threadNum < taskNum){
				ThreadTask* threadTask = new ThreadTask(); 
				threadTask->taskId = threadNum+1;
				Message* mes = new Message();
			    mes->messageID = 0;
				sem_post(&mes->sem_ts[0]);
				threadTask->mes = mes;
				thread t = threadTask->thread_creat(taskVector,threadFinished);
				threadTask->task_thread = move(t);
				taskVector->push_back(threadTask);
				//cout<<"create thread,id----:" << t.get_id() << endl;
				cout<<"create task:"<<threadTask->taskId<<"----"<<taskVector->size()<<endl;
				threadNum++;
			}
			// 检查已有的任务进度 
			int i=0;
			while(i<taskVector->size()){
				ThreadTask* threadTask = taskVector->at(i); 
				if(threadTask->mes->messageID < threadTask->mes->messageMaxID && threadTask->mes->p_finish[threadTask->mes->messageID]){
					cout<<"threadTask taskID---"<<threadTask->taskId<<"---go on"<<endl;
					threadTask->nextProcess();
				}
				i++;
			}
			for (vector<ThreadTask*>::iterator it = taskVector->begin(); it != taskVector->end();) {
				if((*it)->mes->messageID == (*it)->mes->messageMaxID && (*it)->mes->p_finish[(*it)->mes->messageID]){
					cout<<"----------------------task"<<(*it)->taskId<<"finished----------------------"<<endl;
					threadFinished->push_back((*it)->taskId);
					taskVector->erase(it); 
				}else {
					++it;
				}
			}
		} 
		void pipelineEnd(vector<ThreadTask*>* taskVector){
			if(taskVector->size() == 0){
				cout<<"--------------------pipeline end--------------------"<<endl;
			}
		} 
		void pipelineCompaction(int taskNum){
			threadNum = 0;
			vector<ThreadTask*>* taskVector = new vector<ThreadTask*>;
			vector<int>* threadFinished = new vector<int>;
		    pipelineStart(taskVector,threadFinished);
		    cout<<"-------------------pipeline process-------------------"<<endl;
		    while(taskVector->size() > 0 || threadNum < taskNum) {
		    	pipelineProcess(taskVector,threadFinished,taskNum);
			}
			pipelineEnd(taskVector); 
			delete taskVector;
			delete threadFinished;
		} 
		private:
			int threadNum;
		
};
#endif  // STORAGE_LEVELDB_DB_PIPELINE_H_


