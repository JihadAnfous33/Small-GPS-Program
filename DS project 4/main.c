#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

//this is the connection between two cities
struct edge
{
    char source [100];
    char destination [100];
    int cost;
};

//every city has index
struct City
{
    char cityName [100];
    int index;
};

//functions prototypes:
int numOfEdges();
void readCities(struct City city[]);
void readEdges(struct edge data[]);
int CityIndexing(struct City city[] , char CityName[] , int size);
int numOfCities();
void ConstructGraph(struct edge edges[] , struct City cities[] , int Connection[][numOfCities()] , int CitySize , int EdgesSize);
int minDistance(int distance[], bool reached[] , int CitySize);
void dijkstraAlgo(int graph[][numOfCities()], char sourceCity [], struct City cities[] , int CitySize, int distance[], int previous[]);
void printDistanceTable(int distance[] , struct City cities [] , int CitySize , char sourceCity[]);
void printLine();
void printLine2();
void printRoute(int previous[], int j , struct City cities[] , int choice , FILE *output);
void printTerminalFile(int distance[], int previous[] , int citySize , int index , struct City cities[] ,
                       char destination[], int choice , struct edge edges[], int EdgesSize);
void saveInFile(int distance[] , int previous[] , int citySize , int index , struct City cities[] , char destination[], struct edge edges[], int EdgesSize);
void printDistances(int previous[], int j , struct City cities[] , int choice , FILE *output , struct edge edges[] , int EdgesSize);
int CitySize;

