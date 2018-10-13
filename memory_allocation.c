#include <stdio.h>
#include <stdlib.h>

//to keep the process id data
struct part {
	int start; // start address
	int end; // end address
	int pid; // process Id
};

//structure of the linked node
struct memory {
	struct part data;
	struct memory * next; //pointer for the the next node
	struct memory * prev; //pointer for the previous node
};

struct memory * base; 

//inserting a node after a node
int insertAfter(struct memory * node, struct part data)
{
	if(node != NULL)
	{
		struct memory * new = malloc(sizeof(struct memory));
		new -> data = data;
		new -> next = NULL;
		new -> prev = NULL;


		struct memory * a = node;
		struct memory * b = new;
		struct memory * c = node -> next;

		a -> next = b;
		b -> prev = a;

		if(c != NULL) {
			b -> next = c;
			c -> prev = b;
		}

		return 1;
	}


	return 0;
}

int insertBefore(struct memory * node, struct part data){
	if(node != NULL){
		struct memory * new = malloc(sizeof(struct memory));
		new -> data = data;
		new -> next = NULL;
		new -> prev = NULL;


		struct memory * a = node -> prev;
		struct memory * b = new;
		struct memory * c = node;

		b -> next = c;
		c -> prev = b;

		if(a != NULL){
			a -> next = b;
			b -> prev = a;
		}


		return 1;
	}

	return 0;	
}

int removeNode(struct memory * node){
	if(node != NULL){
		struct memory * a = node -> prev;
		struct memory * b = node;
		struct memory * c = node -> next;

		if(a != NULL){
			a -> next = c;
		}

		if(c != NULL){
			c -> prev = a;
		}

		free(b);

		return 1;
	}

	// return 0 if the node supplied was NULL
	return 0;
}

//print the current memory layout
void display(){
	printf("\t\tCURRENT MEMORY LAYOUT\n");
	printf("\t\t#########################################\n");
	struct memory * now = base;
	while(now != NULL){
		struct part data = now -> data;

		printf("\t\t[%4d - %4d]\t", data.start, data.end);

		if(data.pid == -1) printf("OS");
		else if(data.pid == 0) printf("FREE");
		else printf("P%d", data.pid);

		printf("\t(Size: %4dK)\n", data.end - data.start + 1);

		now = now -> next;
	}
	printf("\t\t#########################################\n");
	printf("\n\n");
}

// process ID, and size of memory
int allocate(int pid, int size){

	struct memory * now = base;
	while(now != NULL){
		if((now->data).pid == 0) {
			int freeSpace = (now -> data).end - (now -> data).start + 1; // size of the memory 
			
			if(freeSpace > size){
				// inputing data to the part 
				struct part data;
				data.pid = pid;
				data.start = (now->data).start; 
				data.end = data.start + size - 1;

				insertBefore(now, data);

				(now -> data).start = data.end + 1;

				break;
			} else if(freeSpace == size) {
				
				(now -> data).pid = pid;
				break;
			}
		}
		now = now -> next;
	}
		
	

	// failed to allocate memory
	return 0;
}

int terminate(int pid){
	if(pid > 0){
		
		struct memory * now = base;
		// remove the pid node
		while(now != NULL){			
			if((now -> data).pid == pid){

				(now -> data).pid = 0;
				
				struct memory * a = now -> prev;
				struct memory * b = now;
				struct memory * c = now -> next;

				// combine the free spacees
				// a and b
				if(a != NULL && (a -> data).pid == 0){
					(b -> data).start = (a -> data).start;
					removeNode(a); 
				}

				// b and c
				if(c != NULL && (c -> data).pid == 0){
					(b -> data).end = (c -> data).end;
					removeNode(c);
				}

				break; 
			}

			now = now -> next;
		}
	}

	return 0;
}

void initialize(int size){
	base = malloc(sizeof(struct memory));
	struct part systemMemory;
	systemMemory.start = 0;
	systemMemory.end = size - 1;
	systemMemory.pid = -1;

	base -> data = systemMemory;
	base -> next = NULL;
	base -> prev = NULL;

	//inserting freespace to the memory
	struct part free;
	free.start = size + 1;
	free.end = 2559;
	free.pid = 0;

	insertAfter(base, free);
}

int main(){
	
	
	char res = 'X';
	printf("\n\n");
	printf("\t\t(((((((((((((((((((((((((((((!)))))))))))))))))))))))))))))\n\n");
	printf("\t\t--------------------MEMORY ALLOCATION---------------------\n\n");
	printf("\t\t(((((((((((((((((((((((((((((!)))))))))))))))))))))))))))))\n\n");
	
	printf("\t\tA : TO INITIALIZE SYSTEM MEMORY \n");
	printf("\t\tB : ALLOCATE A NEW MEMORY\n");
	printf("\t\tC : TO TERMINATE A MEMORY\n");
	printf("\t\tD : TO DISPLAY CURRENT MEMORY LAYOUT\n");
	printf("\t\tQ : TO QUIT \n");
	
	while(res != 'Q')
	{
		
		
		printf("\t\tENTER YOUR RESPONSE : ");
		scanf("%c",&res);
		getchar();//to avoid enter
		
		if(res=='A')
		{
			int sysize;
			printf("\t\tENTER SYSTEM SIZE : ");
			scanf("%d",&sysize);
			getchar();
			initialize(sysize);
			display();
		}
		else if(res=='B')
		{
			int pd , size;
			printf("\t\tENTER THE PROCESS ID NUMBER : P");
			scanf("%d",&pd);
			getchar();
			printf("\t\tENTER THE MEMORY SIZE YOU NEED : ");
			scanf("%d",&size);
			getchar();
			allocate(pd, size);
			display();
		}
		else if(res=='C')
		{
			int pi;
			printf("\t\tENTER THE PROCESS ID YOU NEED TO TERMINATE : P");
			scanf("%d",&pi);
			getchar();
			terminate(pi);
			display();
		}
		else if(res=='Q')
		{
			break;
		}
		else
		{
			printf("\t\tWRONG INPUT TRY AGAIN");
		}
		
	}

	
	

	return 0;
}
