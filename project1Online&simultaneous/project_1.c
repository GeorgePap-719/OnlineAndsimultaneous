#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h> 
#include <time.h>
#include <sys/wait.h> //for wait function
#include <pthread.h>
#include <string.h>
//defines
#define Tseatfind 8 //seconds
#define Tcardcheck 3 //seconds
#define Npelates 100
//initialize mutex counters
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
//initialize condition variables
pthread_cond_t tilefonites_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t bankterminal_cond = PTHREAD_COND_INITIALIZER;
//
int avaible_tilefonites = 12;
int avaible_bankterminals = 6;
//initialize 4 global variables for checking
int BankMoney=0;
//the 4 rows if they are full
int Arow=0,Brow=0,Crow=0,Drow=0;
//tracking the total seats 
//failed orders
int Forders=0;
float ord=0;
clock_t begin1 ,end1,begin2,end2;
clock_t begin3 ,end3,begin4,end4;

//functions
void *orderthread(void *);
void *searchthread(void *);
void *credithread(void *);
//init global char aray
char Az[40][40], Bz[40][40],Cz[80][80],Dz[100][100];
//initialize global struct
typedef struct array {
	int A[40], B[40],C[80],D[100];
	
} array;
array zoneseats;
//initialize struct to pass arguments in the searchthread
typedef struct arg_struct {
	char *kind;
	int number;
	
}arg_struct;
//calculating times
double time_wait1=0,time_wait2=0,w1time=0;
double time_used1=0,time_used2=0,w2time=0;
//tracking the id of each customer
float arrayid[Npelates];
float zoneA[40],zoneB[40],zoneC[80],zoneD[100];
//
int main (int argc , char **argv){


pthread_t threads[Npelates];
	
	
	int i;
	
	
	//warning message
	printf("the program is running, may take a while\n");
	//sets everything to zero , zero means seat is empty and 1 is taken
	for(i=0;i<40;i++){
	zoneseats.A[i] = 0;
	}
	for(i=0;i<40;i++){
	zoneseats.B[i] = 0;
	}
	
	for(i=0;i<80;i++){
	zoneseats.C[i] = 0;
	}
	
	for(i=0;i<100;i++){
	zoneseats.D[i] = 0;
	}
	
	//creation of 100threas/pelates
	for(i=0;i<Npelates;i++){
		pthread_create(&threads[i],NULL,orderthread,NULL);
	}
	
	
	//waiting to end each thread
	for(i=0;i<Npelates;i++){
		//ending the threads
		//sleep(1);
		pthread_join(threads[i],NULL);
	}
	
	
	/*
	printf("\n\n");	
	for(i=0;i<40;i++){
	printf("seatA:%d\n",zoneseats.A[i]);
	}
	for(i=0;i<40;i++){
	printf("seatB:%d\n",zoneseats.B[i]);
	}
	
	for(i=0;i<80;i++){
	printf("seatC:%d\n",zoneseats.C[i]);
	}
	
	for(i=0;i<100;i++){
	printf("seatD:%d\n",zoneseats.D[i]);
	}
	*/
	printf("\n\n");	
	for(i=0;i<40;i++){
	printf("\nZone Aseat[%d] id:%f\n",i,zoneA[i]);
	}
	for(i=0;i<40;i++){
	printf("\nZone Bseat[%d] id:%f\n",i,zoneB[i]);
	}
	
	for(i=0;i<80;i++){
	printf("\nZone Cseat[%d] id:%f\n",i,zoneC[i]);
	}
	
	for(i=0;i<100;i++){
	printf("\nZone Dseat[%d] id:%f\n",i,zoneD[i]);
	}
	
	//printf("Test Zone Seat %s \n",Az[0]);
	printf("\nTotal Money recieved:%d \n",BankMoney);
	ord=(float)Forders/(float)Npelates;
	//printf("\n ord:%.2f \n",ord);
	ord=(float)ord*(float)100;
	printf("Failed orders:%.1f percentage\n",ord);
	//caclulating times>>
	w1time=(double)time_wait1+(double)time_wait2;
	w2time=(double)time_used1+(double)time_used2;
	//printf("Average time spend on waiting to connect before:%f\n",w1time);
	w1time=(double)w1time/(double)Npelates;
	w2time=(double)w2time/(double)Npelates;
	printf("Average time spend on waiting to connect:%f\n",w1time);
	printf("Average time spend on servicing a customer:%f\n",w2time);
	
	if(Arow>=40 && Brow>=40 && Crow>=80 && Drow>=100){
		printf("The theater is full\n");
	}
		
	
return 0;
}
//Functions
void *orderthread(void *dummy){
	void *status,*status2;
	pthread_t seatsearchthread, creditcheckthread;
	arg_struct *args =malloc(sizeof(struct arg_struct));
	time_t t;
	int zone,j=0,i=0,k=0,customer=0;
	//t is a special value to keep track the number of the customers
	//int id_thread = (intptr_t)pthread_self();
	
	/* Intializes random number generator */
   srand((unsigned) time(&t));
	sleep(1);//i use sleep here to make sure we get random numbers everytime
	//srand();
	//setting random seats and rows
	//zone
	zone = rand()%100+1;
	//printf("%d",zone);
	if(zone <= 10)args->kind = "A";
	if(zone > 10 && zone <= 40)args->kind = "B";
	if(zone > 40 && zone <= 60)args->kind = "C";
	if(zone > 60 && zone <= 100)args->kind = "D";
	args->number = rand()%5+1;//number of seats
	//
	
	//printf("kind:%s  and number:%d\n",args->kind,args->number);
	//do stuff
	
	//

	//create the two threads
	pthread_create(&seatsearchthread,NULL,searchthread,args);
	pthread_create(&creditcheckthread,NULL,credithread,NULL);
	
	//printf("current thread id:%d\n",*(int*)pthread_self());
	//printf("avaible_tilefonites%d: \n",avaible_tilefonites);
	//printf("avaible_bank_terminals:%d \n",avaible_bankterminals);
	//end
	//waiting the two threads to end
	pthread_join(seatsearchthread,&status);
	pthread_join(creditcheckthread,&status2);
	//casting the result from pinter to integer
	int threadnumber = (intptr_t) status;
	int threadnumber2 = (intptr_t) status2;
	//printf("this is the status that i return :%d\n",*(int*)status);
	//printf("this is the status that i return :%d\n",threadnumber);
	//printf("\nthis is the status that i return :%d\n",threadnumber2);

	
	//filling the right seats
	pthread_mutex_lock(&mutex3);
	//checking if the rows are full
	if(Arow>=40 && Brow>=40 && Crow>=80 && Drow>=100){
		//the theater is FULL so we return the client
		//printf("\n\nThe theater is full\n\n");
		//printf("\nTotal Money recieved:%d \n",BankMoney);
		//ord=(Forders/Npelates)*100;
		//printf("Failed orders:%d percentage\n",Forders);
		pthread_mutex_unlock(&mutex3);
		return 0;
	}
	//pthread_cond_wait(&tilefonites_cond,&mutex1);
	//checking if clinet has money and theater enough seats
	if(threadnumber == 1 && threadnumber2 == 1){
		//checking where customer wants to seat and 
		//also taking the right sum of money from the customer
		if(strcmp(args->kind,"A")==0){
			for(i=0;i<40;i++){
				if(zoneseats.A[i] == 0){
					//reservation is successful
printf("\nYour reservation is successful,your id:%lu\n",pthread_self());
printf("It costed you:%d,for:%d seats in zone A \n",args->number*50,args->number);					
					k=i+args->number;
					for(j=i;j<k;j++){
						zoneseats.A[j]=1;
						Arow++;
						BankMoney=BankMoney+50;
						//zoneA[i]=arrayid[customer];//tracking the id of customer
						zoneA[j]=pthread_self();
					}
					break;
				}
					
			}
		}
		
		if(strcmp(args->kind,"B")==0){
			for(i=0;i<40;i++){
				if(zoneseats.B[i] == 0){
					//reservation is successful
printf("\nYour reservation is successful ,your id:%lu\n",pthread_self());
printf("It costed you:%d,for:%d seats in zone B \n",args->number*40,args->number);					k=i+args->number;
					for(j=i;j<k;j++){
						zoneseats.B[j]=1;
						Brow++;
						BankMoney=BankMoney+40;
						zoneB[j]=pthread_self();//tracking the id of customer
					}
					break;
				}
					
			}
		}
		if(strcmp(args->kind,"C")==0){
			for(i=0;i<80;i++){
				if(zoneseats.C[i] == 0){
					//reservation is successful
printf("\nYour reservation is successful ,your id:%lu\n",pthread_self());
printf("It costed you:%d,for:%d seats in zone C \n",args->number*35,args->number);					k=i+args->number;
					for(j=i;j<k;j++){
						zoneseats.C[j]=1;
						Crow++;
						BankMoney=BankMoney+35;
						zoneC[j]=pthread_self();//tracking the id of customer
					}
					break;
				}
					
			}
		}
		if(strcmp(args->kind,"D")==0){
			for(i=0;i<100;i++){
				if(zoneseats.D[i] == 0){
					//reservation is successful
printf("\nYour reservation is successful ,your id:%lu\n",pthread_self());
printf("It costed you:%d,for:%d seats in zone D \n",args->number*30,args->number);					k=i+args->number;
					for(j=i;j<k;j++){
						zoneseats.D[j]=1;
						Drow++;
						BankMoney=BankMoney+30;
						zoneD[j]=pthread_self();//tracking the id of customer
					}
					break;
				}
					
			}
		}
	
		customer++;
	//end of filling
	}
	if(threadnumber == 0){
		printf("Not enough seats for the row you asked for\n");
		Forders++;//failed orders
	}
	 if(threadnumber2 == 0){
		printf("\n\n Your credit card is not vaild\n\n");
		Forders++;//failed orders
	}
	//if(threadnumber==0)exit;
	
	
	//printf("order id:%d \n",id_thread);
	pthread_mutex_unlock(&mutex3);	
	//

	return 0;
	
}
//functions//--
void *searchthread(void* arguments){
	int i,flag,Nseats=0;
	arg_struct* args =(struct arg_struct*) arguments;
	//printf("kind:%s  and number:%d\n",args->kind,args->number);
	//checking for avaible tilefwnhtes
	while(1){
	
		
	if(avaible_tilefonites > 0){
		//printf("avaible_tilefonites:%d \n",avaible_tilefonites);
		//wrong sleep? 
		sleep(Tseatfind);
		pthread_mutex_lock(&mutex1);
		 begin1 = clock();	
		//checking for avaible tilefwnites
		if(avaible_tilefonites > 0){
		//printf("avaible_tilefonites:%d \n",avaible_tilefonites);
	//printf("inside tilefwniths avaible_tilefonites%d: \n\n\n",avaible_tilefonites);
			avaible_tilefonites--;
			begin3=clock();
			 end1 = clock();	
			//only!! searching for empty seats
		if(strcmp(args->kind,"A")==0){
			for(i=0;i<40;i++){
				if(zoneseats.A[i] == 0)Nseats++;
			}
		}
			if(strcmp(args->kind,"B")==0){
				for(i=0;i<40;i++){
					if(zoneseats.B[i] == 0)Nseats++;
			}			
		
		}
			if(strcmp(args->kind,"C")==0){
				for(i=0;i<80;i++){
					if(zoneseats.C[i] == 0)Nseats++;
			}
		
		}
			if(strcmp(args->kind,"D")==0){
				for(i=0;i<100;i++){
					if(zoneseats.D[i] == 0)Nseats++;
			}
		
		}
		
			//checking for the right number of seats
			//1 means enough seats, zero means not enough seats
			//here we are just checking the availbe seats
			//outside this thread ,at the orderthread we lock the seats
			//unlock the mutex cause we dont need him anymore
			if(Nseats>=args->number){
				//enough seats
				flag=1;
				break;
			}
				else { 
					//not enough seats;
					flag=0;
					break;
			}
		
			
		}else{
			pthread_cond_wait(&tilefonites_cond,&mutex1);
			pthread_mutex_unlock(&mutex1);	
			
		
		//printf("before waitavaible_tilefonites%d: \n",avaible_tilefonites);
		//pthread_cond_wait(&tilefonites_cond,&mutex1);
		//avaible_tilefonites++;prob wrong here

	
			 }
		}
		
		
	}
	/////
		//outside WHILE
		//calculating the time
		
		//printf("\n\n\n%f TIME SPEND\n\n\n",time_wait1);
		
	
	if(flag==1){
		
		pthread_cond_signal(&tilefonites_cond);
	//	pthread_cond_broadcast(&tilefonites_cond);
		time_wait1 =(double)time_wait1+(double)(end1-begin1)/CLOCKS_PER_SEC;
		 end3 = clock();	
		time_used1=(double)time_used1+(double)(end3-begin3)/CLOCKS_PER_SEC;
		avaible_tilefonites++;
		pthread_mutex_unlock(&mutex1);	

		return (void *) 1; //right one 
	
	}
	
	if(flag==0){
		
		pthread_cond_signal(&tilefonites_cond);
		time_wait1 =(double)time_wait1+(double)(end1-begin1)/CLOCKS_PER_SEC;
		//pthread_cond_broadcast(&tilefonites_cond);
		 end3 = clock();	
		time_used1=(double)time_used1+(double)(end3-begin3)/CLOCKS_PER_SEC;
		avaible_tilefonites++;
		pthread_mutex_unlock(&mutex1);	
	
		return (void *) 0; 
	
	}
	return NULL;
	
}

