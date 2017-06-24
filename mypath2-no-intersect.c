#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<assert.h>
int n;
const fix_count=5;
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

struct POS
{
	double left;
	double right;
	double up;
	double down;
};

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

int find_min_next(struct POINT point[],struct POS pos,int current_city)
{
	double min_distance=9999999;
	int min_city=-1;
	int next_city;
	for(next_city=0;next_city<n;next_city++)
	{
		if(distance_matrix[current_city][next_city]<min_distance &&
		 visited[next_city]==0 && point[next_city].x>=pos.left && point[next_city].x<=pos.right 
		 && point[next_city].y>=pos.up && point[next_city].y <=pos.down)
		{

			min_distance=distance_matrix[current_city][next_city];
			min_city=next_city;
		}
	}
	//printf("min city = %d\n",min_city);
	return min_city;
}

int find_leftup_city(struct POINT point[],struct POS pos)
{
	int i,min_city=-1;
	double min_xy=9999999;
	for(i=0;i<n;i++)
	{
		if(point[i].x+point[i].y<min_xy && point[i].x>=pos.left && point[i].x<=pos.right && point[i].y>=pos.up && point[i].y <=pos.down )
		{
			min_xy=point[i].x+point[i].y;
			min_city=i;
		}
	}	
	return min_city;
}

int divide(struct POINT point[],struct POS pos,int current_city,char op,char from,char to,int count)
{
	int i;
	double middle_x=(pos.left+pos.right)/2,middle_y=(pos.up+pos.down)/2;
	//printf("current_city = %d count = %d\n",current_city,count);
	if(count==fix_count)
	{
		int next_city;
		while(1)
		{
			if(current_city==-1)    // first city
			{
				next_city=find_leftup_city(point,pos);
			}
			else
			{
				next_city=find_min_next(point,pos,current_city);
			}
			//printf("next city = %d\n",next_city);
			if(next_city==-1) break;    //no other cities to visit
			
			visited[next_city]=1;
			answer_path[index_path]=next_city;
		
			index_path++;
			
			current_city=next_city;
		}

		return current_city;
	}
	else
	{
		struct POS leftup=pos;
		struct POS leftdown=pos;
		struct POS rightup=pos;
		struct POS rightdown=pos;

		leftup.right=middle_x;
		leftup.down=middle_y;

		rightup.left=middle_x;
		rightup.down=middle_y;

		rightdown.left=middle_x;
		rightdown.up=middle_y;

		leftdown.right=middle_x;
		leftdown.up=middle_y;

		if (op=='u')
		{
		current_city=divide(point,leftup,current_city,'q','a','s',count+1);
		current_city=divide(point,rightup,current_city,'w','a','s',count+1);
		
		current_city=divide(point,rightdown,current_city,'s','w','q',count+1);
		current_city=divide(point,leftdown,current_city,'a','w','q',count+1);
		}
		else if(op=='q')
		{
			if(from=='a' && to=='s')
			{
				current_city=divide(point,leftdown,current_city,'a','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','a','s',count+1);
				current_city=divide(point,rightup,current_city,'w','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','w','s',count+1);
			}
			else if(from=='a' && to=='q')
			{
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
			}
			else if(from=='w' && to=='q')
			{
				
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
			}			
			else if(from=='w' && to=='s')
			{
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);

			}
			else if(from=='s' && to=='w')
			{
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				
			}
			else if(from=='s' && to=='a')
			{
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				
				
			}
			else if(from=='q' && to=='w')
			{
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				
			}
			else// if(from=='q' && to=='a')
			{
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				
			}
		}
		else if(op=='a')
		{
			if(from=='a' && to=='s')
			{
				current_city=divide(point,leftdown,current_city,'a','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','a','s',count+1);
				current_city=divide(point,rightup,current_city,'w','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','w','s',count+1);
			}
			else if(from=='a' && to=='q')
			{
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
			}
			else if(from=='w' && to=='q')
			{
				current_city=divide(point,rightup,current_city,'w','w','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','w','q',count+1);
				current_city=divide(point,leftup,current_city,'q','a','q',count+1);
			}			
			else if(from=='w' && to=='s')
			{
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				
			}
			else if(from=='s' && to=='w')
			{
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				
			}
			else if(from=='s' && to=='a')
			{
				
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
			}
			else if(from=='q' && to=='w')
			{
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				
			}
			else// if(from=='q' && to=='a')
			{
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				
			}
		}
		else if(op=='w')
		{
			if(from=='a' && to=='s')
			{
				current_city=divide(point,leftdown,current_city,'a','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','a','s',count+1);
				current_city=divide(point,rightup,current_city,'w','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','w','s',count+1);
			}
			else if(from=='a' && to=='q')
			{
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
			}
			else if(from=='w' && to=='q')
			{
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
			}			
			else if(from=='w' && to=='s')
			{
				current_city=divide(point,rightup,current_city,'w','w','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','s',count+1);
				current_city=divide(point,leftdown,current_city,'a','w','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','s',count+1);

			}
			else if(from=='s' && to=='w')
			{
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				
			}
			else if(from=='s' && to=='a')
			{
				
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
			}
			else if(from=='q' && to=='w')
			{
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				
			}
			else// if(from=='q' && to=='a')
			{
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				
			}
		}
		else if(op=='s')
		{
			if(from=='a' && to=='s')
			{
				current_city=divide(point,leftdown,current_city,'a','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','a','s',count+1);
				current_city=divide(point,rightup,current_city,'w','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','w','s',count+1);
			}
			else if(from=='a' && to=='q')
			{
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
			}
			else if(from=='w' && to=='q')
			{
				current_city=divide(point,rightup,current_city,'w','w','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','w','q',count+1);
				current_city=divide(point,leftup,current_city,'q','a','q',count+1);
			}			
			else if(from=='w' && to=='s')
			{
				current_city=divide(point,rightup,current_city,'w','w','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','s',count+1);
				current_city=divide(point,leftdown,current_city,'a','w','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','s',count+1);
				
			}
			else if(from=='s' && to=='w')
			{
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				
			}
			else if(from=='s' && to=='a')
			{
				
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
			}
			else if(from=='q' && to=='w')
			{
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				
			}
			else// if(from=='q' && to=='a')
			{
				current_city=divide(point,leftup,current_city,'q','w','q',count+1);
				current_city=divide(point,rightup,current_city,'w','a','q',count+1);
				current_city=divide(point,rightdown,current_city,'s','a','q',count+1);
				current_city=divide(point,leftdown,current_city,'a','a','s',count+1);
				
				
				
			}
		}
		
		return current_city;
	}
}




