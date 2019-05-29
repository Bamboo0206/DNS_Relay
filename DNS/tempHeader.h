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
int case_count = 0;