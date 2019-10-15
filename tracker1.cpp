#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<string>
#include <errno.h>
#include<stdlib.h>
#include<unistd.h>
#include <sstream>
#include<netdb.h>
#include <fcntl.h>

using namespace std;

#define LENGTH 1023
#define PORT_NO 50000					//49152-65535

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

struct argument{
	string s1;
	string s2;
};


void* createUser(void * arg);
void* ListFiles(void * arg);
void* UserLogout(void * arg);
void* createUser(void * arg);
void* UserLogin(void * arg);

////////////////////////////////////////////////START-TRACKER/////////////////////////////////////////////////

void start_tracker(char* s1, char* s2)
{

	string s = s1;
	int n = *(int*)s2, flag=0,a;

	string ip = "127.0.0.1";

	fstream f(s);
	string def;
	def="TNo\tIP\tPORT\tStatus\n";
	f << def;
	string st;
	while(getline(f,st)){
		a=f.tellg();
		if(st.find("50000")!=-1){
			flag=1;
			break;
		}
	}
	// cout<<flag<<endl;
	if(flag==0){
		f.close();
		fstream ff(s,ios_base::out | ios_base::app);
		string ta(s2);
		st=ta+"\t"+ip+"\t"+"50000"+"\t"+"1"+"\n";
		ff << st;
		ff.close();

	}
	else if(flag==1){
		f.seekg(a-20);
		string tb(s2);
		st=tb+"\t"+ip+"\t"+"50000"+"\t"+"1"+"\n";
		f << st;
	}

	if(f.is_open())
		f.close();

}

////////////////////////////////////////////////LIST-FILES/////////////////////////////////////////////////

/* Returns port number of active peers */

void* ListFiles(void * arg)
{
	string x=*(string*)arg;
	string word, port_no;

	istringstream ss1(x);
	int zz=0;
	do {  
    	    ss1 >> word; 
  			zz++;
  			if(zz==2)
  				port_no = word;				//contains portnumber of peer who wants fileList
        } while (zz!=2); 

    string st;
    fstream ff("Tracker.txt");
    char* ret=(char*)malloc(50);
    getline(ff,st);
    string port="",tstore="";
    while(getline(ff,st))
    {
    				istringstream ss2(st);
					int zz1=0;
	    			do {  
    	    				ss2 >> word; 
    	    				// cout<<word<<" ";
	  						zz1++;
	  						if(zz1==3 && word.compare(port_no)!=0){
	  							tstore=word;
	  						}
	  						if(zz1==4 && word.compare("1")==0 && tstore!="" && word!=""){
									if(port==""){
	  									port=tstore+"\t";
	  									tstore="";
									}
	  								else{
	  									port=port+tstore+"\t";  							
	  									tstore="";
	  								}
	  						}
	  						word="";
    					} while (zz1!=4);
	  						
    				ss2.clear();
    }
    // cout<<endl<<"P:"<<port<<endl;
    strcpy(ret,port.c_str());
    return ret;

}

////////////////////////////////////////////////USER-LOGIN/////////////////////////////////////////////////

void* UserLogin(void * arg)
{
	string x=*(string*)arg;
	string word,portno_client,user,psswd;
    istringstream ss1(x); 	

    int zz=0;
    do {  
    		ss1 >> word; 
  			zz++;
  			if(zz==2)
  				user=word;
  			if(zz==3)
  				psswd=word;
  			if(zz==4)
  				portno_client=word;

  	    }while(ss1);

  	fstream f("authentication.txt");
	string def;
	def="PEER-PORT\tUSER\tPASSWORD\n";
	f << def;
	string st;
	int flag=0;
	char* ret=(char*)malloc(50);
	while(getline(f,st)){
			istringstream ss(st); 	
    		int zz=0,z=0;
    		do {  
			    		ss >> word;
			    		// cout<<word<<endl;
			    		if(zz==0){
			    			if(portno_client==word)
			    				z++;
			    		}
						if(zz==1){
							if(user==word)
								z++;
						}
						if(zz==2){	
							if(psswd==word)
								z++;
			    		}			
  						zz++;
  	    	   }while(ss);
  	    	   if(z==3)
			   {
			    	flag=1;
			    	break;
			   }			
	}
	if(flag==1)
	{
		// cout<<"**";	
	fstream ft("Tracker.txt");
	string def,st,peer,ip,port;
	int flagg=0,a;
	def="PEER\tIP\tPORT\tSTATUS\n";
	ft << def;
	while(getline(ft,st)){
			istringstream ss(st); 	
    		int zz=0;
    		do {  
			    		ss >> word;
			    		// cout<<word<<endl;
			    		if(zz==0)
			    			peer=word;
						if(zz==1)
							ip=word;
						if(zz==2)	
							port=word;		    			
			    		a=ft.tellg();
			  			if(word.compare(portno_client)==0){
  							flagg=1;
  							break;
  						}	
  						zz++;
  	    	   }while(ss);

  	   		if(flagg==1)
  	    	   	break;
	}

	ft.seekg(a-20);
	st=peer+"\t"+ip+"\t"+port+"\t"+"1"+"\n";
	ft << st;
	f.close();
	ft.close();
	strcpy(ret,"Login Successful");
	}

	if(flag==0)
	{
		strcpy(ret,"Login Failed");
	}
	return ret;
}
////////////////////////////////////////////////USER-LOGOUT/////////////////////////////////////////////////


