#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1

typedef struct cacheLine
{
    int isFull;
    long unsigned int tag;
    long unsigned int time;
} cacheLine;

int cacheSize,blockSize,assocNum,isPrefetch=0,isFifo=0,setNum,cacheMisses=0,cacheHits=0,memoryReads=0,memoryWrites=0;
long unsigned int counter;

cacheLine **cache;

void MakeCache();
void Execute(FILE*);
int logBase2(int);
int isPowerOf2(int);
void Read(long unsigned int,long unsigned int,long unsigned int,long unsigned int);
void ReadP1(long unsigned int,long unsigned int);
void Write(long unsigned int,long unsigned int,long unsigned int,long unsigned int);
void WriteP1(long unsigned int,long unsigned int);

int main(int argc, char *argv[])
{
    if(argv[3][1]=='1')
        isPrefetch=1;
    if(argv[4][0]=='f')
        isFifo=1;
    cacheSize=atoi(argv[1]);
    blockSize=atoi(argv[5]);
    if(!isPowerOf2(cacheSize)||!isPowerOf2(blockSize))
        return -1;
    if(argv[2][0]=='d')
    {
        assocNum=1;
        setNum=cacheSize/blockSize;
    }
    else if(argv[2][5]==':')
    {
        int buffer;
        sscanf(argv[2],"assoc:%d",&buffer);
        assocNum=buffer;
        setNum=cacheSize/blockSize/buffer;
    }
    else
    {
        assocNum=cacheSize/blockSize;
        setNum=1;
    }
    MakeCache();
    FILE *fp=fopen(argv[6],"r");
    Execute(fp);
    fclose(fp);
    printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",memoryReads,memoryWrites,cacheHits,cacheMisses);
    return 0;
}

void MakeCache()
{
    int i,j;
    int num=setNum;
    cache=(cacheLine**)malloc(num*sizeof(cacheLine*));
    for(i=0;i<num;i++)
        cache[i]=(cacheLine*)malloc((assocNum)*sizeof(cacheLine));
    for(i=0;i<num;i++)
    {
        for(j=0;j<assocNum;j++)
            cache[i][j].isFull=0;
    }
}

int isPowerOf2(int x)
{
    int temp=(x||(x&(x-1))==0);
    if(!temp)
        printf("Error: cache size is not a power of 2\n");
    return temp;
}

int logBase2(int arg)
{
    int counter=0;
    while(arg>>=1)
        counter++;
    return counter;
}

void Execute(FILE *fp)
{
    long unsigned int addr;
    char operation[2];
    int b=logBase2(blockSize),s=logBase2(setNum);
    long unsigned int mask;
    long unsigned int indexOfTag,indexOfSet,newIndexOfTag,newIndexOfSet;
    mask=((1<<s)-1);
    while(fscanf(fp," %*x: %s %lx",operation,&addr)==2)
    {
        newIndexOfTag=(addr+blockSize)>>(b+s);
        newIndexOfSet=((addr+blockSize)>>b&mask);
        indexOfSet=(addr>>b)&mask;
        indexOfTag=addr>>(b+s);
        if(operation[0]=='R')
            Read(indexOfTag,indexOfSet,newIndexOfTag,newIndexOfSet);
        else if(operation[0]=='W')
            Write(indexOfTag,indexOfSet,newIndexOfTag,newIndexOfSet);
    }
}

void Read(long unsigned int indexOfTag,long unsigned int indexOfSet,long unsigned int newIndexOfTag,long unsigned int newIndexOfSet)
{
    int i=0,minIndex=0;
    while(TRUE)
    {
        if(!cache[indexOfSet][i].isFull)
        {
            counter++;
            memoryReads++;
            cacheMisses++;
            cache[indexOfSet][i].isFull=1;
            cache[indexOfSet][i].time=counter;
            cache[indexOfSet][i].tag=indexOfTag;
            if(isPrefetch)
                ReadP1(newIndexOfTag,newIndexOfSet);
            break;
        }
        else if(cache[indexOfSet][i].tag==indexOfTag)
        {
            if(!isFifo)
            {
                counter++;
                cache[indexOfSet][i].time=counter;
            }
            cacheHits++;
            break;
        }
        else if(i==assocNum-1)
        {
            int j;
            for(j=0;j<assocNum;j++)
            {
                if(cache[indexOfSet][j].time<=cache[indexOfSet][minIndex].time)
                    minIndex=j;
            }
            counter++;
            memoryReads++;
            cacheMisses++;
            cache[indexOfSet][minIndex].isFull=1;
            cache[indexOfSet][minIndex].time=counter;
            cache[indexOfSet][minIndex].tag=indexOfTag;
            if(isPrefetch)
                ReadP1(newIndexOfTag,newIndexOfSet);
            break;
        }
        i++;
    }
}

