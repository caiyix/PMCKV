#ifndef STORAGE_LEVELDB_DB_THREADTASK_H_
#define STORAGE_LEVELDB_DB_THREADTASK_H_
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>
#include "db/version_edit.h"
#include "message.h"

using namespace std;
namespace leveldb {
	#define process_num 4
	#define task_num 1

	struct fileInfo{
		FileMetaData* fileMetaData;
		int levelNum;             //文件所处的层级
		int fileNum;              //文件具体信息沿用leveldb的FileMetaData结构
		bool modifyFlag = false;  //文件修改标志，用来指明本任务在 P3 是否需要等待上一个任务的 P3完成
		bool writeBackFlag = true;//文件写回标志，用来指明本任务在 P3 生成的文件是否可以直接在 P4写回
	};
	
	class ThreadTask{
		public:
		void process1(vector<ThreadTask*>* taskVector,vector<int>* threadFinished){
			sem_wait(&mes->sem_ts[0]);
			//不是第一个任务，需要检查我现在获取的文件信息是否会被上一个任务修改
			if(taskId != 1){
//				ThreadTask* threadTask = taskVector->at(taskVector->size()-1); 
				//if(taskId == 3){
					//cout<<"*****************************************"<<endl;
				//}
				ThreadTask* threadTask = new ThreadTask();
				for(int i=0;i<taskVector->size();i++){
					if(taskVector->at(i)->taskId == (taskId-1)){
						threadTask = taskVector->at(i);
						break;
					}
				}
				if(threadTask->taskId == (taskId-1)){
					int size = threadTask->inputs_[1].size();
					//上一个任务--仅需要检查最后一个文件；本任务--仅需要检查第一个文件
					int lastFileNum = threadTask->inputs_[1].at(size-1).fileNum;
					int firstFileNum = inputs_[1].at(0).fileNum;
					if(lastFileNum == firstFileNum){//两个任务之间存在范围重叠
						threadTask->inputs_[1].at(size-1).writeBackFlag = false;
						inputs_[1].at(0).modifyFlag = true; 
					}
				}else{
					cout<<"p1-task"<<taskId<<"don't have pre"<<endl;
				}
				//delete threadTask;
				/**/
			}
			cout<<taskId<<"--p1: get compactin file info ,files are:"<<endl;
			cout<<"level i:";
			for(int i=0;i<inputs_[0].size();i++){
				cout<<inputs_[0].at(i).fileMetaData->number<<",";
			}
			cout<<endl;
			cout<<"level i+1:";
			for(int i=0;i<inputs_[1].size();i++){
				cout<<inputs_[1].at(i).fileMetaData->number<<",";
			} 
			cout<<endl;
			mes->p_finish[mes->messageID] = true;
		}
		void process2(){
			sem_wait(&mes->sem_ts[1]);//主线程是否允许我执行本任务 
			cout<<taskId<<"--p2:read files"<<endl;
			cout<<"level i:";
			for(int i=0;i<inputs_[0].size();i++){
				cout<<inputs_[0].at(i).fileMetaData->number<<",";
			}
			cout<<endl;
			cout<<"level i+1:";
			for(int i=0;i<inputs_[1].size();i++){
				cout<<inputs_[1].at(i).fileMetaData->number<<",";
			}
			cout<<endl;
			mes->p_finish[mes->messageID] = true;
		}
		void process3(vector<ThreadTask*>* taskVector,vector<int>* threadFinished){
			sem_wait(&mes->sem_ts[2]);
			bool modifyFlag = inputs_[1].at(0).modifyFlag;//仅需检查第一个文件的modifyFlag
			if(taskId != 1 && modifyFlag){
				ThreadTask* threadTask = taskVector->at(taskVector->size()-1); 
				//若本任务已经入队，那么需要获取上一个任务 
				for(int i=0;i<taskVector->size();i++){
					if(taskVector->at(i)->taskId == (taskId-1)){
						threadTask = taskVector->at(i);
						break;
					}
				}	
				if(threadTask->taskId == (taskId-1)){
					sem_wait(&threadTask->mes->sem_p3);//上一个任务的P3是否完成
				}else{
					cout<<"p3-1-task"<<taskId<<"don't have pre"<<endl;
				}
			}
			/**/
			vector<fileInfo> newfile0,newfile1,newfile2;
			fileInfo fileInfo9,fileInfo10,fileInfo11,fileInfo12,fileInfo13,fileInfo14;
			fileInfo9.levelNum = 4;
			fileInfo10.levelNum = 4;
			fileInfo11.levelNum = 4;
			fileInfo12.levelNum = 4;
			fileInfo13.levelNum = 4;
			fileInfo14.levelNum = 4;
			fileInfo9.fileNum = 9;
			fileInfo10.fileNum = 10;
			fileInfo11.fileNum = 11;
			fileInfo12.fileNum = 12;
			fileInfo13.fileNum = 13;
			fileInfo14.fileNum = 14;
			newfile0.push_back(fileInfo9);
			newfile0.push_back(fileInfo10);
			newfile1.push_back(fileInfo11);
			newfile1.push_back(fileInfo12);
			newfile2.push_back(fileInfo13);
			newfile2.push_back(fileInfo14);
			if(taskId == 1){
				newfile =  newfile0;
			}else if(taskId == 2){
				newfile =  newfile1;
			}else{
				newfile =  newfile2;
			}
			//不是第一个任务，需要检查我现在获取的文件信息是否被上一个任务修改
			//两个任务之间存在范围重叠，我需要获取上一个任务的新文件---即检查任务队列的倒数第二个任务
			
			if(taskId != 1 && modifyFlag){
				ThreadTask* threadTask = taskVector->at(taskVector->size()-1); 
				for(int i=0;i<taskVector->size();i++){
					if(taskVector->at(i)->taskId == (taskId-1)){
						threadTask = taskVector->at(i); 
						break;
					}
				}
				if(threadTask->taskId == (taskId-1)){
					//假设需要上一个任务的部分新文件，此时允许上一个任务把任务不需要的文件写回
					cout<<"****************************"<<threadTask->taskId<<"******************************"<<endl;
					cout<<"threadTask->newfile:"<<threadTask->newfile[0].fileNum<<",don't need"<<endl;
					cout<<"threadTask->newfile:"<<threadTask->newfile[1].fileNum<<",need"<<endl;
					threadTask->newfile[0].writeBackFlag = true; 
					threadTask->newfile[1].writeBackFlag = false; 
					sem_post(&(threadTask->mes->sem_write));
				}else{
					cout<<"p3-2-task"<<taskId<<"don't have pre"<<endl;
				}
					
				
			}
			cout<<taskId<<"--p3:files content marge and sort,new files are"<<endl;
			cout<<"level i+1:";
			for(int i=0;i<newfile.size();i++){
				cout<<newfile[i].fileNum<<",";
			}
			cout<<endl;
			//我的任务完成了，允许下一个任务使用我产生的数据，设置 sem_p3
			sem_post(&mes->sem_p3);
			mes->p_finish[mes->messageID] = true;
		}
		void process4(vector<ThreadTask*>* taskVector,vector<int>* threadFinished){
			sem_wait(&mes->sem_ts[3]);
			bool writeFlag = false;
			ThreadTask* threadTask = taskVector->at(taskVector->size()-1); 
			if(threadTask->taskId == taskId && taskId == task_num){//不会再有新的任务，可以直接写回 
				cout<<"task"<<taskId<<"is the last task"<<endl;
				writeFlag = true; 
			}else{//需要检查下一个任务对本任务的限制 
			    vector<int>::iterator iter = std::find(threadFinished->begin(), threadFinished->end(), taskId+1);
			    if (iter != threadFinished->end())
			    {
			        int size = inputs_[1].size();
				writeFlag = inputs_[1].at(size-1).writeBackFlag;
			    }else{
			       cout<<"****************************"<<taskId<<"******************************"<<endl;
				while(threadTask->taskId == taskId){//需要等待下一个任务入队
					threadTask = taskVector->at(taskVector->size()-1); 
				}
				cout<<"****************************"<<taskId<<"******************************"<<endl;
				while(!threadTask->mes->p_finish[0]){//需要等待下一个任务完成P1
				}
				cout<<"****************************"<<taskId<<"******************************"<<endl;
				int size = inputs_[1].size();
				writeFlag = inputs_[1].at(size-1).writeBackFlag;
			    }
			}
			cout<<"task"<<taskId<<"--p4: writeFlag is :"<<writeFlag<<endl;
			while(!writeFlag){
				sem_wait(&mes->sem_write);	
				writeFlag = true;
			} /**/
			cout<<taskId<<"--p4:new files write to disk,write files are:"<<endl;
			for(int i=0;i<newfile.size();i++){
				if(newfile[i].writeBackFlag){
					cout<<newfile[i].fileNum<<",";
				}
			} 
			mes->p_finish[mes->messageID] = true;
		}
		void process(vector<ThreadTask*>* taskVector,vector<int>* threadFinished){
		 	process1(taskVector,threadFinished);
		 	process2();
		 	process3(taskVector,threadFinished);
		 	process4(taskVector,threadFinished);
		}
    	
