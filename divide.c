#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<assert.h>
int n;
const fix_count=10;
double distance_matrix[2048][2048];
int answer_path[2048];
double sum_distance=0;
int index_path=0;
int visited[2048];

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

void divide(struct POINT point[],double left,double right,double up,double down,int count)
{
	int i;
	double middle_x=(left+right)/2,middle_y=(up+down)/2;
	if(count==fix_count)
	{
		for(i=0;i<n;i++)
		{
			if(visited[i]==0 && point[i].x>=left && point[i].x<=right 
				&& point[i].y>=up && point[i].y <=down)
			{
				visited[i]=1;
				answer_path[index_path]=i;
				if(index_path>0) 
				{
					sum_distance+=distance_matrix[answer_path[index_path-1]][answer_path[index_path]];
				}
				index_path++;
			}
		}
	}
	else
	{
		divide(point,left,middle_x,up,middle_y,count+1);
		divide(point,middle_x,right,up,middle_y,count+1);
		
		divide(point,middle_x,right,middle_y,down,count+1);
		divide(point,left,middle_x,middle_y,down,count+1);
	}
}


void tsp(struct POINT point[])
{
	int current_city,next_city;
	double min_x=9999999,max_x=-1,min_y=9999999,max_y=-1;
	int visited[n],i;
	int index_path=0;
	for(i=0;i<n;i++)
	{
		visited[i]=0;
		if(point[i].x<min_x) min_x=point[i].x;
		if(point[i].x>max_x) max_x=point[i].x;
		if(point[i].y<min_y) min_y=point[i].y;
		if(point[i].y>max_y) max_y=point[i].y;
	}
	divide(point,min_x,max_x,min_y,max_y,1);

}


void output(char file_name[])
{
	int i;
	FILE *f=fopen(file_name,"w");
	fprintf(f,"index\n");
	for(i=0;i<n;i++)
	{
		fprintf(f,"%d\n",answer_path[i]);
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

	tsp(point);
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
	for(i=0;i<n;i++)
	{
		printf("%d\n",answer_path[i]);
	}
	output(output_file);
	
	
	return 0;
	
}