int detect_intersect(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4)
{
	double x_12=x2-x1,y_12=y2-y1;
	double x_34=x4-x3,y_34=y4-y3;
	double x_23=x3-x2,y_23=y3-y2;
	double x_24=x4-x2,y_24=y4-y2;
	double x_41=x4-x1,y_41=y4-y1;
	double x_42=x4-x2,y_42=y2-y4;

	return ((x_12*y_23-y_12*x_23)*(x_12*y_24-y_12*x_24) <0 && 
		(x_34*y_41-y_34*x_41)*(x_34*y_42-y_34*x_42) <0);
	// if == intersect return 1
}

void swap(int path[],int x,int y)
{
	int temp[y-x+1],temp_index=0;
	int k;
	for(k=x;k<=y;k++)
	{
	//	printf("path[k] = %d\n",path[k]);
		temp[temp_index]=path[k];
		temp_index++;
	}
	
	temp_index--;
	for(k=x;k<=y;k++)
	{
		path[k]=temp[temp_index];
		temp_index--;
	}
}
void improve(struct POINT point[],int path[])
{
	int i,j;
	int count=0;
	for(i=0;i<n-1;i++)
	{
		for(j=i+1;j<n-1;j++)
		{
			
			if(i!=j && detect_intersect(point[path[i]].x,point[path[i]].y,point[path[i+1]].x,point[path[i+1]].y,
				point[path[j]].x,point[path[j]].y,point[path[j+1]].x,point[path[j+1]].y))
			{
				count++;
				swap(path,i+1,j);
			}
		}
	}
	printf("found %d intersect\n",count);
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
	struct POS pos;
	pos.left=min_x;
	pos.right=max_x;
	pos.up=min_y;
	pos.down=max_y;
	divide(point,pos,-1,'u','q','q',1);

	int copy_answer_path[n];
	for(i=0;i<n;i++)
	{
		copy_answer_path[i]=answer_path[i];
	}
	improve(point,copy_answer_path);
	for(i=0;i<n;i++)
	{
		answer_path[i]=copy_answer_path[i];
	}

}
void output(char file_name[])
{
	int i;
	FILE *f=fopen(file_name,"w");
	fprintf(f,"index\n");
	for(i=0;i<n;i++)
	{
		fprintf(f,"%d\n",answer_path[i]);
		if(i>0)
			sum_distance+=distance_matrix[answer_path[i-1]][answer_path[i]];
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
	//check_distance_matrix();

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