void *credithread(void* Dummy){
	int failed=0;
	
	while(1){
		//printf("avaible_bankterminals %d: \n",avaible_bankterminals);
		 srand((unsigned) time(NULL));
		if(avaible_bankterminals > 0){
			sleep(Tcardcheck);
			pthread_mutex_lock(&mutex2);
			 begin2 = clock();	
			if(avaible_bankterminals > 0){
				avaible_bankterminals--;
				 begin4 = clock();	
				 end2 = clock();	
				
				sleep(0.5);//i use sleep here to make sure we get random numbers everytime
				//srand();
				//setting random seats and rows
				//zone
				failed = rand()%100+1;
			//	printf("\n\nfailed:%d\n\n",failed);
				//checking for enough money
				if(failed <= 85){
					//printf("\n\nSucces Bank\n\n");
					pthread_cond_signal(&bankterminal_cond);
					avaible_bankterminals++;
					 end4 = clock();	
					time_wait2 =(double)time_wait2+(double)(end2-begin2)/CLOCKS_PER_SEC;
					time_used2 =(double)time_used2+(double)(end4-begin4)/CLOCKS_PER_SEC;
					pthread_mutex_unlock(&mutex2);	
					return (void *) 1; //1 means success
					
				}
				else{
					//printf("\n\nFailed Bank\n\n");
					pthread_cond_signal(&bankterminal_cond);
					time_wait2 =(double)time_wait2+(double)(end2-begin2)/CLOCKS_PER_SEC;
					end4 = clock();	
					time_used2 =(double)time_used2+(double)(end4-begin4)/CLOCKS_PER_SEC;
					avaible_bankterminals++;
					pthread_mutex_unlock(&mutex2);	
					return (void *) 0; //0 means failure
						
					}
			}
			else{
				
			pthread_cond_wait(&bankterminal_cond,&mutex2);
			pthread_mutex_unlock(&mutex2);	
			
		
			}
			
		}
			
	}

}















