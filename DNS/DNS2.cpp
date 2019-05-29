#include"MainHeader.h"
#include<map>

using namespace std;

map<string, UINT32> 


/*处理包（结构体）case3*/

/*改answer和改header*/

/*查资源记录*/


enum Query_QR { Q_QUERY = 0, Q_RESPONSE = 1 };
enum WEBADDR_TYPE { ADDR_BLOCKED = 100, ADDR_CACHED, ADDR_NOT_FOUND };
enum PACKET_TYPE { SELF_CREATED_PACKET, UPPER_DNS_ANSWER };



struct DnsResponse
{
	char* r_name;
	u_short r_type;
	u_short r_class;
	int r_ttl;
	u_short r_rdlength;
	char* r_rdata;
};
typedef struct DnsResponse DNSResponse;

struct DnsPacket
{
	PACKET_TYPE p_type;
	Query_QR p_qr;
	DNSHeader* p_header;
	DNSQuery* p_qpointer[50];
	DNSResponse* p_rpointer[50];
};
typedef struct DnsPacket DNSPacket;

struct DnsRequest
{
	bool served;
	int ttl;
	int old_id;
	int new_id;
	DNSPacket* packet;
	struct sockaddr_in client_addr;
	int client_addr_len;
};
typedef struct DnsRequest DNSRequest;

struct RequestPOOL
{
	bool available;
	DNSRequest* req;
};
typedef struct RequestPOOL ReqPool;

struct host_item_struct
{
	UINT32 ip_addr;
	char* webaddr;
	WEBADDR_TYPE type;
};
typedef struct host_item_struct host_item;

struct cached_item_struct
{
	bool occupied;
	UINT32 ip_addr;
	char* webaddr;
	int ttl;
};
typedef struct cached_item_struct cached_item;
//dns帧 
struct DnsHeader
{
	u_short h_id;
	bool h_qr;
	u_short h_opcode;
	bool h_aa;
	bool h_tc;
	bool h_rd;
	bool h_ra;
	char h_rcode;
	u_short h_qdcount;
	u_short h_ancount;
	u_short h_nscount;
	u_short h_arcount;
};
typedef struct DnsHeader DNSHeader;
//dns回答帧 
struct DnsRequest
{
	bool served;
	int ttl;
	int old_id;
	int new_id;
	DNSPacket* packet;
	struct sockaddr_in client_addr;
	int client_addr_len;
};
typedef struct DnsRequest DNSRequest;
enum WEBADDR_TYPE { ADDR_BLOCKED = 100, ADDR_CACHED, ADDR_NOT_FOUND };


//处理txt 
struct node
{
	string ip_, id_;
	unsigned int IP_;
	int type;
}
vector<node> IP_ID;

char localdns[] = "dnsrelay.txt";
char Re_Dns[] = "10.3.9.5";

void input_file()
{
	int ww = SetParameter(int argc, char *argv[]);
	if (ww == 0) exit(0);

	char buffer[1000];
	FILE *fp = NULL;
	if ((fp = fopen(localdns, "r")) != NULL)
	{
		node w;
		int pos = 0;
		string ip_, id_;
		unsigned int IP_;
		char *result = NULL;
		while (!feof(fp))
		{

			fscanf(fp, "%s", &buffer[pos]);

			result = strtok(buffer[pos], " ");
			ip = result;
			//IP_=
			inet_pton(AF_INET, ip_, &IP_);

			result = strtok(NULL, "\0");
			id = result;

			w.id_ = id;
			w.ip_ = ip;
			w.IP_ = IP_;
			IP_ID.pushback(w);
			if (w.IP_ == 0)
				hosts_list[count]->type = ADDR_BLOCKED;
			else
				hosts_list[count]->type = ADDR_CACHED;

		}
	}
}

int judge_1(int a)
{
	if (a <= 255 && a >= 0)
		return 1;
}
int judge_ip(string ip)
{
	int a, b, c, d;
	string result = strtok(ip, ".");
	a = atio(result.c_str());

	result = strtok(NULL, ".");
	b = (atio(result.c_str()));

	result = strtok(NULL, ".");
	c = (atio(result.c_str()));

	result = strtok(NULL, "\0");
	d = (atio(result.c_str()));

	if (judge_1(a) && judge_1(b) && judge_1(c) && judge_1(d))
		return 1;
	else return 0;

}
/////////////////////////////////////////////////
int SetParameter(int argc, char *argv[])
{
	int file_flag = 0;
	int case_count;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-'&&argv[i][1] == 'd')
		{
			if (argv[i][2] == '1' || argv[i][2] == '2')
			{
				case_count = argv[i][2] - '0';
				continue;
			}
			else
			{
				printf("wrong ip\n");
				return 0;
			}
		}

		if (judge_ip(argv[i]))
		{
			strcpy(Re_Dns, argv[i]);//赋值给上级 
			continue;
		}

		if (!file_flag)
		{
			//确保只读入一次 
			file_flag = 1;

			int file_AR_len = strlen(argv[i]);
			int pos = 0;
			for (int j = 0; j < file_AR_len; j++)
			{
				localdns[pos] = argv[i][j];//全局变量 
				pos++;
				if (argv[i][j] == '\\')
				{
					localdns[cnt] = '\\';
					pos++;
				}
			}
			localdns[pos] = '\0';//完成地址 
			continue;
		}

		return 0;

	}

	return 1;

}









