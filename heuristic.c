#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<assert.h>
int n;
double distance_matrix[2048][2048];
int edge[2048];
int answer_path[2048];
double sum_distance=0;

struct POINT
{
	double x;
	double y;
} ;

double calculate_distance(double x1,double y1,double x2,double y2)
{
	double dx=x1-x2;
	double dy=y1-y2;
	return sqrt(dx*dx+dy*dy);
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

void make_edge(int visited[])
{
	double min_distance;
	int min_city;
	int current_city;
	int next_city;
	for(current_city=0;current_city<n;current_city++)
	{
		min_distance=9999999;
		if(edge[current_city]==-1)
		{
			printf("current_city = %d\n",current_city );
			for(next_city=0;next_city<n;next_city++)
			{
				if(edge[next_city]!=current_city &&visited[next_city]==0 &&  next_city!=current_city && distance_matrix[current_city][next_city]<min_distance)
				{
					min_distance=distance_matrix[current_city][next_city];
					min_city=next_city;
				}
			}
			printf("min city = %d\n", min_city);
			edge[current_city]=min_city;
		}
		
	
	}
}

void delete_same_edge()
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(edge[i]==edge[j])
				edge[j]=-1;
		}
	}
}

void check_edge()
{
	int i;
	printf("check_edge\n");
	for(i=0;i<n;i++)
	{
		printf("%d %d\n",i,edge[i]);
	}
}

void check_visited(int visited[])
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

void tsp()
{
	int current_city=0;
	int visited[n],i;
	int index_path=0;
	int count=0;
	for(i=0;i<n;i++)
	{
		visited[i]=0;
		edge[i]=-1;
	}
	
	while(count!=n)
	{
		
		make_edge(visited);
		delete_same_edge();
		check_edge();
		
		for(i=0;i<n;i++)
		{
			if(visited[edge[i]]==0 && edge[i]!=-1)
			{
				visited[edge[i]]=1;
				count++;
			}
		}
		
		//check_edge();
		check_visited(visited);
		//printf("count = %d\n",count);
	}
	check_edge();
	//make answer_path
	current_city=0;
	answer_path[index_path]=0;
	index_path++;
	while(index_path<n)
	{
		sum_distance+=distance_matrix[current_city][edge[current_city]];
		current_city=edge[current_city];
		answer_path[index_path]=current_city;
		index_path++;
	}
	answer_path[index_path]=-1; // mark end of path 
}

void output(char file_name[])
{
	int i;
	FILE *f=fopen(file_name,"w");
	fprintf(f,"index\n");
	for(i=0;answer_path[i]!=-1;i++)
	{
		fprintf(f,"%d\n",answer_path[i]);
	}
	fclose(f);
	sum_distance+=distance_matrix[answer_path[i-1]][0];
	printf("sum distance = %lf\n",sum_distance);
}

int* input(char file_name[])
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
	printf("finish make distance_matrix\n");
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