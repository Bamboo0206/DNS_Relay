#include"MainHeader.h"

using namespace std;


//enum Query_QR { Q_QUERY = 0, Q_RESPONSE = 1 };
//enum WEBADDR_TYPE { ADDR_BLOCKED = 100, ADDR_CACHED, ADDR_NOT_FOUND };
//enum PACKET_TYPE { SELF_CREATED_PACKET, UPPER_DNS_ANSWER };




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
struct dnsHeader//首部                           12B=96b
{
	u_short h_id;//序号，客户程序设置              16b

	bool h_qr;//类型：1，应答；0，查询              1b
	u_short h_opcode;//一般为0：标准查询            4b
	bool h_aa;//权威回答                            1b
	bool h_tc;//截断                                1b
	bool h_rd;//期望递归                            1b
	bool h_ra;//递归可用                            1b
	//保留字段，必须为0                             3b
	char h_rcode;//响应码：0，无差错；3，名称错误   4b

	//无正负号16位整数
	u_short h_qdcount;//QUESTION字段的数目         16b
	u_short h_ancount;//ANSWER字段的数目           16b
	u_short h_nscount;//                           16b
	u_short h_arcount;//                           16b
};
typedef struct dnsHeader DNSHeader;

struct dnsQuery//问题
{
	char* q_qname;//域名，不定长                   x b 
	//NAME LENGTH：域名 各部分长度之和 + 部分数-1 //LABLE COUNT：域名 部分的数目
	//用标签序列表示的域名，每个标签包括长度八位位组+那个八位位组的数量。空标签用零长度八位位组终止
	//可以是奇数个八位位组，不使用填充
	u_short q_qtype;//类型：A（1）                 16b
	u_short q_qclass;//类别：IN（1）               16b
};
typedef struct dnsQuery DNSQuery;

struct dnsResourceRecord//资源记录
{
	char* r_name;//匹配的域名                               
				 //①偏移量表示(一般情况下为0xc00c)，(0c为)请求部分中域名距离开头的偏移量           16b
				 //②元信息标示法，八位位组标签域名（标签的最高两位为00，其余6位->0~63个八位位组）
	u_short r_type;//类型：A（1）                           16b
	u_short r_class;//类别：IN（1）                         16b
	int r_ttl;//生存期                                      32b
	 //32位无正负号整数，规定可以缓存的时间；0，仅能用于正在进行的业务
	u_short r_rdlength;//资源数据长度：A对应IPv4地址— 4    16b
	char* r_rdata;//资源数据；长度不定，IPv4为32bIP地址   由r_rdlength指明
};
typedef struct dnsResourceRecord DNSResourceRecord;
typedef struct dnsResourceRecord DNSAnswer;

struct dnsPacket
{
	PACKET_TYPE p_type;
	Query_QR p_qr;
	DNSHeader* p_header;
	DNSQuery* p_qpointer[50];
	DNSResourceRecord* p_rpointer[50];
};
typedef struct dnsPacket DNSPacket;
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
};
vector<node> IP_ID;

char localdns[] = "dnsrelay.txt";
char Re_Dns[] = "10.3.9.5";
int case_count=0;

void input_file()
{
	int ww = SetParameter(int argc, char* argv[]);
	if (ww == 0) exit(0);

	char buffer[1000];
	FILE* fp = NULL;
	if ((fp = fopen(localdns, "r")) != NULL)
	{
		node w;
		int pos = 0;
		string ip_, id_;
		unsigned int IP_;
		char* result = NULL;
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
int SetParameter(int argc, char* argv[])
{
	int file_flag = 0;
	
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == 'd')
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
int startDNSServer(SOCKET* ret_socket)
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
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	if (case_count == 1) printf("%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);


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
		char* ret_ptr = NULL, * recvbuf = NULL;
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
			new_req->packet = unpackDNSPacket(recvbuf);//拆包
			new_req->served = false;
			new_req->ttl = 600;///？？？？？？？？？？？？？？？？？？？？？？？ 
			iResult = addDNSRequestPool(new_req);
			if (iResult == MAX_REQ + 1)//满 1000 
				cout << "[MainProc]: Too many requests. Ignore current one.";
		


			/*区分包类型并调函数*/
			if (new_req->packet->p_type == UPPER_DNS_ANSWER)
			{
				recvUpperDNS(new_req);
			}
			else//客户端的请求
			{

			}
		
		}


		


	}


	return 0;
}


#include "definition.h"