    	bool nextProcess()
		{	
			mes->messageID++;
			cout <<"task"<<taskId<<"--start run P"<< mes->messageID+1<< endl;
			sem_post(&(mes->sem_ts[mes->messageID])); 
			
		}
    	
		thread thread_creat(vector<ThreadTask*>* taskVector,vector<int>* threadFinished)
		{
			thread t;
			t = thread(std::mem_fn(&ThreadTask::process), this,taskVector,threadFinished);
			cout<<"create thread,id----:" << t.get_id() << endl;
			return t;
		}
		
		void thread_process()
		{
			task_thread.join();
			cout<<"process thread,id----:" << task_thread.get_id() << endl;
		}
		
		void getFileMetaData(){
			for(int i=0;i<leveldb_inputs_[0].size();i++){
				cout<<"threadTask--leveldb_inputs_[0]:"<<leveldb_inputs_[0][i]->number<<endl;
				fileInfo info;
				info.fileMetaData = leveldb_inputs_[0][i];
				inputs_[0].push_back(info);
			}
			for(int i=0;i<leveldb_inputs_[1].size();i++){
				cout<<"threadTask--leveldb_inputs_[1]:"<<leveldb_inputs_[1][i]->number<<endl;
				fileInfo info;
				info.fileMetaData = leveldb_inputs_[1][i];
				inputs_[1].push_back(info);
			}
		}
		
		int taskId;                 //任务id 
		thread task_thread;         //任务所绑定的子线程 
		Message* mes;               //任务消息，用于主线程对任务执行的控制 
		vector<fileInfo> inputs_[2];//旧文件：i层和i+1层
		vector<fileInfo> newfile;   //新文件:i+1层
		vector<FileMetaData*> leveldb_inputs_[2]; //旧文件：i层和i+1层,数据继承自compaction
	};
}  // namespace leveldb
#endif  // STORAGE_LEVELDB_DB_THREADTASK_H_


