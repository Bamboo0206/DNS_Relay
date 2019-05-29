#include"MainHeader.h"

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
int case_count = 0;