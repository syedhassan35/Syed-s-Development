#include<iostream>
#include<string.h>
#include<stdlib.h>
using namespace std;
class Library{
	public:
	int id;
	char name[100];
	char author[100];
	char student[100];
	int price;
	int pages;	
		
};
int main()
{
	Library lib[20];
	int input=0;
	int count=0;
	
	while(input!=3)
	{
		cout<<"Enter 1 to input detail like id, name, author, student, price and pages."<<endl;
		cout<<"Enter 2 to display details."<<endl;
		cout<<"Enter 3 to quit."<<endl;
		cin>>input;
		
		switch(input)
		{
			case 1:
				start:
				cout<<"Enter book ID: "<<endl;
				cin>>lib[count].id;
				cout<<"Enter Book Name: "<<endl;
				cin>>lib[count].name;
				cout<<"Enter Author Name: "<<endl;
				cin>>lib[count].author;
				cout<<"Enter Student Name: "<<endl;
				cin>>lib[count].student;
				cout<<"Enter Book Price: "<<endl;
				cin>>lib[count].price;
				cout<<"Enter Book Pages: "<<endl;
				cin>>lib[count].pages;
				count++;
				break;
				
			case 2:
			    for(int i=0; i<count; i++){
			    	cout<<"Book ID: "<<lib[i].id<<endl;
			        cout<<"Book Name: "<<lib[i].name<<endl;
			        cout<<"Book Author: "<<lib[i].author<<endl;
			        cout<<"Book Student: "<<lib[i].student<<endl;
			        cout<<"Book Price: "<<lib[i].price<<endl;
			        cout<<"Book Pages: "<<lib[i].pages<<endl;
				}
				break;
			case 3:
				exit(0);
				break;	
			default:
				cout<<"Ja Chala Ja "<<endl;
				goto start;
				
		}
		
	}
	
	
	
	
	
	
	
}