//拆包，包转结构体
//1，包头：返回包头指针；传入指向包头的指针，指针（指向包头读完的下一位置）
DNSHeader* get_header(const char* p_this_part, char** p_return)
{
	//利用2B~16b 的u_short，分别处理ID/FLAG/4*COUNT
	DNSHeader* header = new DNSHeader;
	DNSPacket* packet_ptr = (DNSPacket*)p_this_part;
	u_short* pointer = (u_short*)packet_ptr;

	//ID  16b~2B
	u_short id = htons(*pointer);//取出前16b，转换为ID
			  //htons根据网络顺序理解
	header->h_id = id;

	//FLAG
	//[htons单位为2B~16b]不能用
	pointer++;//u_short类型指针，每次++即后移16b
	u_short num = htons(*pointer);//取出16b，按位与得到各个标志位的值（掩码）
	header->h_qr = (bool)(num && 0x8000 == 0 ? 0 : 1);//1000 0000 0000 0000
	header->h_opcode = (u_short)((num && 0x7800) >> 11);//0111 1000 0000 0000
								 //unsigned short,补0右移（逻辑）
	header->h_aa = (bool)(num && 0x0400 == 0 ? 0 : 1);//0000 0100 0000 0000
	header->h_tc = (bool)(num && 0x0200 == 0 ? 0 : 1);//0000 0010 0000 0000
	header->h_rd = (bool)(num && 0x0100 == 0 ? 0 : 1);//0000 0001 0000 0000
	header->h_ra = (bool)(num && 0x0080 == 0 ? 0 : 1);//0000 0000 1000 0000
	//保留 3b
	header->h_rcode = (char)(num && 0x000f);//0000 0000 0000 1111

	//QDCOUNT
	pointer++;//u_short类型指针，每次++即后移16b
	header->h_qdcount = (u_short)htons(*pointer);

	//ANCOUNT
	pointer++;
	header->h_ancount = (u_short)htons(*pointer);

	//NSCOUNT
	pointer++;
	header->h_nscount = (u_short)htons(*pointer);

	//ARCOUNT
	pointer++;
	header->h_arcount = (u_short)htons(*pointer);

	//pointer指向下一字段，修改引用指针
	pointer++;
	*p_return = (char*)pointer;//下一部分的起始

	return header;
}
//2，查询字段
DNSQuery* get_query(const char* p_this_part, char** p_return)
{
	DNSQuery* query = new DNSQuery;
	char* pointer = (char*)p_this_part;//按字节依次读取

	//域名
	char* name;//域名，二进制原始 八位位组
	int name_length = 0;
	while (*(pointer + name_length) != '\0')//未读到00结束符（'\0'为0000 0000~0x00）
	{
		name_length++;
	}//name_length为除00外的长度
	name = new char[name_length + 1];
	strcpy(name, pointer);//复制，包括00
	pointer += name_length;
	query->q_qname = name;

	//类型 16b
	pointer++;//pointer本来指向00，后移一位
	u_short* type_ptr = (u_short*)pointer;//16b type
	query->q_qtype = htons(*type_ptr);

	//类别 16b
	(u_short*)pointer++;
	u_short* class_ptr = (u_short*)pointer;//16b
	query->q_qclass = htons(*class_ptr);

	(u_short*)pointer++;
	*p_return = (char*)pointer;//下一部分的起始

	return query;
}
//3.应答字段
DNSAnswer* get_answer(const char* p_this_part, const char* p_packet, char** p_return)
{
	DNSAnswer* answer = new DNSAnswer;
	char* pointer = (char*)p_this_part;

	//域名NAME
	char* name = new char[256];//最大长度为256个字符
	int name_length = 0;
	bool name_jump = false;//标记是否完成跳转
	char* name_pointer = pointer;
	while (1)
	{
		//唯一的跳出循环
		if (*name_pointer == '\0')//如果是0x00
		{
			//复制完成
			name[name_length] = '\0';//结束标志

			//未跳转，则最后要修改pointer指针，使指向该指的位置
			if (name_jump == false)//未跳转
			{
				pointer += name_length;//确定读完域名后的指针位置
			}
			break;//跳出本循环
		}
		//偏移量表示
		//根据规定，使用偏移量表示时，首个标签最高两位全1；否则标签最高两位全0
		if (((*name_pointer) & 0xc0) == 0xc0)//1100: 最高两位全1
		{
			//取16b，查看实际偏移量
			int offset = htons(*((u_short*)name_pointer)) & 0x003f;//0000 0000 0011 1111 ，去掉最高两位11
			name_jump = true;//跳转
			pointer = name_pointer + 2;//偏移量表示，只有16b~2B//确定后边的指针位置
			name_pointer = (char*)p_packet + offset;//域名指针位置
			continue;//结束本次循环，进入下次循环
		}
		//元信息表示：根据偏移量所找位置 或者 本身该处为元信息
		//此时name_pointer所指位置为域名的起始位置！
		if (*name_pointer < 20)//???????不太懂 [0011 1111=63]防止域名过长？
		{
			int part_length = *name_pointer;
			name[name_length] = *name_pointer;//注：存入该数所示的ASCII，不是该数的ASCII
			name_pointer++;
			name_length++;
			for (int i = 0; i < part_length; i++)
			{
				name[name_length] = *name_pointer;
				name_pointer++;
				name_length++;
			}//复制一段域名
		}//->转到循环开始，看是否到达00
	}//while
	answer->r_name = name;

	u_short* p_tmp = (u_short*)pointer;
	//类型
	answer->r_type = htons(*p_tmp);

	//类别
	p_tmp++;
	answer->r_class = htons(*p_tmp);

	//ttl
	p_tmp++;
	answer->r_ttl = htons(*(int*)p_tmp);//32b

	//长度
	p_tmp += 2;//ttl为32b~4B
	answer->r_rdlength = htons(*p_tmp);

	//data
	p_tmp++;
	pointer = (char*)p_tmp;//按字节读
	char* data = new char[answer->r_rdlength + 1];//多搞一位，存'\0'
	memcpy(data, pointer, answer->r_rdlength);
	data[answer->r_rdlength] = '\0';//标识结束
	answer->r_rdata = data;

	*p_return = pointer + answer->r_rdlength;//下一部分的位置：pointer＋资源数据长度
	return answer;
}

