#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<assert.h>
int n;
double distance_matrix[2048][2048];
int answer_path[2048];
int answer_path_back[2048];
double sum_distance=0;
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

int find_min_next(int current_city)
{
	double min_distance=9999999;
	int min_city;
	int next_city;
	for(next_city=0;next_city<n;next_city++)
	{
		printf("distance = %lf\n",distance_matrix[current_city][next_city]);
		if(distance_matrix[current_city][next_city]<min_distance && visited[next_city]==0){

			min_distance=distance_matrix[current_city][next_city];
			min_city=next_city;
		}
	}
	sum_distance+=min_distance;
	return min_city;
}

int find_start_city(struct POINT point[])
{
	int i,start_city;
	double min_xy=9999999,xplusy;
	for(i=0;i<n;i++)
	{
		xplusy=point[i].x+point[i].y;
		if(xplusy<min_xy)
		{
			start_city=i;
			min_xy=xplusy;
		}
	}
	return start_city;
}

void tsp(int start_city)
{
	int current_city,next_city;
	int current_city_back,next_city_back;
	int i;
	int index_path=0;
	
	for(i=0;i<n;i++)
	{
		visited[i]=0;
	}
	visited[start_city]=1;  //Marked that insert in Set 
	next_city=start_city;
	next_city_back=start_city;
	answer_path[0]=start_city;
	index_path+=1;
	while(index_path<=n/2)
	{
		current_city=next_city;
		current_city_back=next_city_back;
		next_city=find_min_next(current_city);
		visited[next_city]=1;
		answer_path[index_path]=next_city;
		if(index_path*2==n)      // last loop if n is even
		{
			index_path+=1;
			break;
		}
		next_city_back=find_min_next(current_city_back);
		visited[next_city_back]=1;
		answer_path_back[index_path]=next_city_back;
		printf("next city = %d\n",next_city);
		printf("next city back= %d\n",next_city_back);
		index_path+=1;
	}
	sum_distance+=distance_matrix[next_city][next_city_back];
	if(n%2==0) i=index_path-2;
	else i=index_path-1;
	for(;i>=1;i--,index_path++)   //answer_path_back[0]=NULL
	{
		answer_path[index_path]=answer_path_back[i];
	}
	answer_path[index_path]=-1;  // Marked end

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

	printf("sum distance = %lf\n",sum_distance);
}

void input(char file_name[])
{
	int ret;
	int start_city;
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
	start_city=find_start_city(point);
	tsp(start_city);
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