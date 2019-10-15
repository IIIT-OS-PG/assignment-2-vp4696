#include<bits/stdc++.h>
#include<sys/types.h>
#include <sys/stat.h>
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

void* clientThread(void * arg);
void* serverThread(void * arg);
void* uploadFile(void * arg);
void* PeerHvingFile(void * arg);

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

struct argument{
	string s1;
	string s2;
};

string filename="";

void* serverThread(void * arg)
{

	cout<<"server"<<endl;

	struct argument *args = (argument*)arg;

	int sockfd, newsockfd, portno;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	sockfd = socket(AF_INET, SOCK_STREAM,0);

	if(sockfd<0)
	{
		error("Error opening socket!!!");
	}

	// cout<<"IP: "<<args->s1<<endl;
	// cout<<"PORT: "<<args->s2<<endl;

	stringstream g(args->s2); 
	g >> portno;

	char ip[args->s1.size() + 1];
	strcpy(ip, args->s1.c_str());

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
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

		cout<<"Listening..."<<endl;

		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

		char buffer[LENGTH];
		bzero(buffer,sizeof(buffer));
		
		// strcpy(buffer,"want to download or upload?");
		
		// send(newsockfd,buffer,sizeof(buffer),0);

		// bzero(buffer,sizeof(buffer));

		recv(newsockfd,buffer,sizeof(buffer),0);

		// cout<<"Received: "<<buffer<<endl;

		if(strcmp(buffer,"D")==0)
		{
			cout<<"wants to download"<<endl;
			if( pthread_create(&id[i], NULL, clientThread, &newsockfd) != 0 )
         		  printf("Failed to create thread\n");
		}
		else if(strcmp(buffer,"U")==0)
		{
			cout<<"wants to upload"<<endl;
		}
		else if(strcmp(buffer,"list-files")==0)
		{
			bzero(buffer,sizeof(buffer));	
			fstream ff("/home/viviek/Desktop/OS_Assignment_2/Peer2/list_files.txt");
			string ss,temp="";
			while(getline(ff,ss))
			{
				if(temp=="")
					temp=ss+"\t";
				else
					temp=temp+ss+"\t";
			}	
			cout<<temp<<endl;

			strcpy(buffer,temp.c_str());

			send(newsockfd,buffer,sizeof(buffer),0);

		}

		// if( pthread_create(&id[i], NULL, clientThread, &newsockfd) != 0 )
  //          printf("Failed to create thread\n");

       if( i >= 4)
        {
          i = 0;
          while(i < 4)
          {
            pthread_join(id[i++],NULL);
          }
          i = 0;
        }

       // pthread_join(id[i],NULL);
	}

}

void* clientThread(void * arg)
{
	int n;
	char buffer[LENGTH];
	int newsockfd = *((int *)arg);

	if(newsockfd < 0)
	{
		error("Error on accept!!!");
	}

	FILE *f;

	if((f = fopen("/home/viviek/Desktop/OS_Assignment_2/vp1", "wb")) < 0)
	{
		perror("Open error");
	}

	//int s=ftell(f);
	// cout<<"Size: "<<s<<endl;
	
	bzero(buffer,LENGTH);
	while((n=recv(newsockfd,buffer,511,0)))
	{
		//cout<<"rec bytes::"<<n<<endl;
		//int x =fwrite(buffer,sizeof(char),n,f);
		fwrite(buffer,sizeof(char),n,f);
		bzero(buffer,LENGTH);
	}

	
	fclose(f);
	close(newsockfd);
	pthread_exit(NULL);
}




