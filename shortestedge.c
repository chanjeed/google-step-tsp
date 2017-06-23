#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<assert.h>
int n;
double distance_matrix[2048][2048];
int visited[2048];
int answer_path[2048];
double sum_distance=0;

struct POINT
{
	double x;
	double y;
} ;

struct EDGE
{
	int from;
	int to;
	double distance;
	int use;
}edge[2048*2047/2];

double calculate_distance(double x1,double y1,double x2,double y2)
{
	double dx=x1-x2;
	double dy=y1-y2;
	return sqrt(dx*dx+dy*dy);
}

void make_edge(struct POINT point[])
{
	int i,j,index=0;
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(i!=j)
			{
				edge[index].use=0;
				edge[index].from=i;
				edge[index].to=j;
				edge[index].distance=calculate_distance(point[i].x,point[i].y,point[j].x,point[j].y);
				index++;
			}
		}		

	}
}

void check_distance_matrix()
{
	int i,j;
	for (i = 0; i < n; i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%lf ",distance_matrix[i][j]);
		}
		printf("\n");
	}
}

void make_distance_matrix(struct POINT point[])
{
	int i,j;
	
	
	
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			distance_matrix[i][j]=calculate_distance(point[i].x,point[i].y,point[j].x,point[j].y);
		}
	}
}

void check_visited()
{
	int i;
	printf("visited\n");
	for(i=0;i<n;i++)
	{
		if(visited[i]==1)
			printf("%d, ",i);
	}
	printf("\n");
}

void pick_edge(int visited_count)
{
	int i;
	for(i=0;i<n*(n-1);i++)
	{
		if(visited[edge[i].from]<=visited_count && visited[edge[i].to]<=visited_count && edge[i].use==0)
		{
			edge[i].use=1;
			visited[edge[i].from]++;
			visited[edge[i].to]++;
		}
	}
}

void swap_edge(int p, int q)
{
	int temp_from=edge[p].from,temp_to=edge[p].to;
    double temp_distance = edge[p].distance;
    edge[p].distance=edge[q].distance;
    edge[p].from=edge[q].from;
    edge[p].to=edge[q].to;

    edge[q].distance=temp_distance;
    edge[q].from=temp_from;
    edge[q].to=temp_to;
}
 
/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (int low, int high)
{
    double pivot = edge[high].distance;    // pivot
    int i = (low - 1);  // Index of smaller element
    int j;
    for (j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (edge[j].distance <= pivot)
        {
            i++;    // increment index of smaller element
            swap_edge(i,j);
        }
    }
    swap_edge(i+1,high);
    return (i + 1);
}
 
void sort_edge(int low, int high)
{
    if (low < high)
    {
        int pi = partition(low, high);
 
        // Separately sort elements before
        // partition and after partition
        sort_edge(low, pi - 1);
        sort_edge(pi + 1, high);
    }
}
 

void make_answer()
{
	int i;
	int start;
	int index_path=0;
	if(n%2==1)
	{
		for(i=0;i<n;i++)
		{
			if(visited[i]==0) 
			{
				start=i;
				break;
			}
		}
		answer_path[index_path]=start;
		index_path++;
	}
	for(i=0;i<n*(n-1);i++)
	{
		if(edge[i].use==1)
		{
			answer_path[index_path]=edge[i].from;
			index_path++;
			answer_path[index_path]=edge[i].to;
			index_path++;			
		}
	}
	answer_path[index_path]=-1;
}

void tsp()
{
	int current_city=0;
	int i;
	int index_path=0;
	int count=0;
	for(i=0;i<n;i++)
	{
		visited[i]=0;
		
	}
	pick_edge(0);
	make_answer();
	
}

void check_edge()
{
	int i;
	printf("CHECK EDGE\n");
	for(i=0;i<n*(n-1)/2;i++)
	{
		printf("from %d to %d = %lf\n",edge[i].from,edge[i].to,edge[i].distance);
	}
}

void output(char file_name[])
{
	int i;
	int previous_city=answer_path[0];
	FILE *f=fopen(file_name,"w");
	fprintf(f,"index\n");
	for(i=0;answer_path[i]!=-1;i++)
	{
		fprintf(f,"%d\n",answer_path[i]);
		sum_distance+=distance_matrix[previous_city][answer_path[i]];
		previous_city=answer_path[i];
	}
	fclose(f);
	sum_distance+=distance_matrix[answer_path[i-1]][answer_path[0]];
	printf("sum distance = %lf\n",sum_distance);
}

void input(char file_name[])
{
	int ret;
	double x,y;
	int i=0;
	FILE *f=fopen(file_name,"r");
	printf("open file\n");
	struct POINT point[2048];
	char buffer[4];
     fscanf(f,"%s\n",buffer);
     printf("%s\n",buffer);
   
	printf("finish first fscanf\n");
	char line[100];
	while(1)
	{	
		ret=fscanf(f,"%lf,%lf\n",&x,&y);
		printf("%d\n",ret);
		if(ret==-1) break;  //end of file
		printf("%lf,%lf\n",x,y);
		point[i].x=x;
		point[i].y=y;
		i++;
	}
	n=i;
	fclose(f);
	printf("finish read file\n");
	make_distance_matrix(point);
	make_edge(point);
	sort_edge(0,n*(n-1)/2-1);
	check_edge();
	check_distance_matrix();
	tsp();
	printf("finish tsp()\n");
	
}

int main(){
	
	char input_file[50],output_file[50];
	int i;
	printf("input file:");
	scanf("%s",input_file);
	printf("scanf\n");
	input(input_file);
	printf("output file:");
	scanf("%s",output_file);	
	for(i=0;answer_path[i]!=-1;i++)
	{
	printf("%d\n",answer_path[i]);
	}
	output(output_file);
	
	
	return 0;
	
}