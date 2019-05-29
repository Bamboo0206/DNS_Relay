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
//dns֡ 
struct dnsHeader//�ײ�                           12B=96b
{
	u_short h_id;//��ţ��ͻ���������              16b

	bool h_qr;//���ͣ�1��Ӧ��0����ѯ              1b
	u_short h_opcode;//һ��Ϊ0����׼��ѯ            4b
	bool h_aa;//Ȩ���ش�                            1b
	bool h_tc;//�ض�                                1b
	bool h_rd;//�����ݹ�                            1b
	bool h_ra;//�ݹ����                            1b
	//�����ֶΣ�����Ϊ0                             3b
	char h_rcode;//��Ӧ�룺0���޲��3�����ƴ���   4b

	//��������16λ����
	u_short h_qdcount;//QUESTION�ֶε���Ŀ         16b
	u_short h_ancount;//ANSWER�ֶε���Ŀ           16b
	u_short h_nscount;//                           16b
	u_short h_arcount;//                           16b
};
typedef struct dnsHeader DNSHeader;

struct dnsQuery//����
{
	char* q_qname;//������������                   x b 
	//NAME LENGTH������ �����ֳ���֮�� + ������-1 //LABLE COUNT������ ���ֵ���Ŀ
	//�ñ�ǩ���б�ʾ��������ÿ����ǩ�������Ȱ�λλ��+�Ǹ���λλ����������ձ�ǩ���㳤�Ȱ�λλ����ֹ
	//��������������λλ�飬��ʹ�����
	u_short q_qtype;//���ͣ�A��1��                 16b
	u_short q_qclass;//���IN��1��               16b
};
typedef struct dnsQuery DNSQuery;

struct dnsResourceRecord//��Դ��¼
{
	char* r_name;//ƥ�������                               
				 //��ƫ������ʾ(һ�������Ϊ0xc00c)��(0cΪ)���󲿷����������뿪ͷ��ƫ����           16b
				 //��Ԫ��Ϣ��ʾ������λλ���ǩ��������ǩ�������λΪ00������6λ->0~63����λλ�飩
	u_short r_type;//���ͣ�A��1��                           16b
	u_short r_class;//���IN��1��                         16b
	int r_ttl;//������                                      32b
	 //32λ���������������涨���Ի����ʱ�䣻0�������������ڽ��е�ҵ��
	u_short r_rdlength;//��Դ���ݳ��ȣ�A��ӦIPv4��ַ�� 4    16b
	char* r_rdata;//��Դ���ݣ����Ȳ�����IPv4Ϊ32bIP��ַ   ��r_rdlengthָ��
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
//dns�ش�֡ 
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


//����txt 
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
			strcpy(Re_Dns, argv[i]);//��ֵ���ϼ� 
			continue;
		}

		if (!file_flag)
		{
			//ȷ��ֻ����һ�� 
			file_flag = 1;

			int file_AR_len = strlen(argv[i]);
			int pos = 0;
			for (int j = 0; j < file_AR_len; j++)
			{
				localdns[pos] = argv[i][j];//ȫ�ֱ��� 
				pos++;
				if (argv[i][j] == '\\')
				{
					localdns[cnt] = '\\';
					pos++;
				}
			}
			localdns[pos] = '\0';//��ɵ�ַ 
			continue;
		}

		return 0;

	}

	return 1;

}