//初始化socket 
int startWSA()
{
	int iResult = 0;
	WSADATA wsaData;
	//套子 
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) { return 255; }
	return 0;
}
int startDNSServer(SOCKET * ret_socket)
{
	int iResult = 0;
	SOCKET ListenSocket = INVALID_SOCKET;

	//建立新socket 
	ListenSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (ListenSocket == INVALID_SOCKET)
		//define INVALID_SOCKET  (SOCKET)(~0) 
	{
		cout << "Socket creation failed with error: " << iResult << endl;
		WSACleanup();
		return 1;
	}

	struct sockaddr_in hints;// 
	hints.sin_family = AF_INET;//设置家族地址 
	hints.sin_addr.s_addr = INADDR_ANY;//设置地址 
	hints.sin_port = htons(DNS_PORT);// 设置端口 

	iResult = bind(ListenSocket, (struct sockaddr*) & hints, sizeof(hints));
	if (iResult == SOCKET_ERROR)
	{

		WSACleanup();
		return 1;
	}

	*ret_socket = ListenSocket;
	return 0;
}
//////////////////////////////////////////////////////////////////// 




cached_item* cached_list[MAX_HOST_ITEM];
ReqPool* request_pool = new ReqPool[MAX_REQ];//二维？？ ？？？？？？？ 
/////////////////////////////////////////////////////////////////////////// 
int main()
{
	int iResult = 0;
	int thread_num = 0;
	char buffer[1000];
	//初始化socket 
	iResult = startWSA();
	//读入文件 
	//input_file();  
	//IP_ID 存储 
	//dnsrelay   
/////////////////////////////////////////////////////////////////////
	// 监听信道 
	SOCKET listen_socket;
	//开始监听 
	iResult = startDNSServer(&listen_socket);
	if (iResult == 1) return 255;  //return -1 表示返回值是255 -1不能很好的结束 

	thread dns_consulting_threads1(DNSHandleThread, "10.3.9.4", listen_socket, 1);
	thread dns_consulting_threads2(DNSHandleThread, "10.3.9.4", listen_socket, 2);
	thread dns_consulting_threads3(DNSHandleThread, "10.3.9.4", listen_socket, 3);
	thread dns_consulting_threads4(DNSHandleThread, "10.3.9.4", listen_socket, 4);


	// 初始化Cache
	for (int i = 0; i < MAX_CACHED_ITEM; i++)
	{
		cached_list[i] = new cached_item;
		cached_list[i]->occupied = false;
		cached_list[i]->ttl = 0;
		cached_list[i]->webaddr = new char[256];
	}

	//初始化dns接受buf DNSRequest Pool
	for (int i = 0; i < MAX_REQ; i++)//300 
	{
		request_pool[i].available = true;
	}

	while (iResult >= 0)
	{
		char* ret_ptr = NULL, *recvbuf = NULL;
		int recvbuflen = DEFAULT_BUFLEN, sendbuflen = DEFAULT_BUFLEN;

		recvbuf = (char*)malloc(recvbuflen * sizeof(char));//申请buf 

		struct sockaddr_in clientaddr;

		int client_addr_len = sizeof(clientaddr);

		memset(recvbuf, '\0', sizeof(recvbuf));//初始化buffer 

		ZeroMemory(&clientaddr, sizeof(clientaddr));// 
		//ZeroMemory是一个宏，只是用于把一段内存的内容置零，内部其实是用 
		//memset实现的，而memset除了对内存进行清零操作，还可以将内存置成别的字符。// 

		DWORD dwBytesReturned = 0;
		DWORD status;
		BOOL bNewBehavior = FALSE;


		//disable  new behavior using
		//IOCTL: SIO_UDP_CONNRESET
		status = WSAIoctl(listen_socket, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);
		//一个套接口句柄 
		//将进行的操作的控制代码
		//输入缓冲区的地址          //输出缓冲区的地址
		//输入缓冲区大小           //输出缓冲区大小
		//输出实际字节数的地址 
		//WSAOVERLAPPED结构的地址
		//一个指向操作结束后调用的例程指针 
	/*调用成功后，WSAIoctl ()函数返回0。否则的话，将返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。 */


		if (SOCKET_ERROR == status)//调用失败 
		{
			DWORD dwErr = WSAGetLastError();//该函数返回上次发生的网络错误 
			if (dwErr == WSAEWOULDBLOCK)//产生这个错误只是说明out buffer已经满了,不代表出错. 
			{
				//上一次网络出错在于buffer满了 
				//结束 
				return 255;
			}
			else { return 255; }
		}

		// Receive DNS Requests

		iResult = recvfrom(listen_socket, recvbuf, recvbuflen, 0, (struct sockaddr*) & clientaddr, &client_addr_len);
		//接受包 
		if (iResult == SOCKET_ERROR) //失败 
		{
			printf("[MainProc]: recvfrom_client() error with code: %d\n", WSAGetLastError());//该函数返回上次发生的网络错误 
			break;
		}
		else
		{
			//动态申请了一个答复数组 
			DNSRequest* new_req = new DNSRequest;
			new_req->client_addr = clientaddr;
			new_req->client_addr_len = client_addr_len;
			new_req->packet = unpackDNSPacket(recvbuf);
			new_req->served = false;
			new_req->ttl = 600;///？？？？？？？？？？？？？？？？？？？？？？？ 
			iResult = addDNSRequestPool(new_req);
			if (iResult == MAX_REQ + 1)//满 1000 
				cout << "[MainProc]: Too many requests. Ignore current one.";
		}

	}


	return 0;
}