//拆包！！！
DNSPacket* packet_to_struct(const char* p_packet)
{
	DNSPacket* packet = new DNSPacket;
	char* pointer = (char*)p_packet;

	char* p_return;//用作各部分的衔接

	//header
	packet->p_header = get_header(pointer, &p_return);
	pointer = p_return;//指向下一部分

	//query 如果有的话
	for (int i = 0; i < packet->p_header->h_qdcount; i++)
	{
		packet->p_qpointer[i] = get_query(pointer, &p_return);
		pointer = p_return;
	}

	//answer 如果有的话
	//???????为啥只复制一个？？
	for (int i = 0; i < packet->p_header->h_ancount; i++)
	{
		packet->p_rpointer[i] = get_answer(pointer, (char*)p_packet, &p_return);
		pointer = p_return;
	}

	//本 包的 类型
	if (packet->p_header->h_qr)
	{
		packet->p_qr = Q_RESPONSE;
	}
	else
	{
		packet->p_qr = Q_QUERY;
	}

	return packet;
}

//原始二进制域名表示，转换为ascii可读
string to_dotted(char* name)
{
	string dotted_name = "";
	while (*name != '\0')
	{
		int part_length = *name - '\0';//该部分长度
		name++;
		for (int i = 0; i < part_length; i++)
		{
			dotted_name += name + '\0';
			name++;
		}
		if (*name != '\0')
		{
			dotted_name += ".";
		}
	}
	return dotted_name;
}
//原始二进制域名->点分十进制
string to_dotted_decimal(char* name, int name_length)
{
	string dotted_decimal = "";
	unsigned char* pointer = (unsigned char*)name;
	for (int i = 0; i < name_length; i++)
	{
		dotted_decimal += pointer[i] - '\0';
		if (i != name_length - 1)
		{
			dotted_decimal += ".";
		}
	}
	return dotted_decimal;
}

//www.baidu.com转八位位组
char* to_char(string name)
{
	int length = name.length();//www.baidu.com
	char* name_pointer = (char*)name.c_str();
	char* pointer = new char[length + 1 + 1];//标签，'\0'
	char* ret = pointer;

	for (int i = 0, j = 0; i < length; )
	{
		int part_length = 0;
		for (; *(name_pointer + i) != '.' && i < length; i++)
		{
			part_length++;
		}
		*pointer = part_length - '\0';
		pointer++;
		for (; j < i; j++)
		{
			*pointer = name_pointer[j] - '\0';
			pointer++;
		}
		i++;
		j++;
	}
	*pointer = '\0';

	return ret;
}