//��ʼ��socket 
int startWSA()
{
	int iResult = 0;
	WSADATA wsaData;
	//���� 
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) { return 255; }
	return 0;
}
int startDNSServer(SOCKET* ret_socket)
{
	int iResult = 0;
	SOCKET ListenSocket = INVALID_SOCKET;

	//������socket 
	ListenSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (ListenSocket == INVALID_SOCKET)
		//define INVALID_SOCKET  (SOCKET)(~0) 
	{
		cout << "Socket creation failed with error: " << iResult << endl;
		WSACleanup();
		return 1;
	}

	struct sockaddr_in hints;// 
	hints.sin_family = AF_INET;//���ü����ַ 
	hints.sin_addr.s_addr = INADDR_ANY;//���õ�ַ 
	hints.sin_port = htons(DNS_PORT);// ���ö˿� 

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
ReqPool* request_pool = new ReqPool[MAX_REQ];//��ά���� �������������� 
/////////////////////////////////////////////////////////////////////////// 
int main()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	if (case_count == 1) printf("%4d/%02d/%02d %02d:%02d:%02d.%03d ����%1d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);


	int iResult = 0;
	int thread_num = 0;
	char buffer[1000];
	//��ʼ��socket 
	iResult = startWSA();
	//�����ļ� 
	//input_file();  
	//IP_ID �洢 
	//dnsrelay   
/////////////////////////////////////////////////////////////////////
	// �����ŵ� 
	SOCKET listen_socket;
	//��ʼ���� 
	iResult = startDNSServer(&listen_socket);
	if (iResult == 1) return 255;  //return -1 ��ʾ����ֵ��255 -1���ܺܺõĽ��� 

	thread dns_consulting_threads1(DNSHandleThread, "10.3.9.4", listen_socket, 1);
	thread dns_consulting_threads2(DNSHandleThread, "10.3.9.4", listen_socket, 2);
	thread dns_consulting_threads3(DNSHandleThread, "10.3.9.4", listen_socket, 3);
	thread dns_consulting_threads4(DNSHandleThread, "10.3.9.4", listen_socket, 4);


	// ��ʼ��Cache
	for (int i = 0; i < MAX_CACHED_ITEM; i++)
	{
		cached_list[i] = new cached_item;
		cached_list[i]->occupied = false;
		cached_list[i]->ttl = 0;
		cached_list[i]->webaddr = new char[256];
	}

	//��ʼ��dns����buf DNSRequest Pool
	for (int i = 0; i < MAX_REQ; i++)//300 
	{
		request_pool[i].available = true;
	}

	while (iResult >= 0)
	{
		char* ret_ptr = NULL, * recvbuf = NULL;
		int recvbuflen = DEFAULT_BUFLEN, sendbuflen = DEFAULT_BUFLEN;

		recvbuf = (char*)malloc(recvbuflen * sizeof(char));//����buf 

		struct sockaddr_in clientaddr;

		int client_addr_len = sizeof(clientaddr);

		memset(recvbuf, '\0', sizeof(recvbuf));//��ʼ��buffer 

		ZeroMemory(&clientaddr, sizeof(clientaddr));// 
		//ZeroMemory��һ���ֻ꣬�����ڰ�һ���ڴ���������㣬�ڲ���ʵ���� 
		//memsetʵ�ֵģ���memset���˶��ڴ������������������Խ��ڴ��óɱ���ַ���// 

		DWORD dwBytesReturned = 0;
		DWORD status;
		BOOL bNewBehavior = FALSE;


		//disable  new behavior using
		//IOCTL: SIO_UDP_CONNRESET
		status = WSAIoctl(listen_socket, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);
		//һ���׽ӿھ�� 
		//�����еĲ����Ŀ��ƴ���
		//���뻺�����ĵ�ַ          //����������ĵ�ַ
		//���뻺������С           //�����������С
		//���ʵ���ֽ����ĵ�ַ 
		//WSAOVERLAPPED�ṹ�ĵ�ַ
		//һ��ָ�������������õ�����ָ�� 
	/*���óɹ���WSAIoctl ()��������0������Ļ���������SOCKET_ERROR����Ӧ�ó����ͨ��WSAGetLastError()����ȡ��Ӧ�Ĵ�����롣 */


		if (SOCKET_ERROR == status)//����ʧ�� 
		{
			DWORD dwErr = WSAGetLastError();//�ú��������ϴη������������ 
			if (dwErr == WSAEWOULDBLOCK)//�����������ֻ��˵��out buffer�Ѿ�����,���������. 
			{
				//��һ�������������buffer���� 
				//���� 
				return 255;
			}
			else { return 255; }
		}

		// Receive DNS Requests

		iResult = recvfrom(listen_socket, recvbuf, recvbuflen, 0, (struct sockaddr*) & clientaddr, &client_addr_len);
		//���ܰ� 
		if (iResult == SOCKET_ERROR) //ʧ�� 
		{
			printf("[MainProc]: recvfrom_client() error with code: %d\n", WSAGetLastError());//�ú��������ϴη������������ 
			break;
		}
		else
		{
			//��̬������һ�������� 
			DNSRequest* new_req = new DNSRequest;
			new_req->client_addr = clientaddr;
			new_req->client_addr_len = client_addr_len;
			new_req->packet = unpackDNSPacket(recvbuf);//���
			new_req->served = false;
			new_req->ttl = 600;///���������������������������������������������� 
			iResult = addDNSRequestPool(new_req);
			if (iResult == MAX_REQ + 1)//�� 1000 
				cout << "[MainProc]: Too many requests. Ignore current one.";
		


			/*���ְ����Ͳ�������*/
			if (new_req->packet->p_type == UPPER_DNS_ANSWER)
			{
				recvUpperDNS(new_req);
			}
			else//�ͻ��˵�����
			{

			}
		
		}


		


	}


	return 0;
}


#include "definition.h"

//�������ת�ṹ��
//1����ͷ�����ذ�ͷָ�룻����ָ���ͷ��ָ�룬ָ�루ָ���ͷ�������һλ�ã�
DNSHeader* get_header(const char* p_this_part, char** p_return)
{
	//����2B~16b ��u_short���ֱ���ID/FLAG/4*COUNT
	DNSHeader* header = new DNSHeader;
	DNSPacket* packet_ptr = (DNSPacket*)p_this_part;
	u_short* pointer = (u_short*)packet_ptr;

	//ID  16b~2B
	u_short id = htons(*pointer);//ȡ��ǰ16b��ת��ΪID
			  //htons��������˳�����
	header->h_id = id;

	//FLAG
	//[htons��λΪ2B~16b]������
	pointer++;//u_short����ָ�룬ÿ��++������16b
	u_short num = htons(*pointer);//ȡ��16b����λ��õ�������־λ��ֵ�����룩
	header->h_qr = (bool)(num && 0x8000 == 0 ? 0 : 1);//1000 0000 0000 0000
	header->h_opcode = (u_short)((num && 0x7800) >> 11);//0111 1000 0000 0000
								 //unsigned short,��0���ƣ��߼���
	header->h_aa = (bool)(num && 0x0400 == 0 ? 0 : 1);//0000 0100 0000 0000
	header->h_tc = (bool)(num && 0x0200 == 0 ? 0 : 1);//0000 0010 0000 0000
	header->h_rd = (bool)(num && 0x0100 == 0 ? 0 : 1);//0000 0001 0000 0000
	header->h_ra = (bool)(num && 0x0080 == 0 ? 0 : 1);//0000 0000 1000 0000
	//���� 3b
	header->h_rcode = (char)(num && 0x000f);//0000 0000 0000 1111

	//QDCOUNT
	pointer++;//u_short����ָ�룬ÿ��++������16b
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

	//pointerָ����һ�ֶΣ��޸�����ָ��
	pointer++;
	*p_return = (char*)pointer;//��һ���ֵ���ʼ

	return header;
}
//2����ѯ�ֶ�
DNSQuery* get_query(const char* p_this_part, char** p_return)
{
	DNSQuery* query = new DNSQuery;
	char* pointer = (char*)p_this_part;//���ֽ����ζ�ȡ

	//����
	char* name;//������������ԭʼ ��λλ��
	int name_length = 0;
	while (*(pointer + name_length) != '\0')//δ����00��������'\0'Ϊ0000 0000~0x00��
	{
		name_length++;
	}//name_lengthΪ��00��ĳ���
	name = new char[name_length + 1];
	strcpy(name, pointer);//���ƣ�����00
	pointer += name_length;
	query->q_qname = name;

	//���� 16b
	pointer++;//pointer����ָ��00������һλ
	u_short* type_ptr = (u_short*)pointer;//16b type
	query->q_qtype = htons(*type_ptr);

	//��� 16b
	(u_short*)pointer++;
	u_short* class_ptr = (u_short*)pointer;//16b
	query->q_qclass = htons(*class_ptr);

	(u_short*)pointer++;
	*p_return = (char*)pointer;//��һ���ֵ���ʼ

	return query;
}
//3.Ӧ���ֶ�
DNSAnswer* get_answer(const char* p_this_part, const char* p_packet, char** p_return)
{
	DNSAnswer* answer = new DNSAnswer;
	char* pointer = (char*)p_this_part;

	//����NAME
	char* name = new char[256];//��󳤶�Ϊ256���ַ�
	int name_length = 0;
	bool name_jump = false;//����Ƿ������ת
	char* name_pointer = pointer;
	while (1)
	{
		//Ψһ������ѭ��
		if (*name_pointer == '\0')//�����0x00
		{
			//�������
			name[name_length] = '\0';//������־

			//δ��ת�������Ҫ�޸�pointerָ�룬ʹָ���ָ��λ��
			if (name_jump == false)//δ��ת
			{
				pointer += name_length;//ȷ�������������ָ��λ��
			}
			break;//������ѭ��
		}
		//ƫ������ʾ
		//���ݹ涨��ʹ��ƫ������ʾʱ���׸���ǩ�����λȫ1�������ǩ�����λȫ0
		if (((*name_pointer) & 0xc0) == 0xc0)//1100: �����λȫ1
		{
			//ȡ16b���鿴ʵ��ƫ����
			int offset = htons(*((u_short*)name_pointer)) & 0x003f;//0000 0000 0011 1111 ��ȥ�������λ11
			name_jump = true;//��ת
			pointer = name_pointer + 2;//ƫ������ʾ��ֻ��16b~2B//ȷ����ߵ�ָ��λ��
			name_pointer = (char*)p_packet + offset;//����ָ��λ��
			continue;//��������ѭ���������´�ѭ��
		}
		//Ԫ��Ϣ��ʾ������ƫ��������λ�� ���� ����ô�ΪԪ��Ϣ
		//��ʱname_pointer��ָλ��Ϊ��������ʼλ�ã�
		if (*name_pointer < 20)//???????��̫�� [0011 1111=63]��ֹ����������
		{
			int part_length = *name_pointer;
			name[name_length] = *name_pointer;//ע�����������ʾ��ASCII�����Ǹ�����ASCII
			name_pointer++;
			name_length++;
			for (int i = 0; i < part_length; i++)
			{
				name[name_length] = *name_pointer;
				name_pointer++;
				name_length++;
			}//����һ������
		}//->ת��ѭ����ʼ�����Ƿ񵽴�00
	}//while
	answer->r_name = name;

	u_short* p_tmp = (u_short*)pointer;
	//����
	answer->r_type = htons(*p_tmp);

	//���
	p_tmp++;
	answer->r_class = htons(*p_tmp);

	//ttl
	p_tmp++;
	answer->r_ttl = htons(*(int*)p_tmp);//32b

	//����
	p_tmp += 2;//ttlΪ32b~4B
	answer->r_rdlength = htons(*p_tmp);

	//data
	p_tmp++;
	pointer = (char*)p_tmp;//���ֽڶ�
	char* data = new char[answer->r_rdlength + 1];//���һλ����'\0'
	memcpy(data, pointer, answer->r_rdlength);
	data[answer->r_rdlength] = '\0';//��ʶ����
	answer->r_rdata = data;

	*p_return = pointer + answer->r_rdlength;//��һ���ֵ�λ�ã�pointer����Դ���ݳ���
	return answer;
}

//���������
DNSPacket* packet_to_struct(const char* p_packet)
{
	DNSPacket* packet = new DNSPacket;
	char* pointer = (char*)p_packet;

	char* p_return;//���������ֵ��ν�

	//header
	packet->p_header = get_header(pointer, &p_return);
	pointer = p_return;//ָ����һ����

	//query ����еĻ�
	for (int i = 0; i < packet->p_header->h_qdcount; i++)
	{
		packet->p_qpointer[i] = get_query(pointer, &p_return);
		pointer = p_return;
	}

	//answer ����еĻ�
	//???????Ϊɶֻ����һ������
	for (int i = 0; i < packet->p_header->h_ancount; i++)
	{
		packet->p_rpointer[i] = get_answer(pointer, (char*)p_packet, &p_return);
		pointer = p_return;
	}

	//�� ���� ����
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

//ԭʼ������������ʾ��ת��Ϊascii�ɶ�
string to_dotted(char* name)
{
	string dotted_name = "";
	while (*name != '\0')
	{
		int part_length = *name - '\0';//�ò��ֳ���
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
//ԭʼ����������->���ʮ����
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

//www.baidu.comת��λλ��
char* to_char(string name)
{
	int length = name.length();//www.baidu.com
	char* name_pointer = (char*)name.c_str();
	char* pointer = new char[length + 1 + 1];//��ǩ��'\0'
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


//ƴ����������ת��������
//1.��ͷ
char* to_header(const DNSHeader* p_header)
{
	char* header;
	u_short* pointer = (u_short*)new char[12 + 1];//�̶���ͷ����Ϊ12B����һ��char='\0'��ǽ���
	header = (char*)pointer;
	//��� 16b
	*pointer = ntohs((u_short)p_header->h_id);
	pointer++;//����16b

	//FLAG��1 4 1 1 1 1 3 4
	u_short flag = 0;
	flag += (u_short)(p_header->h_qr << 15);//qr
	flag += (u_short)(p_header->h_opcode << 11);//op_code
	flag += (u_short)(p_header->h_aa << 10);//aa
	flag += (u_short)(p_header->h_tc << 9);//tc
	flag += (u_short)(p_header->h_rd << 8);//rd
	flag += (u_short)(p_header->h_ra << 7);//ra
	//����3b
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
//2.��ѯ
char* to_query(const DNSQuery* p_query)
{
	char* query;
	char* pointer;
	int query_length = 0;//������ǿ�����ڴ�

	query_length = strlen(p_query->q_qname) + 1;//ʵ�ʵ��������ȣ�strlen����ֵ������'\0'
	query_length += 2;//type
	query_length += 2;//class
	pointer = new char[query_length + 1];//��һ��char��'\0'��־����
	query = pointer;

	//NAME
	strcpy(pointer, p_query->q_qname);//���ư���'\0'
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
//3.Ӧ��
char* to_answer(DNSAnswer* p_answer)
{
	char* answer;
	char* pointer;
	int answer_length = 0;

	//����Ӧ������ڴ��С
	answer_length = strlen(p_answer->r_name) + 1;//������strlen������'\0'
	answer_length += 2 + 2 + 4 + 2;//type2 class2 ttl4 length2
	answer_length += p_answer->r_rdlength;//��Դ���ݳ���

	pointer = new char[answer_length + 1];//��ʶ����'\0'
	answer = pointer;

	//NAME
	strcpy(pointer, p_answer->r_name);//����'\0'
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

	//��Դ����char
	pointer = (char*)tmp;
	memcpy(pointer, p_answer->r_rdata, p_answer->r_rdlength);

	pointer += p_answer->r_rdlength;
	*pointer = '\0';

	return answer;
}

//ƴ��������
char* struct_to_packet(const DNSPacket* p_packet, int* len)
{
	char* packet = new char[1024];
	int packet_length = 0;

	//��ͷ���̶�12B
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

//��ѯ��Դ��¼//��һ�����������������ڶ������������ҵ���node
int query_resource(string DN/*domin name*/, node &IP_address)
{
	for (auto val : IP_ID)
	{
		if (val.id_.find(DN) != string::npos)//string���Ӵ�
		{//�ҵ���
			if (val.IP_ != 0)
			{
				IP_address = val;
				return ADDR_CACHED;//�ҵ���¼
			}
			else
			{
				return ADDR_BLOCKED;//��ֹ����
			}
		}
	}
	return ADDR_NOT_FOUND;//������δ�ҵ���¼��
}

//�����Դ��¼
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
			/***********************����**********************/
			break;

			/*����δ�ҵ���ѯ���ϲ�DNS*/
		
		case ADDR_NOT_FOUND: //����δ�ҵ���ѯ���ϲ�DNS

			//�������ԭ��ת�������˸���id��

			int packet_length;
			u_short p_id = req->new_id;
			recv_packet->p_header->h_id = p_id;//�޸��������idΪnew_id(��old_idһһ��Ӧ��
			char* send_string = packDNSPacket(recv_packet, &packet_length);//���

			//���������
			iResult = sendto(upper_dns_socket, send_string, packet_length, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
			if (iResult == SOCKET_ERROR)
				cerr << "sendto() failed with error code : " << WSAGetLastError() << std::endl;


			break;
		}
	}
}
/*��д��ͷ��answer*/
DNSPacket *getDNSResult(DNSPacket *ori_packet, UINT32 ip_addr, WEBADDR_TYPE addr_type)
{
	DNSPacket *ret_packet = new DNSPacket;
	DNSHeader *ret_header = new DNSHeader;
	DNSQuery *ret_query = ori_packet->p_qpointer[0];
	DNSResponse *ret_response = new DNSResponse;
	u_short ret_id;

	if (addr_type == ADDR_BLOCKED)//��ֹ���ʵ�����
	{
		//��answer�ֶ�


		//header�ֶ�
		ret_header->h_id = ori_packet->p_header->h_id;//id��ͬ
		ret_header->h_qr = 1;//Ϊ1��Ӧ���
		ret_header->h_opcode = ori_packet->p_header->h_opcode;
		ret_header->h_aa = 0;//��Ȩ��������
		ret_header->h_tc = 0;//�ǽضϵ�
		ret_header->h_rd = ori_packet->p_header->h_rd;//�����ݹ�
		ret_header->h_ra = 1;//�ݹ����
		ret_header->h_rcode = 3;//��������ֹ����!ָ������������
		ret_header->h_qdcount = 0;
		ret_header->h_ancount = 0;
		ret_header->h_nscount = 0;
		ret_header->h_arcount = 0;

		// ����ָ��ָ��
		ret_packet->p_header = ret_header;

		ret_packet->p_qpointer[0] = NULL;//�������ֶ�
		ret_packet->p_rpointer[0] = NULL;//��answer�ֶ�
		ret_packet->p_qr = Q_RESPONSE;
	}
	else //���Է��ʸ�����
	{
		//�����µ�DNSӦ��
		/*answer�ֶ�*/
		//���ģ����ú�����������Ϊ char���͵�
		//ret_response->r_name = ori_packet->p_qpointer[0]->q_qname;//�����ֶ�//?????Ӧ����ƫ����C0 0C����//����
		ret_response->r_name = to_char(ori_packet->p_qpointer[0]->q_qname);
		ret_response->r_type = 1;//A����
		ret_response->r_class = ori_packet->p_qpointer[0]->q_qclass;
		ret_response->r_ttl = 0x100;//time to live
		ret_response->r_rdlength = 4;//��Դ���ݳ��ȣ�type A ��4�ֽ�
		ret_response->r_rdata = (char*)malloc(sizeof(UINT32) + 1);//������������ҪЭ��
		*(UINT32*)(ret_response->r_rdata) = htonl(ip_addr);//��һ��32λ���������ֽ�˳��ת���������ֽ�˳��

		/*header�ֶ�*/
		ret_header->h_id = ori_packet->p_header->h_id;
		ret_header->h_qr = 1;//Ϊ1��Ӧ���
		ret_header->h_opcode = ori_packet->p_header->h_opcode;
		ret_header->h_aa = 0;//��Ȩ��������
		ret_header->h_tc = 0;//�ǽضϵ�
		ret_header->h_rd = 1;//�����ݹ�
		ret_header->h_ra = 1;//�ݹ����
		ret_header->h_rcode = 0;//���Է���
		ret_header->h_qdcount = 1;//һ������
		ret_header->h_ancount = 1;//һ���ش�//������������
		ret_header->h_nscount = 0;
		ret_header->h_arcount = 0;

		// ����ָ��ָ��
		ret_packet->p_header = ret_header;
		ret_packet->p_qpointer[0] = ret_query;
		ret_packet->p_rpointer[0] = ret_response;
		ret_packet->p_qr = Q_RESPONSE;
	}

	return ret_packet;
}


/*�Ļؾ�idת����ȥ������������Դ*/
void recvUpperDNS(DNSRequest* req)
{
	req->packet->p_header->h_id = req->old_id;
	//���ģ����Ͱ�

	/*������Դ*/
	struct node newNode;

	//��������answer
	for (int i = 0; i < req->packet->p_header->h_qdcount; i++)
	{
		if (req->packet->p_qpointer[i]->q_qtype == 1)
		{
			string DN = req->packet->p_qpointer[i]->q_qname;//����
			newNode.id_ = DN;//����
			newNode.ip_ = to_dotted_decimal(req->packet->p_qpointer[i]->r_rdata, req->packet->p_qpointer[i]->r_rdlength);//ip��ַ
			newNode.IP_ = 10;//????
		}
	}
}