void* UserLogout(void * arg)
{
	string x=*(string*)arg;
	string word,portno_client;
    istringstream ss1(x); 	

    int zz=0;
    do {  
    		ss1 >> word; 
  			zz++;
  			if(zz==2)
  				portno_client=word;
  	    }while(ss1);

  	    // cout<<portno_client<<endl;

  	fstream f("Tracker.txt");
	string def,st,peer,ip,port;
	int flag=0,a;
	def="PEER\tIP\tPORT\tSTATUS\n";
	f << def;
	while(getline(f,st)){
			istringstream ss(st); 	
    		int zz=0;
    		do {  
			    		ss >> word;
			    		// cout<<word<<endl;
			    		if(zz==0)
			    			peer=word;
						if(zz==1)
							ip=word;
						if(zz==2)	
							port=word;		    			
			    		a=f.tellg();
			  			if(word.compare(portno_client)==0){
  							flag=1;
  							break;
  						}	
  						zz++;
  	    	   }while(ss);

  	   		if(flag==1)
  	    	   	break;
	}
	f.seekg(a-20);
	// cout<<peer<<":::"<<ip<<":::"<<port<<endl;
	st=peer+"\t"+ip+"\t"+port+"\t"+"0"+"\n";
	f << st;

	pthread_exit(NULL);
}

////////////////////////////////////////////////CREATE-USER/////////////////////////////////////////////////

void* createUser(void * arg)
{
			string x=*(string*)arg;
			string username, password,word,portno_client;
            istringstream ss1(x); 	

            int zz=0;
	    	do {  
    	    		ss1 >> word; 
  					zz++;
  					if(zz==2)
  						username = word;				//contains username
  					if(zz==3)
	  					password = word;				//contains password
	  				if(zz==4)
	  					portno_client = word;				//contains client portno
    		} while (zz!=4); 

    		// struct argument arrg1;
    		// arrg1.s1 = username;
    		// arrg1.s1 = password;

    		// cout<<"U: "<<username<<endl;
    		// cout<<"P: "<<password<<endl;
    		fstream f("authentication.txt");				//New peer with login details will be addded in the list
    		int a,flag=0;
    		string def,st;
    		char* ret=(char*)malloc(50);
			
			def="PEER-PORT\tUSER\tPASSWORD";
			f << def;
			while(getline(f,st)){

					istringstream ss2(st);
					int zz1=0;
	    			do {  
    	    					ss2 >> word; 
	  							zz1++;
  								if(word.compare(username)==0){
  									flag=1;
  									break;
  								}
    					} while (zz1!=4);
    				ss2.clear();

  				}
  				// cout<<"F: "<<flag<<endl;
			if(flag==0){
				f.close();
				fstream ff("authentication.txt",ios_base::out | ios_base::app);
				st=portno_client+"\t"+username+"\t"+password+"\n";
				ff << st;
				ff.close();
				// ret="Account created!!!";
				strcpy(ret,"Account created!!!");
			}
			else if(flag==1){
				// ret="Account already created with this credentials!!!";
				strcpy(ret,"Account already created with this credentials!!!");
			}

			if(f.is_open())
				f.close();
			return ret;

}