//拼包，各部分转二进制数
//1.包头
char* to_header(const DNSHeader* p_header)
{
	char* header;
	u_short* pointer = (u_short*)new char[12 + 1];//固定包头长度为12B，加一个char='\0'标记结束
	header = (char*)pointer;
	//序号 16b
	*pointer = ntohs((u_short)p_header->h_id);
	pointer++;//后移16b

	//FLAG们1 4 1 1 1 1 3 4
	u_short flag = 0;
	flag += (u_short)(p_header->h_qr << 15);//qr
	flag += (u_short)(p_header->h_opcode << 11);//op_code
	flag += (u_short)(p_header->h_aa << 10);//aa
	flag += (u_short)(p_header->h_tc << 9);//tc
	flag += (u_short)(p_header->h_rd << 8);//rd
	flag += (u_short)(p_header->h_ra << 7);//ra
	//保留3b
	flag += (u_short)(p_header->h_rcode);//rcode
	*pointer = ntohs(flag);
	pointer++;

	//QDCOUNT
	*pointer = ntohs(p_header->h_qdcount);
	pointer++;

	//ANCOUNT
	*pointer = ntohs(p_header->h_ancount);
	pointer++;

	//NSCOUNT
	*pointer = ntohs(p_header->h_nscount);
	pointer++;

	//ARCOUNT
	*pointer = ntohs(p_header->h_arcount);
	pointer++;

	*(char*)pointer = '\0';

	return header;
}
//2.查询
char* to_query(const DNSQuery* p_query)
{
	char* query;
	char* pointer;
	int query_length = 0;//辅助标记开多大内存

	query_length = strlen(p_query->q_qname) + 1;//实际的域名长度，strlen返回值不包括'\0'
	query_length += 2;//type
	query_length += 2;//class
	pointer = new char[query_length + 1];//多一个char存'\0'标志结束
	query = pointer;

	//NAME
	strcpy(pointer, p_query->q_qname);//复制包括'\0'
	pointer += strlen(p_query->q_qname) + 1;

	u_short* tmp = (u_short*)pointer;
	//type
	*tmp = ntohs(p_query->q_qtype);
	tmp++;

	//class
	*tmp = ntohs(p_query->q_qclass);
	tmp++;

	pointer = (char*)tmp;
	*pointer = '\0';

	return query;
}
//3.应答
char* to_answer(DNSAnswer* p_answer)
{
	char* answer;
	char* pointer;
	int answer_length = 0;

	//计算应分配的内存大小
	answer_length = strlen(p_answer->r_name) + 1;//域名，strlen不包括'\0'
	answer_length += 2 + 2 + 4 + 2;//type2 class2 ttl4 length2
	answer_length += p_answer->r_rdlength;//资源数据长度

	pointer = new char[answer_length + 1];//标识结束'\0'
	answer = pointer;

	//NAME
	strcpy(pointer, p_answer->r_name);//包含'\0'
	pointer += strlen(p_answer->r_name) + 1;

	//16b~u_short
	u_short* tmp = (u_short*)pointer;
	//type
	*tmp = ntohs(p_answer->r_type);
	tmp++;
	//class
	*tmp = ntohs(p_answer->r_class);
	tmp++;
	//ttl 4B~int
	*(int*)tmp = ntohs(p_answer->r_ttl);
	tmp += 2;
	//length
	*tmp = ntohs(p_answer->r_rdlength);
	tmp++;

	//资源数据char
	pointer = (char*)tmp;
	memcpy(pointer, p_answer->r_rdata, p_answer->r_rdlength);

	pointer += p_answer->r_rdlength;
	*pointer = '\0';

	return answer;
}

//拼包！！！
char* struct_to_packet(const DNSPacket* p_packet, int* len)
{
	char* packet = new char[1024];
	int packet_length = 0;

	//包头，固定12B
	char* header = to_header(p_packet->p_header);
	memcpy(packet, header, 12 * sizeof(char));
	packet_length += 12;

	//Query
	for (int i = 0; i < p_packet->p_header->h_qdcount; i++)
	{
		char* query = to_query(p_packet->p_qpointer[i]);
		memcpy(packet + packet_length, query, strlen(p_packet->p_qpointer[i]->q_qname) + 1 + 2 + 2);//name'\0',type2,class2
		packet_length += strlen(p_packet->p_qpointer[i]->q_qname) + 1 + 2 + 2;
	}

	//RR(answer)
	if (p_packet->p_qr == Q_RESPONSE && p_packet->p_header->h_ancount > 0)
	{
		for (int i = 0; i < p_packet->p_header->h_ancount; i++)
		{
			char* answer = to_answer(p_packet->p_rpointer[i]);
			memcpy(packet + packet_length, answer, strlen(p_packet->p_rpointer[i]->r_name) + 1 + 2 + 2 + 4 + 2 + p_packet->p_rpointer[i]->r_rdlength);
			//name'\0' type2 class2 ttl4 length2 resource
			packet_length += strlen(p_packet->p_rpointer[i]->r_name) + 1 + 2 + 2 + 4 + 2 + p_packet->p_rpointer[i]->r_rdlength;
		}
	}

	*len = packet_length;
	return packet;
}