int main()
{
    CitySize = numOfCities();//to get the number of cities that are in the file
    int EdgesSize = numOfEdges();//to get the number of connection that are in the file
    struct edge edges [EdgesSize];//array of edge struct it's size is the number of connections
    struct City cities[CitySize];//array of City struct it's size is the number of cities
    int distance[CitySize];//to store in it the shortest distance for each city
    int previous[CitySize];//to store in it the previous city for every city
    int Connection[CitySize][CitySize];//2D array to store in it the distance between two cities
    int choice , source = -1;
    char FileDest [100];//this variable is to pass destination city to saveInFile function
    printf("Welcome to my GPS route.\n");
    printLine();
    printf("Choose from the following menu:\n");
    printLine();
    menu();
    printLine();
    scanf("%d",&choice);
    printLine();
    while(choice>=1 || choice<=4)
    {
        if(choice==1)
        {
             readCities(cities);//fill cities array
             readEdges(edges);//fill edges array
             ConstructGraph(edges , cities , Connection , CitySize , EdgesSize);//construct the graph and connect each vertex to it's adjacent
             for(int i=0; i<EdgesSize; i++)
             {
                 printf("%d) The Distance between %s and %s is: %d km\n",i+1 ,edges[i].source , edges[i].destination , edges[i].cost);
                 printLine();
             }
             menu();
             printLine();
             scanf("%d",&choice);
             printLine();

        }
        else if (choice==2)
        {
            int flag = 0;
            char cityName[100];
            printf("Enter the source city:");
            getchar();
            scanf("%[^\n]",cityName);
            printLine();
            for(int i=0; i<CitySize; i++)//to see if the city exists
            {
                if(strcmp(cities[i].cityName , cityName)==0)
                {
                    flag = 1;
                    source = i;
                }
            }
            if(flag)//if flag is 1 then the city exists
            {

                dijkstraAlgo(Connection , cityName , cities , CitySize , distance , previous);//execute the dijkstra algorithm
            }
            else//the city doesn't exist
            {
                printf("%s is not in the list.\n",cityName);
                printLine();
            }
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
        else if(choice==3)
        {
            int flag = 0;
            char cityName[100];
            if(source==-1)//*if source is -1 then there is no entered source city
            {
                char sourceCity[100];
                printf("You have to enter source city first:");
                getchar();
                scanf("%[^\n]",sourceCity);
                for(int i=0; i<CitySize; i++)//to see if the city exists
                {
                    if(strcmp(cities[i].cityName , sourceCity)==0)
                    {
                        source = i;
                        flag = 1;
                    }
                }
                if(flag)//if flag is 1 then the source city exists
                {
                    dijkstraAlgo(Connection , sourceCity , cities , CitySize , distance , previous);
                    printf("\nEnter destination city:");
                    getchar();
                    scanf("%[^\n]",cityName);
                    strcpy(FileDest , cityName);
                    printLine2();
                    printTerminalFile(distance , previous , CitySize , source , cities , cityName , 1 , edges , EdgesSize);//to print the route between the two cities on terminal
                    printLine2();
                }
                else//the city is not in the list
                {
                    printf("The source city you want is not in the list.\n");
                    printLine();
                }

            }
            else
            {
                printf("Enter destination city:");
                getchar();
                scanf("%[^\n]",cityName);
                strcpy(FileDest , cityName);
                printLine2();
                //to print the route between the two cities on terminal
                printTerminalFile(distance , previous , CitySize , source , cities , cityName , 1 , edges , EdgesSize);
                printLine2();

            }
            menu();
            printLine();
            scanf("%d",&choice);
            printLine();
        }
        else if(choice==4)
        {
            //to save the route between two cities in shortest_path.txt file
            saveInFile(distance , previous , CitySize , source , cities , FileDest, edges , EdgesSize);
            printf("The route has been saved successfully\nHave a safe trip :)");
            exit(0);
        }
        else
        {
            printf("Choose a number from the list only.\n");
            scanf("%d",&choice);
        }
    }
    return 0;
}

//this function is to save the route between two cities in a file
void saveInFile(int distance[] , int previous[] , int citySize , int index , struct City cities[] , char destination[], struct edge edges[] , int EdgesSize)
{
    printTerminalFile(distance , previous , citySize , index , cities , destination , 2 , edges , EdgesSize);//to save the route between two cities in a file

}

void menu()
{
    printf("1. Load cities.\n"
           "2. Enter source city.\n"
           "3. Enter destination city.\n"
           "4. Exit.\n");
}

void readCities(struct City city[])//this function is to read cities from the cities.txt file and fill the cities array
{
    FILE* cities;
    cities = fopen("cities.txt","r");//open cities.txt for reading mode
    char line[300] , cityName1[100] , cityName2[100] , cost[10] , temp[5];
    int length , index = 0 , i , flag = 1;
    while(!feof(cities))// keep going while it's not the end of the file
    {
        fgets(line , 300 , cities);//reads line by line
        length = strlen(line);
        if(line[length-1]=='\n')//to avoid reading new line character
        {
            line[length-1] = '\0';
        }
        //spliting the line
        strcpy(cityName1 , strtok(line , "\t"));
        strcpy(cityName2 , strtok(NULL , "\t"));
        strcpy(cost , strtok(NULL , "\t"));
        for(i=0; i<index+1; i++)//this is to make sure to not repeat the cities
        {
            if(strcmp(city[i].cityName , cityName1)==0)
            {
                flag = 0;
            }
        }
        if(flag)//if flag is 1 this means the city is not in the array
        {
            strcpy(city[index].cityName , cityName1);
            city[index].index = CityIndexing(city , cityName1 , index+1);//this function is to give the array a unique index
            index++;
        }
        flag = 1;
        for(i=0; i<index+1; i++)
        {
            if(strcmp(city[i].cityName , cityName2)==0)
            {
                flag = 0;
            }
        }
        if(flag)
        {
            strcpy(city[index].cityName , cityName2);
            city[index].index = CityIndexing(city , cityName2 , index+1);
            index++;
        }
        flag = 1;
    }
    fclose(cities);//close cities.txt file

}

void readEdges(struct edge data[])//this function is to read edges from cities.txt file and fill the edges array
{
    FILE* cities;
    cities = fopen("cities.txt","r");//open cities.txt file for reading
    char line[300] , cityName1[100] , cityName2[100] , cost[10] , temp[5];
    int length , index = 0;
    while(!feof(cities))//keep going while it's not the end of the file
    {
        fgets(line , 300 , cities);
        length = strlen(line);
        if(line[length-1]=='\n')//to avoid reading new character
        {
            line[length-1] = '\0';
        }
        //splitting line and fill edges array
        strcpy(cityName1 , strtok(line , "\t"));
        strcpy(cityName2 , strtok(NULL , "\t"));
        strcpy(cost , strtok(NULL , "\t"));
        strcpy(data[index].source , cityName1);
        strcpy(data[index].destination , cityName2);
        strcpy(temp , strtok(cost , "k"));
        data[index].cost = atoi(temp);
        index++;
    }
    fclose(cities);//close cities.txt file

}

//construct the graph and fill the Connections array
void ConstructGraph(struct edge edges[], struct City cities[] , int Connection[][numOfCities()] , int CitySize , int EdgesSize)
{
    int counter = 0 , cost = 0;
    for(int i=0; i<CitySize; i++)//giving 0 as initial value for all the elements
    {
        for(int j=0; j<CitySize; j++)
        {
            Connection[i][j] = 0;
        }
    }

    while(counter<EdgesSize)//to trace all the edges array
    {
        int row = -1 , coloumn = -1;
        //getting the indecies of row and coloumn for each element
        for(int i=0; i<CitySize; i++)
        {
            if(strcmp(edges[counter].source , cities[i].cityName)==0)
            {
                row = cities[i].index;
            }
        }

        for(int i=0; i<CitySize; i++)
        {
            if(strcmp(edges[counter].destination , cities[i].cityName)==0)
            {
                coloumn = cities[i].index;
            }
        }
        if(row !=-1 && coloumn !=-1)//this means if the cities are found then connect them with their cost
        {
            Connection[row][coloumn] = edges[counter].cost;
            Connection[coloumn][row] = edges[counter].cost;

        }
        counter++;
    }
}

int numOfCities()//this function is to get the number of the cities that are in the file
{
    int size = numOfEdges();
    struct City cities[size];
    FILE *fCity;
    fCity = fopen("cities.txt","r");
    char line[300] , cityName[100];
    int length , counter = 0 , i , flag = 1 , index = 0;
    while(!feof(fCity))
    {
        fgets(line , 300 , fCity);
        strcpy(cityName , strtok(line , "\t"));
        for(i = 0; i<size; i++)
        {
            if(strcmp(cities[i].cityName , cityName)==0)//if the city is found then the flag is 0
            {
                flag = 0;
            }
        }
        if(flag)//if flag is 1 then the city is not in the array so it can be added
        {
            strcpy(cities[index].cityName,cityName);
            index++;
            counter++;
        }
        flag = 1;
    }
    fclose(fCity);
    return counter;
}

int CityIndexing(struct City city[] , char CityName[] , int size)//this function is to give each city a unique index
{
   int i;
   for(i = 0; i<size; i++)
   {
       if(strcmp(city[i].cityName , CityName)==0)
       {
           return i;
       }
   }
   return i;
}

int numOfEdges()//this function is to get the number of the edges that are in the file
{
    int counter = 0;
    FILE* cities;
    cities = fopen("cities.txt","r");
    char line [1000];
    int length;
    while(!feof(cities))
    {
        fgets(line , 1000 , cities);
        counter++;
    }
    fclose(cities);
    return counter;
}

int minDistance(int distance[], bool reached[] , int CitySize)//to find the minimum distance between two adjacent vertices
{
    int min = INT_MAX; //assigning infinity value initially
    int minIndex;

    for (int i = 0; i < CitySize; i++)
    {
        if (reached[i] == false && distance[i] <= min)//if the vertex is not reached and the distance of it is less than INFINITY
        {
            min = distance[i];
            minIndex = i;

        }
    }
    return minIndex;
}


void dijkstraAlgo(int Connections[][CitySize] , char sourceCity [], struct City cities[] ,int CitySize , int distance[], int previous[])
{

    int src = -1;
    for(int i=0; i<CitySize; i++)//this loop is to get the index of the source city
    {
        if(strcmp(sourceCity , cities[i].cityName)==0)
        {
            src = cities[i].index;
        }
    }
    bool reached[CitySize]; //this boolean array to know if we reached the vertex

    for(int i=0; i<CitySize; i++)//assigning -1 as initial value
    {
        previous[i] = -1;
    }

    for (int i = 0; i < CitySize; i++)//set all distances to all vertices to Infinity and not reached
    {
        distance[i] = INT_MAX;
        reached[i] = false;
    }
    distance[src] = 0; //assigning 0 to the distance of the source city

    for (int counter = 0; counter < CitySize - 1; counter++) //Find shortest path for all vertices except the source city
    {
        int min = minDistance(distance, reached , CitySize);//get the index of the vertex that has the short distance
        reached[min] = true; //assigning true to the vertices that are reached

        for (int i = 0; i < CitySize; i++)
        {
            if (!reached[i] && Connections[min][i] && distance[min] != INT_MAX //update the distance only if the vertex is reached and there is
                 && distance[min] + Connections[min][i] < distance[i])        //connection between it and the final vertex reached before this vertex
            {                                                                //and if the sum of the cost that is stored in distance array is less than
                    distance[i] = distance[min] + Connections[min][i];       //the minimum sum that MinDistance returned
                    previous[i] = min;//to know the route for each node
            }
        }
    }
    printDistanceTable(distance , cities , CitySize , sourceCity);
}

void printRoute(int previous[], int j , struct City cities[] , int choice , FILE *output)//this function is to print the route
{
    if (previous[j] == -1)//if the element is -1 then stop there is nothing in it
    {
        return;
    }
    printRoute(previous, previous[j] , cities , choice , output);
    if(choice==1)//print on the terminal
    {
        printf(" -> %s ", cities[j].cityName);
    }
    else//print in shortest_path.txt file
    {
        fprintf(output , " -> %s ", cities[j].cityName);
    }
}

//print the distances between every two vertices
void printDistances(int previous[], int j , struct City cities[] , int choice , FILE *output , struct edge edges[], int EdgesSize)
{
    if (previous[j] == -1)//if the element is -1 then stop there is nothing in it
    {
        return;
    }
    printDistances(previous, previous[j] , cities , choice , output , edges, EdgesSize);
    if(choice==1)//print on the terminal
    {
        int index;
        for(int i=0; i<EdgesSize; i++)
        {
            //if the source and the destination are in the edges array then get the index of it to get the cost between them
            if(strcmp(cities[previous[j]].cityName , edges[i].source)==0 && strcmp(cities[j].cityName , edges[i].destination)==0 ||
               strcmp(cities[previous[j]].cityName , edges[i].destination)==0 && strcmp(cities[j].cityName , edges[i].source)==0)
            {
                index = i;
            }
        }
        printf("The distance between %s and %s is: %d km\n", cities[previous[j]].cityName , cities[j].cityName , edges[index].cost);
        printLine2();

    }
    else//print in shortest_path.txt file
    {
        int index;
        for(int i=0; i<EdgesSize; i++)
        {
            if(strcmp(cities[previous[j]].cityName , edges[i].source)==0 && strcmp(cities[j].cityName , edges[i].destination)==0 ||
               strcmp(cities[previous[j]].cityName , edges[i].destination)==0 && strcmp(cities[j].cityName , edges[i].source)==0)
            {
                index = i;
            }
        }
        fprintf(output , "The distance between %s and %s is: %d km\n", cities[previous[j]].cityName , cities[j].cityName , edges[index].cost);
        fprintf(output,"---------------------------------------------------------------------"
                        "--------------------------------------------------------\n");
    }
}

//this function is to print the route and the total distance and the distance between every two vertices
void printTerminalFile(int distance[], int previous[] , int citySize , int index , struct City cities[] ,
                       char destination[], int choice , struct edge edges[], int EdgesSize)
{
    int flag = 0;
    int index2;
    for(int i=0; i<citySize; i++)//to see if the destination city is in the list
    {
        if(strcmp(destination , cities[i].cityName)==0)
        {
            flag = 1;
            index2 = i;
        }
    }
    if(choice==1)//print on the terminal
    {
        if(flag)//if flag is 1 then the city exists
        {
            printf("Source -> Destination: %s -> %s\n",cities[index].cityName ,destination);
            printLine2();
            printf("Route: %s ",cities[index].cityName);
            printRoute(previous, index2 , cities , choice , NULL);
            printf("\n");
            printLine2();
            printDistances(previous , index2 , cities , choice , NULL , edges , EdgesSize);
            printf("Total Distance : %d km\n",distance[index2]);
            printLine2();
        }
        else//the city doesn't exists
        {
            printf("The destination you want is not in the list.\n");
        }
    }
    else//print in shortest_path.txt file
    {
        FILE *output;
        output = fopen("shortest_path.txt","w");
        if(flag)
        {
            fprintf(output,"Source -> Destination: %s -> %s\n",cities[index].cityName ,destination);
            fprintf(output , "---------------------------------------------------------------"
                             "--------------------------------------------------------------\n");
            fprintf(output , "Route: %s ",cities[index].cityName);
            printRoute(previous, index2 , cities , choice , output);
            fprintf(output , "\n");
            fprintf(output , "---------------------------------------------------------------"
                             "--------------------------------------------------------------\n");
            printDistances(previous , index2 , cities , choice , output , edges , EdgesSize);
            fprintf(output , "Total Distance : %d km\n",distance[index2]);
            fprintf(output , "---------------------------------------------------------------"
                             "--------------------------------------------------------------\n");
        }
        else
        {
            fprintf(output ,"The destination you want is not in the list.\n");
        }
        fclose(output);
    }
}


void printDistanceTable(int distance[] , struct City cities [] , int CitySize , char sourceCity[])
{
    printf("City \t\t Distance from %s\n",sourceCity);
    printLine();
    int length;
    for (int i = 0; i < CitySize; i++)
    {
         printf("%s\t",cities[i].cityName);
         length = cities[i].cityName;
         if(distance[i]!=INT_MAX)
         {
             if(length > 6)
             {
                 printf("\t\t%d km\n",distance[i]);
             }
             else
             {
                 printf("%d km\n",distance[i]);
             }
         }
         else
         {
             if(length > 6)
             {
                 printf("\t\tCan't be reached\n");
             }
             else
             {
                 printf("Can't be reached \n");
             }
         }

         printLine();

    }
}

void printLine()
{
        printf("----------------------------------------------------------\n");
}

void printLine2()
{
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
}