////////////////////////////////////////////////CREATE-USER/////////////////////////////////////////////////


void* serverThread(void * arg)
{

	// cout<<"server"<<endl;

	int sockfd, newsockfd, portno;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	sockfd = socket(AF_INET, SOCK_STREAM,0);

	if(sockfd<0)
	{
		error("Error opening socket!!!");
	}

	portno = 50000;												//UPDATE IT


	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(portno);


	if(bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
	{
		error("Binding failed!!!");
	}


	listen(sockfd, 10);

	cout<<"Rec"<<endl;

	clilen = sizeof(cli_addr);

	pthread_t id[4];
	int i=0;

	while(1)
	{
		cout<<"Listening......"<<endl;

		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

		char buffer[LENGTH];
		bzero(buffer,sizeof(buffer));

		// int i=0,j=0,k=0;

		recv(newsockfd,buffer,sizeof(buffer),0);

		// cout<<"BR: "<<buffer<<endl;

		string temp(buffer);

		// cout<<"temp rec: "<<temp<<endl;

		

////////////////////////////////////CREATE_USER/////////////////////////////////////////////////////////////		
		
		pthread_t m1;

		if(temp.find("create_user")==0)							//CREATE USER
        {
        	pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&m1,&attr,&createUser,&temp);

			char* retM;
			pthread_join (m1, (void**)&retM);

			bzero(buffer,sizeof(buffer));

			strcpy(buffer, retM);

			send(newsockfd,buffer,sizeof(buffer),0);

        }

////////////////////////////////////USER-LOGIN//////////////////////////////////////////////////////////////

        pthread_t m2;
       
        if(temp.find("logout")==0)									//LOGIN
        {
            pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&m2,&attr,&UserLogout,&temp);

			pthread_join(m2, NULL);

			bzero(buffer,sizeof(buffer));

			string s="Logout Successful";

			strcpy(buffer, s.c_str());

			send(newsockfd,buffer,sizeof(buffer),0);

        }



////////////////////////////////////LIST-FILES//////////////////////////////////////////////////////////////

        pthread_t m3;

        if(temp.find("listfiles")==0)								//LIST FILES
        {
            pthread_attr_t attr;
			pthread_attr_init(&attr);

			pthread_create(&m3,&attr,&ListFiles,&temp);

			char* rM;
			pthread_join(m3, (void**)&rM);

			// cout<<"V:"<<rM<<"**"<<endl;

			bzero(buffer,sizeof(buffer));

			strcpy(buffer, rM);

			send(newsockfd,buffer,sizeof(buffer),0);

        }

////////////////////////////////////USER-LOGIN//////////////////////////////////////////////////////////////

        pthread_t m4;

        if(temp.find("login")==0)								//LOGIN
        {

        	// cout<<"**"<<endl;
            pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&m4,&attr,&UserLogin,&temp);

			char* rM;
			pthread_join(m4, (void**)&rM);

			// cout<<"V:"<<rM<<"**"<<endl;

			bzero(buffer,sizeof(buffer));

			strcpy(buffer, rM);

			send(newsockfd,buffer,sizeof(buffer),0);

        }




        close(newsockfd);
        
        cout<<"Request completed..."<<endl;
	}
	close(sockfd);
}




int main(int argc,char* argv[])
{

	start_tracker(argv[1],argv[2]);

	pthread_t t1;
	pthread_attr_t sattr;
	pthread_attr_init(&sattr);
	pthread_create(&t1,&sattr,&serverThread, (void *)argv[1]);

	while(1)
	{
		string x;
		cin>>x;

		if(x.compare("logout")==0)
        {
        		string s = argv[1];
        		fstream f(s);

        		int n = *(int*)argv[2], flag=0,a;

				string ip = "127.0.0.1";

        		string def;
				def="TNo\tIP\tPORT\tStatus\n";
				f << def;
				string st;
				while(getline(f,st)){
					a=f.tellg();
					if(st.find("50000")!=-1){
							flag=1;
							break;
					}
				}

				if(flag==1){
						f.seekg(a-20);
						string tb(argv[2]);
						st=tb+"\t"+ip+"\t"+"50000"+"\t"+"0"+"\n";
						f << st;
				}
				cout<<"Logout Successful"<<endl;
				f.close();
            	exit(1);
        }
	}

	pthread_join(t1,NULL);


	return 0;
}