void* PeerHvingFile(void * arg)
{
	string ports=*((string*)arg);

	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	istringstream s1(ports); 		
	string w;
	int zz=0;
	do {  
    	  	s1 >> w; 
  			zz++;
  			
    	} while (s1); 


	istringstream ss1(ports); 	
	string fileN="",word,list="";
	int z=0;
	    do {  
    	    	ss1 >> word; 
  				z++;
  				int port=0;
  				stringstream k(word); 
  				k >> port;

  				// cout<<"PORT: "<<port<<endl;
  				
  				sockfd = socket(AF_INET, SOCK_STREAM, 0);

				if(sockfd<0)
				{
						error("Error opening socket!!!");
				}

				server = gethostbyname("127.0.0.1");

				if(server == NULL)
					cout<<"No such host"<<endl;

				bzero((char *) &serv_addr, sizeof(serv_addr));

				serv_addr.sin_family = AF_INET;

				bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);


  				memset(&serv_addr.sin_port, 0, sizeof(serv_addr.sin_port));
  				serv_addr.sin_port = htons(port);

  				if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
							error("Connection failed!!");

				char buff[LENGTH];
			
				bzero(buff,sizeof(buff));		

				string xv="lookForFile\t"+filename;

				strcpy(buff,xv.c_str());

				send(sockfd,buff,sizeof(buff),0);	

				bzero(buff,sizeof(buff));

				// recv(sockfd,buff,sizeof(buff),0);											
			}while(z!=zz-1);
}		

void* ListFiles(void * arg)
{
	string ports=*((string*)arg);

	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	istringstream s1(ports); 		
	string w;
	int zz=0;
	do {  
    	  	s1 >> w; 
  			zz++;
  			
    	} while (s1); 


	istringstream ss1(ports); 	
	string fileN="",word,list="";
	int z=0;
	    do {  
    	    	ss1 >> word; 
  				z++;
  				int port=0;
  				stringstream k(word); 
  				k >> port;

  				// cout<<"PORT: "<<port<<endl;
  				
  				sockfd = socket(AF_INET, SOCK_STREAM, 0);

				if(sockfd<0)
				{
						error("Error opening socket!!!");
				}

				server = gethostbyname("127.0.0.1");

				if(server == NULL)
					cout<<"No such host"<<endl;

				bzero((char *) &serv_addr, sizeof(serv_addr));

				serv_addr.sin_family = AF_INET;

				bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);


  				memset(&serv_addr.sin_port, 0, sizeof(serv_addr.sin_port));
  				serv_addr.sin_port = htons(port);

  				if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
							error("Connection failed!!");

				char buff[LENGTH];
			
				bzero(buff,sizeof(buff));		
				string xv="list-files";

				strcpy(buff,xv.c_str());

				send(sockfd,buff,sizeof(buff),0);	

				bzero(buff,sizeof(buff));

				recv(sockfd,buff,sizeof(buff),0);											

				fileN = buff;
				// cout<<"B: "<<buff<<endl;
				// cout<<"FN: "<<fileN<<endl;
				if(list=="")
					list=fileN+"\t";
				else
					list=list+fileN+"\t";


				// cout<<buff<<endl;
				close(sockfd);

    	   } while(z!=zz-1); 

    	   // cout<<"L: "<<list<<endl;

    	   char* ret=(char*)malloc(100);
    	   strcpy(ret,list.c_str());
    	   return ret;

}



void* uploadFile(void * arg)
{

	string d="/home/viviek/Desktop/OS_Assignment_2/Peer2";
	string fn;

	string src=*((string*)arg);

	char source[src.length()+1];
	strcpy(source,src.c_str());

	string srr(source);
	int i = srr.find_last_of('/');
	if (i != string::npos)
	    fn = srr.substr(i+1);

	d=d+"/"+fn;
	char destination[d.length()+1];
	strcpy(destination,d.c_str());

	char buffer[LENGTH];

	int sfd,dfd,error,n;

	sfd = open(source, O_RDONLY);
    dfd = open(destination, O_CREAT | O_WRONLY);

    if(dfd == -1)
	{
		cout<<"You have an error opening file!"<<endl;
		return NULL;
	}

    while (1) {
        error = read(sfd, buffer, LENGTH);
        if (error == -1) {
            printf("Error reading file.\n");
            break;
        }
        n = error;

        if (n == 0) break;

        error = write(dfd, buffer, n);
        if (error == -1) {
            printf("Error writing to file.\n");
            break;
        }

     }
     chmod(destination, S_IRWXU);  
     close(sfd);
     close(dfd);

     fstream wFile("/home/viviek/Desktop/OS_Assignment_2/Peer2/list_files.txt",ios_base::out | ios_base::app);
     wFile << fn;
     wFile.close();

}



