#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define SIZE 255

typedef struct vertex{
	int id;
	int level;
	double betweenness;
	char nameOfVertex[SIZE];
	struct adjacentOfVertex *AOV;
	struct vertex *next;
}graph;

typedef struct adjacentOfVertex{
	int id;
	int level;
	struct adjacentOfVertex *next;
}adjOV;

graph *vertexHeader=NULL;

void readVertex();
void readAdjacent();
void createVertex(int vertexId,char *vertexName);
void addAdjacent(int vertextId,int adjacentId);
void matrix();
void degreeCentrality();
void closenessCentrality();
void betweennessCentrality();
int betweennessFinder(int id);
void levelPlus(int id,int num);
void resetLevel();
int shortestPath(int targetId);
int compare(char *a,char *b);

int arr[SIZE];
int dist=0;

int lengthOfArray();
void insertLast(int data);
void deleteFirst();
void deleteArr();

void main(){
	readVertex();
	readAdjacent();
	printf("\n------------------------------------------------------------------------------------------\n");
	printf("MATRIX\n\n");
	matrix();
	printf("\n------------------------------------------------------------------------------------------\n");
	printf("DEGREE CENTRALITY\n\n");
	degreeCentrality();
	printf("\n------------------------------------------------------------------------------------------\n");
	printf("CLOSENESS CENTRALITY\n\n");
	closenessCentrality();
	printf("\n------------------------------------------------------------------------------------------\n");
	printf("BETWEENNESS CENTRALITY\n\n");
	printf("SOURCE\tTARGET\tINTERMEDIA NODES\n");
	betweennessCentrality();
}

void createVertex(int vertexId,char *vertexName){
	graph *p,*q;
	
	p=(graph *)malloc(sizeof(graph));
	p->id=vertexId;
	p->level=0;
	p->betweenness=0;
	strcpy(p->nameOfVertex,vertexName);
	p->AOV=NULL;
	p->next=NULL;
	
	
	if(vertexHeader==NULL){
   		vertexHeader=p;	
	}else {
		q=vertexHeader;
		while(q->next!=NULL){
			q=q->next;
		}
		q->next=p;
	}  	
}

void addAdjacent(int vertextId,int adjacentId){
	graph *p;
	adjOV *ap,*aq,*ar;
	
	ap=(adjOV *)malloc(sizeof(adjOV));
	ap->id=adjacentId;
	ap->level=0;
	ap->next=NULL;
   	
   	
   	p=vertexHeader;
   	while (p != NULL) {
    	if(p->id==vertextId){
    		if(p->AOV==NULL){
    			p->AOV=ap;
			}else{
				aq=p->AOV;
				while(aq->next!=NULL){
					aq=aq->next;
				}
				aq->next=ap;
			}
			break;
		}
		p=p->next;
  	}
   	
}

void matrix(){
	graph *p;
	adjOV *ap;
	
	int counter=0;
	
	p=vertexHeader;    
  	while (p != NULL) {
  		counter++;
    	printf("\t|%s",p->nameOfVertex);
		p=p->next;
  	}
  	
  	int i=0,j,control=0;
  	
  	p=vertexHeader;    
  	while (p != NULL) {
  		i++;
  		if(p->id==i){
  			
  			printf("\n------------------------------------------------------------------------------------------\n");
  			printf("%s\t|",p->nameOfVertex);
  			for(j=1;j<=counter;j++){
  				ap=p->AOV;
  				while(ap!=NULL){
	  				if(ap->id==j){
	  					control=1;
	  					break;
					}else{
						control=0;
					}
  					ap=ap->next;
				}
				if(control==1){
	  				printf("   1\t|");
				}else if(control==0){
					printf("   0\t|");
				}
			}
		}
		p=p->next;
  	}
}

void degreeCentrality(){
	graph *p;
	adjOV *ap;
	
	int counter=0;
	
	p=vertexHeader;    
  	while (p != NULL) {
    	counter++;
		p=p->next;
  	}
	
	int i=0;
	double j;
	
	p=vertexHeader;
  	while (p != NULL) {
  		j=0;
  		i++;
  		if(p->id==i){
  			printf("%s \t= ",p->nameOfVertex);
  			ap=p->AOV;
  			while(ap!=NULL){
  				j++;
  				ap=ap->next;
			}
			printf("%lf\n",j/(counter-1));
		}
		p=p->next;
	}
}

void closenessCentrality(){
	int i,j,result=0;
	double counter=0;
	
	graph *p;
	
	p=vertexHeader;    
  	while (p != NULL) {
  		counter++;
		p=p->next;
	}
	
	for(i=1;i<=10;i++){
		p=vertexHeader;    
	  	while (p != NULL) {
	  		if(p->id==i){
	  			printf("%s \t= ",p->nameOfVertex);
			}
			p=p->next;
	  	}
		for(j=1;j<=10;j++){
			insertLast(i);
			levelPlus(i,0);
			shortestPath(j);
			result=result+dist;
			resetLevel();
			deleteArr();
		}
		printf("%lf\n",((counter-1)/result));
		result=-1;
	}	
}