string to_dotted_decimal(char* name, int name_length)
{
	string dotted_decimal = "";
	unsigned char* pointer = (unsigned char*)name;
	for (int i = 0; i < name_length; i++)
	{
		dotted_decimal += pointer[i] - '\0';
		if (i != name_length - 1)
		{
			dotted_decimal += ".";
		}
	}
	return dotted_decimal;
}

//查询资源记录//第一个参数传入域名，第二个参数返回找到的node
int query_resource(string DN/*domin name*/, node &IP_address)
{
	for (auto val : IP_ID)
	{
		if (val.id_.find(DN) != string::npos)//string的子串
		{//找到了
			if (val.IP_ != 0)
			{
				IP_address = val;
				return ADDR_CACHED;//找到记录
			}
			else
			{
				return ADDR_BLOCKED;//禁止访问
			}
		}
	}
	return ADDR_NOT_FOUND;//遍历完未找到记录。
}

//添加资源记录
int add_resource(struct node IP_address)
{
	struct node newNode = IP_address;
	IP_ID.push_back(IP_address);
}

void DNSHandleThread(std::string upper_DNS_addr, SOCKET listen_socket, int t_id)
{
	cerr << "Thread created." << std::endl;
	char *sendbuf, *dnsbuf;
	int sendbuflen = DEFAULT_BUFLEN;
	int iResult = 0;
	sendbuf = (char*)malloc(sendbuflen * sizeof(char));
	dnsbuf = (char*)malloc(DEFAULT_BUFLEN * sizeof(char));

	//Create profile for upper DNS
	struct sockaddr_in servaddr;
	ZeroMemory(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(DNS_PORT);
	inet_pton(AF_INET, upper_DNS_addr.c_str(), &servaddr.sin_addr);

	struct sockaddr_in myaddr;
	ZeroMemory(&myaddr, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(TOPORT(t_id));

	// Initialize upper dns server
	SOCKET upper_dns_socket = socket(AF_INET, SOCK_DGRAM, 0);

	::bind(upper_dns_socket, (struct sockaddr*)&myaddr, sizeof(myaddr));

	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	DWORD status;
	// disable  new behavior using
	// IOCTL: SIO_UDP_CONNRESET
	WSAIoctl(upper_dns_socket, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior),
		NULL, 0, &dwBytesReturned, NULL, NULL);

	std::thread return_thread = std::thread(DNSReturnThread, upper_dns_socket, listen_socket, t_id);

	// Initialize cache list;
	while (1)
	{
		DNSRequest *req = NULL;
		while (req == NULL)
		{
			Sleep(20);
			req = getDNSRequest();
		}

		THREADDEBUG << "Got DNSReq" << std::endl;
		DNSPacket *recv_packet = req->packet;

		UINT32 ip_addr = 0;
		int wait_count = 0;
		WEBADDR_TYPE addr_type = getWebAddrType(recv_packet->p_qpointer[0]->q_qname, &ip_addr);
		THREADDEBUG << "Search type finished, type: " << addr_type << std::endl;
		/*if(recv_packet->p_qpointer[0]->q_qtype == 28) //28=AAAA
		{
			addr_type = ADDR_BLOCKED;
			continue;
		}*/

		switch ((int)addr_type)
		{
		case ADDR_BLOCKED:
		case ADDR_CACHED:
			/***********************待改**********************/
			break;

			/*本地未找到，询问上层DNS*/
		
		case ADDR_NOT_FOUND: //本地未找到，询问上层DNS

			//将请求包原样转发（除了改了id）

			int packet_length;
			u_short p_id = req->new_id;
			recv_packet->p_header->h_id = p_id;//修改请求包的id为new_id(与old_id一一对应）
			char* send_string = packDNSPacket(recv_packet, &packet_length);//打包

			//发送请求包
			iResult = sendto(upper_dns_socket, send_string, packet_length, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
			if (iResult == SOCKET_ERROR)
				cerr << "sendto() failed with error code : " << WSAGetLastError() << std::endl;


			break;
		}
	}
}
/*填写包头和answer*/
DNSPacket *getDNSResult(DNSPacket *ori_packet, UINT32 ip_addr, WEBADDR_TYPE addr_type)
{
	DNSPacket *ret_packet = new DNSPacket;
	DNSHeader *ret_header = new DNSHeader;
	DNSQuery *ret_query = ori_packet->p_qpointer[0];
	DNSResponse *ret_response = new DNSResponse;
	u_short ret_id;

	if (addr_type == ADDR_BLOCKED)//禁止访问的域名
	{
		//无answer字段


		//header字段
		ret_header->h_id = ori_packet->p_header->h_id;//id相同
		ret_header->h_qr = 1;//为1：应答包
		ret_header->h_opcode = ori_packet->p_header->h_opcode;
		ret_header->h_aa = 0;//非权威服务器
		ret_header->h_tc = 0;//非截断的
		ret_header->h_rd = ori_packet->p_header->h_rd;//期望递归
		ret_header->h_ra = 1;//递归可用
		ret_header->h_rcode = 3;//！！！禁止访问!指定域名不存在
		ret_header->h_qdcount = 0;
		ret_header->h_ancount = 0;
		ret_header->h_nscount = 0;
		ret_header->h_arcount = 0;

		// 处理指针指向
		ret_packet->p_header = ret_header;

		ret_packet->p_qpointer[0] = NULL;//无请求字段
		ret_packet->p_rpointer[0] = NULL;//无answer字段
		ret_packet->p_qr = Q_RESPONSE;
	}
	else //可以访问该域名
	{
		//建立新的DNS应答
		/*answer字段*/
		//待改：调用函数将域名改为 char类型的
		//ret_response->r_name = ori_packet->p_qpointer[0]->q_qname;//域名字段//?????应该用偏移量C0 0C？？//待改
		ret_response->r_name = to_char(ori_packet->p_qpointer[0]->q_qname);
		ret_response->r_type = 1;//A类型
		ret_response->r_class = ori_packet->p_qpointer[0]->q_qclass;
		ret_response->r_ttl = 0x100;//time to live
		ret_response->r_rdlength = 4;//资源数据长度：type A 是4字节
		ret_response->r_rdata = (char*)malloc(sizeof(UINT32) + 1);//？？？？？需要协商
		*(UINT32*)(ret_response->r_rdata) = htonl(ip_addr);//将一个32位数从主机字节顺序转换成网络字节顺序

		/*header字段*/
		ret_header->h_id = ori_packet->p_header->h_id;
		ret_header->h_qr = 1;//为1：应答包
		ret_header->h_opcode = ori_packet->p_header->h_opcode;
		ret_header->h_aa = 0;//非权威服务器
		ret_header->h_tc = 0;//非截断的
		ret_header->h_rd = 1;//期望递归
		ret_header->h_ra = 1;//递归可用
		ret_header->h_rcode = 0;//可以访问
		ret_header->h_qdcount = 1;//一条请求
		ret_header->h_ancount = 1;//一条回答//？？？？？？
		ret_header->h_nscount = 0;
		ret_header->h_arcount = 0;

		// 处理指针指向
		ret_packet->p_header = ret_header;
		ret_packet->p_qpointer[0] = ret_query;
		ret_packet->p_rpointer[0] = ret_response;
		ret_packet->p_qr = Q_RESPONSE;
	}

	return ret_packet;
}


/*改回旧id转发回去，并且增加资源*/
void recvUpperDNS(DNSRequest* req)
{
	req->packet->p_header->h_id = req->old_id;
	//待改：发送包

	/*增加资源*/
	struct node newNode;

	//访问所有answer
	for (int i = 0; i < req->packet->p_header->h_qdcount; i++)
	{
		if (req->packet->p_qpointer[i]->q_qtype == 1)
		{
			string DN = req->packet->p_qpointer[i]->q_qname;//域名
			newNode.id_ = DN;//域名
			newNode.ip_ = to_dotted_decimal(req->packet->p_qpointer[i]->r_rdata, req->packet->p_qpointer[i]->r_rdlength);//ip地址
			newNode.IP_ = 10;//????
		}
	}
}