int main(int argc, char *argv[])
{
	
	pthread_t t1;
	pthread_attr_t sattr;
	pthread_attr_init(&sattr);

	struct argument arrg;

	arrg.s1 = argv[1];
	arrg.s2 = argv[2];

	pthread_create(&t1,&sattr,&serverThread,(void *)&arrg);

	while(1)
	{
			//usleep(900);
			int sockfd, portno_tracker, n;
			string x;
			struct sockaddr_in serv_addr;
			struct hostent *server;

			char buffer[LENGTH];

            // int c=x.compare("logout");
            // cout<<c<<endl;

            fstream fp(argv[3]);
            string getts;
            getline(fp,getts);
            getline(fp,getts);

            istringstream ss(getts); 						//Get port number of tracker-Start
  			string word,portt,stat_tracker;
		    int zz=0;
    		do {  
        			ss >> word; 
  					zz++;
  					if(zz==3)
  						portt=word;
  					if(zz==4){
  						stat_tracker=word;
  					}
    			} while (ss); 

            //string poort=getts.substr(12,5);
    		if(stat_tracker.compare("1")==0);
            	// cout<<"PORT:"<<portt<<endl;
            else{
            	cout<<"No tracker is online"<<endl;
            	exit(1);
            }												//Get port number of tracker-End

            cout<<">> ";
            // cin>>x;
            getline(cin,x);
            // cout<<x<<endl;

            if(x.find("exit")==0)							//SIMPLE EXIT
			{
				exit(1);
			}

			if(x.find("login")==0)							//LOGIN
			{

					stringstream gk(portt); 						//String port_tracker to int
	            	gk >> portno_tracker;
         
					sockfd = socket(AF_INET, SOCK_STREAM, 0);

					if(sockfd<0)
					{
							error("Error opening socket!!!");
					}

					server = gethostbyname("127.0.0.1");

					if(server == NULL)
						cout<<"No such host"<<endl;

					bzero((char *) &serv_addr, sizeof(serv_addr));

					serv_addr.sin_family = AF_INET;

					bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	
					serv_addr.sin_port = htons(portno_tracker);

    				if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
							error("Connection failed!!");

					char buff[LENGTH];
			
					bzero(buff,sizeof(buff));

					x=x+"\t"+arrg.s2;

					strcpy(buff,x.c_str());

					send(sockfd,buff,sizeof(buff),0);	

					bzero(buff,sizeof(buff));

					recv(sockfd,buff,sizeof(buff),0);	

					// cout<<buff<<endl;

					string retV(buff);

					close(sockfd);

				if(retV.compare("Login Successful")==0)
				{
					cout<<"Login Successful"<<endl;
					while(1)
					{

							cout<<">> ";
            				// cin>>x;
            				getline(cin,x);
            				// cout<<x<<endl;

////////////////////////////////////LOGOUT/////////////////////////////////////////////////////////////

            				if(x.compare("logout")==0)
            				{

            						sockfd = socket(AF_INET, SOCK_STREAM, 0);

									if(sockfd<0)
									{
											error("Error opening socket!!!");
									}

									server = gethostbyname("127.0.0.1");

									if(server == NULL)
										cout<<"No such host"<<endl;

									bzero((char *) &serv_addr, sizeof(serv_addr));

									serv_addr.sin_family = AF_INET;

									bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	
									serv_addr.sin_port = htons(portno_tracker);

            						if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
											error("Connection failed!!");

									// char buff[LENGTH];
			
									bzero(buff,sizeof(buff));

									x=x+"\t"+arrg.s2;

									strcpy(buff,x.c_str());

									send(sockfd,buff,sizeof(buff),0);	

									bzero(buff,sizeof(buff));

									recv(sockfd,buff,sizeof(buff),0);		

									cout<<buff<<endl;

									close(sockfd);

            						exit(1);
            				}

////////////////////////////////////LIST-FILES//////////////////////////////////////////////////////////////

            				if(x.find("listfiles")==0)					//LIST FILES
            				{

            						sockfd = socket(AF_INET, SOCK_STREAM, 0);

									if(sockfd<0)
									{
											error("Error opening socket!!!");
									}

									server = gethostbyname("127.0.0.1");

									if(server == NULL)
										cout<<"No such host"<<endl;

									bzero((char *) &serv_addr, sizeof(serv_addr));

									serv_addr.sin_family = AF_INET;

									bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	
									serv_addr.sin_port = htons(portno_tracker);

            						if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
											error("Connection failed!!");

									// char buff[LENGTH];
			
									bzero(buff,sizeof(buff));

									x=x+"\t"+arrg.s2;

									strcpy(buff,x.c_str());

									send(sockfd,buff,sizeof(buff),0);	

									bzero(buff,sizeof(buff));
				
									recv(sockfd,buff,sizeof(buff),0);											

									// cout<<"RB: "<<buff<<endl;

									if(strlen(buff)==0)
											cout<<"No peer is online!!!"<<endl;
									else
									{	
											pthread_t g_f;

											string ports(buff);

											pthread_create(&g_f,NULL,&ListFiles,&ports);

											char* rM;
											pthread_join(g_f,(void**)&rM);

											bzero(buff,sizeof(buff));
											strcpy(buff, rM);
											string s=buff;
											bzero(buff,sizeof(buff));

											// cout<<"List: "<<s<<endl;
											string tp;
											istringstream ss1(s); 	
            								int zz=0;
	    									do {  
    	    										ss1 >> tp; 
  													zz++;
    										} while (ss1); 

    										int z=0;	
    										string filess;
    										istringstream s1(s); 	
	    									do {  
    	    											s1 >> filess;
    	    											cout<<filess<<endl; 
  														z++;
    											} while(z!=zz-1); 

    								}

    								close(sockfd);
				            }

////////////////////////////////////DOWNLOAD-FILES/////////////////////////////////////////////////////////////

				            if(x.find("download")==0)					//DOWNLOAD FILES
            				{

            						sockfd = socket(AF_INET, SOCK_STREAM, 0);

									if(sockfd<0)
									{
											error("Error opening socket!!!");
									}

									server = gethostbyname("127.0.0.1");

									if(server == NULL)
										cout<<"No such host"<<endl;

									bzero((char *) &serv_addr, sizeof(serv_addr));

									serv_addr.sin_family = AF_INET;

									bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	
									serv_addr.sin_port = htons(portno_tracker);

            						if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
											error("Connection failed!!");

									// char buff[LENGTH];
			
									bzero(buff,sizeof(buff));
									// string st(buff);
									istringstream ss2(x);
									int zz1=0;
	    							do {  
    	    								ss2 >> word; 
    	    								// cout<<"W: "<<word;
	  										zz1++;
	  										if(zz1==2)
	  										{
	  											filename=word;
	  											break;
	  										}
	  						 		}while (zz1!=2);

	  						 		x="";
	  						 		x="listfiles";
									x=x+"\t"+arrg.s2;

									strcpy(buff,x.c_str());

									send(sockfd,buff,sizeof(buff),0);	

									bzero(buff,sizeof(buff));
				
									recv(sockfd,buff,sizeof(buff),0);											

									cout<<"RB: "<<buff<<endl;

									if(strlen(buff)==0)
											cout<<"No peer is online!!!"<<endl;
									else
									{	
											pthread_t g_f;

											string ports(buff);

											pthread_create(&g_f,NULL,&PeerHvingFile,&ports);

											char* rM;
											pthread_join(g_f,(void**)&rM);

											bzero(buff,sizeof(buff));
											strcpy(buff, rM);
											string s=buff;
											bzero(buff,sizeof(buff));


									}
							}				

////////////////////////////////////UPLOAD-FILES/////////////////////////////////////////////////////////////

							if(x.find("upload")==0)					//UPLOAD FILES
            				{
            							string getname,word;
            							istringstream uf(x); 	
            							int zz=0;
            							do {  
    	    									uf >> word; 
  												zz++;
  												if(zz==2)
	  												getname = word;				//contains username
    			   							} while (zz!=2); 
            	
    									// cout<<"FilePATH: "<<getname<<endl;

    									pthread_t uplod;
    			
    									pthread_create(&uplod,NULL,&uploadFile,&getname);
    									pthread_join (uplod, NULL);

            				}
		
					}

				}
				else if(retV.compare("Login Failed")==0)
				{
					cout<<"Login failed!!! Invalid credentials"<<endl;
				}	

				close(sockfd);

			}


////////////////////////////////////CREATE_USER/////////////////////////////////////////////////////////////

            if(x.find("create_user")==0)					//CREATE USER
            {
            	

            		stringstream gk(portt); 						//String port_tracker to int
            		gk >> portno_tracker;
            		// cout<<portno_tracker<<endl;

            		sockfd = socket(AF_INET, SOCK_STREAM, 0);

					if(sockfd<0)
					{
							error("Error opening socket!!!");
					}

					server = gethostbyname("127.0.0.1");

					if(server == NULL)
						cout<<"No such host"<<endl;

					bzero((char *) &serv_addr, sizeof(serv_addr));

					serv_addr.sin_family = AF_INET;

					bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	
					serv_addr.sin_port = htons(portno_tracker);

    				if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
							error("Connection failed!!");

					char buff[LENGTH];
			
					bzero(buff,sizeof(buff));

					x=x+"\t"+arrg.s2;

					strcpy(buff,x.c_str());

					send(sockfd,buff,sizeof(buff),0);	

					bzero(buff,sizeof(buff));

					recv(sockfd,buff,sizeof(buff),0);											

					cout<<buff<<endl;

					close(sockfd);

            }
            


			// recv(sockfd,buff,sizeof(buff),0);	

			// cout<<buff<<endl;	

			// cin>>buff;

   //      	send ( sockfd ,buff, sizeof(buff), 0);

        	// bzero(buff,sizeof(buff));

   //      	if(strcmp(buff,"D")==0)
   //      	{

			// 		FILE *f;

			// 		if((f = fopen("/home/viviek/Desktop/OS_Assignment_2/3Idiots", "rb")) == NULL)
			// 		{
			// 				perror("Open error");
			// 		}
	
			// 		bzero(buffer,LENGTH);

			// 		fseek(f,0,SEEK_SET);
			// 		fseek(f,0,SEEK_END);

			// 		int s=ftell(f);
	
			// 		rewind(f);

			// 		//cout<<"fs Size: "<<s<<endl;

			// 		while((n=fread(buffer,sizeof(char),511,f))>0 && s>0)
			// 		{
			// 				//cout<<"read byte:::"<<n<<endl;
			// 				if(send(sockfd,buffer,n,0)<0)
			// 				{
			// 						cout<<"Error sending file!!!"<<endl;
			// 						exit(1);
			// 				}
			// 				//int x=send(sockfd,buffer,n,0);

			// 				//cout<<"ssend byte:::"<<x<<endl;
			// 				bzero(buffer,LENGTH);		
			// 				s=s-n;
			// 				//cout<<"Pos: "<<s<<endl;
			// 		}

			// 		fclose(f);
			// }
			// if(strcmp(buff,"U")==0)
			// {
			// 	continue;
			// 	close(sockfd);
			// }

	}

	pthread_join(t1,NULL);

	return 0;
}