void betweennessCentrality(){
	graph *p,*q;
	adjOV *ap;
	
	int count=0;
	char *nameSource,*nameTarget;
	
	p=vertexHeader;    
  	while (p != NULL) {
    	count++;
		p=p->next;
  	}
  	
  	int i,j;
  	for(i=1;i<=count;i++){
  		printf("\n----------------------------------------------");
  		insertLast(i);
		levelPlus(i,0);
		for(j=i+1;j<=count;j++){
			printf("\n");
			p=vertexHeader; 
			while (p != NULL) {
				if(p->id==i){
		  			nameSource=p->nameOfVertex;
				}
				p=p->next;
			}
			q=vertexHeader;    
			while (q != NULL) {
				if(q->id==j){
					nameTarget=q->nameOfVertex;
				}
				q=q->next;
			}
			printf("%s\t|%s\t|",nameSource,nameTarget);
			shortestPath(j);
			betweennessFinder(j);
		}
		resetLevel();
		deleteArr();
	}
	
	printf("\n---------------------------------------------------------------------------------------------\n");
	printf("BETWEENNESS CENTRALITY STANDARDIZED SCORE\n\n");
	int stan=((((count-1)*(count-2))/2)*(((count-1)*(count-2))/2));
	
	p=vertexHeader;    
	while (p != NULL) {
    	printf("%s \t= ",p->nameOfVertex);
    	printf("%lf\n",p->betweenness/stan);
		p=p->next;
	}
}

int betweennessFinder(int id){
	graph *p,*q;
	adjOV *ap;
	
	int control=0;
	
	p=vertexHeader;    
  	while (p != NULL) {
  		if(p->id==id){
  			if(p->level-1!=1){
	  			q=vertexHeader;    
	  			while (q != NULL) {
					if(q->level+1==p->level){
						ap=q->AOV;
			  			while(ap!=NULL){
			  				if(ap->id==p->id){
			  					if(control==0){
									q->betweenness+=1;
									printf("%s\t",q->nameOfVertex);
									betweennessFinder(q->id);
								}
								control++;
							}
			  				ap=ap->next;
						}
					}
					q=q->next;
				}
			}else{
				return 0;
			}
		}
		p=p->next;
  	}
}

void resetLevel(){
	graph *p;
	adjOV *ap;
	
	p=vertexHeader;
	int i=0;
	
  	while (p != NULL) {
  		i++;
  		if(p->id==i){
  			p->level=0;
  			ap=p->AOV;
  			while(ap!=NULL){
  				ap->level=0;
  				ap=ap->next;
			}
		}
		p=p->next;
	}
}

void levelPlus(int id,int num){
	graph *p;
	adjOV *ap;
	
	p=vertexHeader;
	int i=0;
	
  	while (p != NULL) {
  		i++;
  		if(p->id==i){
  			if(p->id==id){
  				p->level=num+1;
			}
  			ap=p->AOV;
  			while(ap!=NULL){
  				if(ap->id==id){
  					ap->level=num+1;
				}
  				ap=ap->next;
			}
		}
		p=p->next;
	}
}

int shortestPath(int targetId){
	graph *p,*q;
	adjOV *ap;
	
	int target=targetId;
	
	p=vertexHeader;    
  	while (p != NULL) {
    	if(p->id==arr[0]){
    		ap=p->AOV;
  			while(ap!=NULL){
  				if(ap->level==0){
  					insertLast(ap->id);
  					levelPlus(ap->id,p->level);
				}
  				ap=ap->next;
			}
			deleteFirst();
			if(arr[0]==targetId){				
				q=vertexHeader;    
			  	while (q != NULL) {
			    	if(q->id==targetId){
			    		dist=q->level-1;
					}
					q=q->next;
			  	}
				return dist;
			}else{
				shortestPath(target);
			}
		}
		p=p->next;
  	}	
}

void readVertex(){
	char vertex[SIZE];
	char *pvertex;
	int counter=0;
	FILE *inputFile;	
	
    inputFile=fopen("input.txt","r");
    while (!feof(inputFile)) { 
    	fgets(vertex,SIZE,inputFile);
    	counter++;
    	pvertex=strtok(vertex,"; ");
    	createVertex(counter,pvertex);
	}
	fclose(inputFile);		
}

void readAdjacent(){
	char vertex[SIZE];
	char *sorter;
	char *pvertex;
	char *padj;
	FILE *inputFile;	
	graph *p;
	
	int dataSize=0;
	p=vertexHeader;
	while (p != NULL) {
		dataSize++;
		p=p->next;
	}
	
	int vid,aid;
	
	int counter=1;
	
    inputFile=fopen("input.txt","r");
    while (!feof(inputFile)) { 
    	fgets(vertex,SIZE,inputFile);
    	if(counter!=dataSize){
    		int let=strlen(vertex);
    		vertex[let-1]=0;
		}
    	
    	counter++;
    	
    	sorter=strtok(vertex,"; ");
    	pvertex=sorter;
		while(sorter!=NULL){
		    sorter=strtok(NULL,", ");
		    padj=sorter;
		    if(padj!=NULL){
		    	p=vertexHeader;    
			  	while (p != NULL) {
			    	if(compare(p->nameOfVertex,pvertex)==0){
			    		vid=p->id;
					}
					p=p->next;
			  	}
			  	p=vertexHeader;    
			  	while (p != NULL) {
			    	if(compare(p->nameOfVertex,padj)==0){
						aid=p->id;
					}
					p=p->next;
			  	}
		    	addAdjacent(vid,aid);
			}
		}	
	}
	fclose(inputFile);
}

int compare(char *a,char *b){
	int compare=strcmp(a,b);
	if(compare==-1){
		return -1;
	}else if(compare==1){
		return 1;
	}else if(compare==0){
		return 0;
	}
}

int lengthOfArray(){
	int len=sizeof(arr)/sizeof(arr[0]);
	int i;
	for(i=0;i<=len;i++){
		if(arr[i]==0){
			return i;
		}
	}
}

void insertLast(int data){
	int len=lengthOfArray(arr);
	arr[len]=data;
}

void deleteFirst(){
	int len=lengthOfArray(arr);
	int i;
	for(i=0;i<=len-1;i++){
		arr[i]=arr[i+1];
	}
}

void deleteArr(){
	int len=lengthOfArray(arr);
	int i;
	for(i=0;i<=len-1;i++){
		arr[i]=0;
	}
}