void ReadP1(long unsigned int indexOfTag,long unsigned int indexOfSet)
{
    int i=0,minIndex=0;
    while(TRUE)
    {
        if(!cache[indexOfSet][i].isFull)
        {
            counter++;
            memoryReads++;
            cache[indexOfSet][i].isFull=1;
            cache[indexOfSet][i].time=counter;
            cache[indexOfSet][i].tag=indexOfTag;
            break;
        }
        else if(cache[indexOfSet][i].tag==indexOfTag)
            break;
        else if(i==assocNum-1)
        {
            int j;
            for(j=0;j<assocNum;j++)
            {
                if(cache[indexOfSet][j].time<=cache[indexOfSet][minIndex].time)
                    minIndex=j;
            }
            memoryReads++;
            counter++;
            cache[indexOfSet][minIndex].isFull=1;
            cache[indexOfSet][minIndex].time=counter;
            cache[indexOfSet][minIndex].tag=indexOfTag;
            break;
        }
        i++;
    }
}

void Write(long unsigned int indexOfTag,long unsigned int indexOfSet,long unsigned int newIndexOfTag,long unsigned int newIndexOfSet)
{
    int i=0,minIndex=0;
    while(TRUE)
    {
        if(!cache[indexOfSet][i].isFull)
        {
            counter++;
            memoryReads++;
            memoryWrites++;
            cacheMisses++;
            cache[indexOfSet][i].isFull=1;
            cache[indexOfSet][i].time=counter;
            cache[indexOfSet][i].tag=indexOfTag;
            if(isPrefetch)
                WriteP1(newIndexOfTag,newIndexOfSet);
            break;
        }
        else if(cache[indexOfSet][i].tag==indexOfTag)
        {
            if(!isFifo)
            {
                counter++;
                cache[indexOfSet][i].time=counter;
            }
            memoryWrites++;
            cacheHits++;
            break;
        }
        else if(i==assocNum-1)
        {
            int j;
            for(j=0;j<assocNum;j++)
            {
                if(cache[indexOfSet][j].time<=cache[indexOfSet][minIndex].time)
                    minIndex=j;
            }
            memoryReads++;
            memoryWrites++;
            cacheMisses++;
            counter++;
            cache[indexOfSet][minIndex].time=counter;
            cache[indexOfSet][minIndex].isFull=1;
            cache[indexOfSet][minIndex].tag=indexOfTag;
            if(isPrefetch)
                WriteP1(newIndexOfTag, newIndexOfSet);
            break;
        }
        i++;
    }
}

void WriteP1(long unsigned int indexOfTag,long unsigned int indexOfSet)
{
    int i=0,minIndex=0;
    while(TRUE)
    {
        if(!cache[indexOfSet][i].isFull)
        {
            counter++;
            memoryReads++;
            cache[indexOfSet][i].isFull=1;
            cache[indexOfSet][i].time=counter;
            cache[indexOfSet][i].tag=indexOfTag;
            break;
        }
        else if(cache[indexOfSet][i].tag==indexOfTag)
            break;
        else if(i==assocNum-1)
        {
            int j;
            for(j=0;j<assocNum;j++)
            {
                if(cache[indexOfSet][j].time<=cache[indexOfSet][minIndex].time)
                    minIndex=j;
            }
            memoryReads++;
            counter++;
            cache[indexOfSet][minIndex].time=counter;
            cache[indexOfSet][minIndex].isFull=1;
            cache[indexOfSet][minIndex].tag=indexOfTag;
            break;
        }
        i++;
    }
}
