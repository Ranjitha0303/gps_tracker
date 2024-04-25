#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

#define NWY_TCP_SOCKET_MAX 5
#define NWY_UDP_SOCKET_MAX 5
#define NWY_BUFF_MAX 128
#define NWY_THREAD_STACK_SIZE  (3*1024)

typedef enum
{
    NWY_CUSTOM_IP_TYPE_OR_DNS_NONE = -1,
    NWY_CUSTOM_IP_TYPE_OR_DNS_IPV4 = 0,
    NWY_CUSTOM_IP_TYPE_OR_DNS_IPV6 = 1,
    NWY_CUSTOM_IP_TYPE_OR_DNS_DNS = 2
} nwy_ip_type_or_dns_enum;
//ftp
typedef struct dataRecvContext
{
    unsigned size;
    unsigned pos;
    char data[0];
} dataRecvContext_t;
typedef struct nwy_file_ftp_info_s
{
    int is_vaild;
    char filename[256];
    int pos;
    //int length;
    //int file_size;
} nwy_file_ftp_info_s;

//socket
static int s_nwy_cli_tcp_fd = 0;
static int s_nwy_cli_udp_fd = 0;
static int s_nwy_tcp_connect_flag = 0;
static int s_nwy_udp_connect_flag = 0;
static char g_udp_manual_close = 0;
nwy_osi_thread_t s_nwy_tcp_recv_thread = NULL;
nwy_osi_thread_t s_nwy_udp_recv_thread = NULL;
char s_nwy_tcp_recv_thread_stack[NWY_THREAD_STACK_SIZE] = {0};
char s_nwy_udp_recv_thread_stack[NWY_THREAD_STACK_SIZE] = {0};
static struct sockaddr_in s_nwy_server_v4 = {0};
static struct sockaddr_in6 s_nwy_server_v6 = {0};
static int s_nwy_inet_flag = AF_INET;
//ftp
static dataRecvContext_t *s_nwy_trans_recv = NULL;
static nwy_file_ftp_info_s s_nwy_cli_fileftp;
#ifdef NWY_OPEN_TEST_FTPS
static nwy_ftp_ssl_ext_paras_t s_ssl_para;
#endif
extern int nwy_test_cli_check_data_connect();
static bool nwy_cli_check_str_isdigit(char *str)
{
    if (NULL == str)
        return false;
    char *p = str;
    while ('\0' != *p)
    {
        if (!('0' <= *p && '9' >= *p))
            return false;
        p++;
    }
    return true;
}
static nwy_ip_type_or_dns_enum nwy_judge_ip_or_dns(char *str)
{
    int len = 0;
    int strLen = 0;
    nwy_ip_type_or_dns_enum retValue = NWY_CUSTOM_IP_TYPE_OR_DNS_DNS;
    if (str == NULL)
    {
        return NWY_CUSTOM_IP_TYPE_OR_DNS_NONE;
    }
    else
    {
        if (strlen(str) <= 0)
        {
            return NWY_CUSTOM_IP_TYPE_OR_DNS_NONE;
        }
    }
    strLen = strlen(str);
    for (len = 0; len < strLen; len++)
    {
        if (((*(str + len) >= '0') && (*(str + len) <= '9')) || (*(str + len) == '.'))
        {
            continue;
        }
        else
        {
            break;
        }
    }
    if (len == strLen)
    {
        retValue = NWY_CUSTOM_IP_TYPE_OR_DNS_IPV4;
        return retValue;
    }
    len = 0;
    for (len = 0; len < strLen; len++)
    {
        if (((*(str + len) >= '0') && (*(str + len) <= '9')) ||
            ((*(str + len) >= 'a') && (*(str + len) <= 'f')) ||
            ((*(str + len) >= 'A') && (*(str + len) <= 'F')) ||
            (*(str + len) == ':'))
        {
            continue;
        }
        else
        {
            break;
        }
    }
    if (len == strLen)
    {
        retValue = NWY_CUSTOM_IP_TYPE_OR_DNS_IPV6;
        return retValue;
    }
    return retValue;
}
static int nwy_hostname_check(char *hostname)
{
    int a, b, c, d;
    char temp[32] = {0};
    if (strlen(hostname) > 15)
        return NWY_GEN_E_UNKNOWN;
    if ((sscanf(hostname, "%d.%d.%d.%d", &a, &b, &c, &d)) != 4)
        return NWY_GEN_E_UNKNOWN;
    if (!((a <= 255 && a >= 0) && (b <= 255 && b >= 0) && (c <= 255 && c >= 0)))
        return NWY_GEN_E_UNKNOWN;
    sprintf(temp, "%d.%d.%d.%d", a, b, c, d);

    strcpy(hostname, temp);
    return NWY_SUCESS;
}
static int nwy_get_ip_str(char *url_or_ip, char *ip_str, int *isipv6)
{
    char *str = NULL;
    nwy_ip_type_or_dns_enum ip_dns_type = NWY_CUSTOM_IP_TYPE_OR_DNS_NONE;

    ip_dns_type = nwy_judge_ip_or_dns(url_or_ip);
    if (ip_dns_type == NWY_CUSTOM_IP_TYPE_OR_DNS_DNS)
    {
        str = nwy_sdk_gethostbyname1(url_or_ip, isipv6);
        if (str == NULL || 0 == strlen(str))
        {
            nwy_test_cli_echo("\r\ninput ip or url %s invalid\r\n", url_or_ip);
            return NWY_GEN_E_UNKNOWN;
        }
        memcpy(ip_str, str, strlen(str));

        NWY_CLI_LOG("\r\n%s get ip:%s\r\n", url_or_ip, ip_str);
    }
    else
    {
        memcpy(ip_str, url_or_ip, strlen(url_or_ip));
    }
    if (strchr(ip_str, ':') != NULL)
    {
        *isipv6 = 1;
    }
    else
    {
        *isipv6 = 0;
    }
    return NWY_SUCESS;
}
static int nwy_port_get(char *port_str, int *port)
{
    int i;
    if (port == NULL)
    {
        return NWY_GEN_E_INVALID_PARA;
    }
    for (i = 0; i < strlen(port_str); i++)
    {
        if (*(port_str + i) == '\r' || *(port_str + i) == '\n')
        {
            continue;
        }
        if ((port_str[i]) < '0' || (port_str[i]) > '9')
        {
            return NWY_GEN_E_INVALID_PARA;
        }
    }

    *port = nwy_cli_str_to_int(port_str);
    if (*port < 1 || *port > 65535)
    {
        *port = 0;
        return NWY_GEN_E_INVALID_PARA;
    }
    else
    {
        return NWY_SUCESS;
    }
}
static int nwy_udpcli_socket_destory(int *socketid)
{
    int ret = 0;
    ret = nwy_sdk_socket_close(*socketid);
    if (ret != NWY_SUCESS)
    {
        nwy_test_cli_echo("\r\nSocket close fail");
        return ret;
    }
    *socketid = 0;
    nwy_test_cli_echo("\r\nUDP Socket close sucess\r\n");
    return ret;
}
static int nwy_tcpcli_socket_destory(int *socketid)
{
    int ret = 0;
    ret = nwy_sdk_socket_close(*socketid);
    if (ret != NWY_SUCESS)
    {
        nwy_test_cli_echo("\r\nSocket close fail");
        return ret;
    }
    *socketid = 0;
    nwy_test_cli_echo("\r\nTCP Socket close sucess\r\n");
    return ret;
}

void nwy_cli_tcp_recv_func(void *param)
{
    char recv_buff[NWY_UART_RECV_SINGLE_MAX + 1] = {0};
    int recv_len = 0, result = 0;
    fd_set rd_fd;
    fd_set ex_fd;
    FD_ZERO(&rd_fd);
    FD_ZERO(&ex_fd);
    FD_SET(s_nwy_cli_tcp_fd, &rd_fd);
    FD_SET(s_nwy_cli_tcp_fd, &ex_fd);
    struct timeval tv = {0};
    tv.tv_sec = 20;
    tv.tv_usec = 0;
    while (1)
    {
        if (s_nwy_cli_tcp_fd <=0) {
            nwy_sleep(1000);
            continue;
        }

        if (nwy_test_cli_check_data_connect() == 0) {
            nwy_test_cli_echo("\r\nData call disconnect");
            nwy_tcpcli_socket_destory(&s_nwy_cli_tcp_fd);
            s_nwy_tcp_connect_flag = 0;
            nwy_sleep(1000);
            continue;
        }
        FD_ZERO(&rd_fd);
        FD_ZERO(&ex_fd);
        FD_SET(s_nwy_cli_tcp_fd, &rd_fd);
        FD_SET(s_nwy_cli_tcp_fd, &ex_fd);
        result = nwy_sdk_socket_select(s_nwy_cli_tcp_fd + 1, &rd_fd, NULL, &ex_fd, &tv);
        if (result < 0)
        {

            if(EBADF ==nwy_sdk_socket_errno())
            {
                if(s_nwy_cli_tcp_fd != 0)
                {
                    nwy_tcpcli_socket_destory(&s_nwy_cli_tcp_fd);
                }
                NWY_CLI_LOG("The socket client does not exist ");
            }
            else
            {
                nwy_test_cli_echo("\r\ntcp select error:\r\n");
            }
            s_nwy_tcp_connect_flag = 0;
            nwy_sleep(1000);
            continue;
        }
        else if (result > 0)
        {
            if (FD_ISSET(s_nwy_cli_tcp_fd, &rd_fd))
            {
                memset(recv_buff, 0, NWY_UART_RECV_SINGLE_MAX + 1);
                recv_len = nwy_sdk_socket_recv(s_nwy_cli_tcp_fd, recv_buff, NWY_UART_RECV_SINGLE_MAX, 0);
                if (recv_len > 0)
                    nwy_test_cli_echo("\r\ntcp read:%d:%s\r\n", recv_len, recv_buff);
                else if (recv_len == 0)
                {
                    nwy_test_cli_echo("\r\ntcp close by server\r\n");
                    nwy_tcpcli_socket_destory(&s_nwy_cli_tcp_fd);
                    s_nwy_tcp_connect_flag = 0;
                }
                else
                {
                    nwy_test_cli_echo("\r\ntcp connection error\r\n");
                    nwy_tcpcli_socket_destory(&s_nwy_cli_tcp_fd);
                    s_nwy_tcp_connect_flag = 0;
                }
            }
            if (FD_ISSET(s_nwy_cli_tcp_fd, &ex_fd))
            {
                nwy_test_cli_echo("\r\ntcp select ex_fd:\r\n");
                nwy_tcpcli_socket_destory(&s_nwy_cli_tcp_fd);
                s_nwy_tcp_connect_flag = 0;
            }
        }
        else
            NWY_CLI_LOG("\r\ntcp select timeout:\r\n");
        nwy_sleep(1000);
    }
}
void nwy_cli_udp_recv_func(void *param)
{
    char recv_buff[NWY_UART_RECV_SINGLE_MAX + 1] = {0};
    int recv_len = 0, result = 0;
    fd_set rd_fd;
    fd_set ex_fd;
    FD_ZERO(&rd_fd);
    FD_ZERO(&ex_fd);
    FD_SET(s_nwy_cli_udp_fd, &rd_fd);
    FD_SET(s_nwy_cli_udp_fd, &ex_fd);
    struct timeval tv = {0};
    tv.tv_sec = 20;
    tv.tv_usec = 0;
    while (1)
    {
        if (s_nwy_cli_udp_fd <=0) {
            nwy_sleep(1000);
            continue;
        }

        if (nwy_test_cli_check_data_connect() == 0) {
            nwy_test_cli_echo("\r\nData call disconnect");
            nwy_udpcli_socket_destory(&s_nwy_cli_udp_fd);
            s_nwy_udp_connect_flag = 0;
            nwy_sleep(1000);
            continue;
        }
        FD_ZERO(&rd_fd);
        FD_ZERO(&ex_fd);
        FD_SET(s_nwy_cli_udp_fd, &rd_fd);
        FD_SET(s_nwy_cli_udp_fd, &ex_fd);
        result = nwy_sdk_socket_select(s_nwy_cli_udp_fd + 1, &rd_fd, NULL, &ex_fd, &tv);
        if (result < 0)
        {

            if(EBADF ==nwy_sdk_socket_errno())
            {
                if(s_nwy_cli_udp_fd !=0)
                {
                    nwy_udpcli_socket_destory(&s_nwy_cli_udp_fd);
                }
                NWY_CLI_LOG("The socket client does not exist ");
            }
            else
            {
                nwy_test_cli_echo("\r\nudp select error:\r\n");
            }

            s_nwy_udp_connect_flag = 0;
            nwy_sleep(1000);
            continue;
        }
        else if (result > 0)
        {
            if (FD_ISSET(s_nwy_cli_udp_fd, &rd_fd))
            {
                memset(recv_buff, 0, NWY_UART_RECV_SINGLE_MAX + 1);
                recv_len = nwy_sdk_socket_recv(s_nwy_cli_udp_fd, recv_buff, sizeof(recv_buff), 0);
                if (recv_len > 0)
                    nwy_test_cli_echo("\r\nudp read:%d:%s\r\n", recv_len, recv_buff);
                else if (recv_len == 0)
                    nwy_test_cli_echo("\r\nudp close by server\r\n");
                else
                {
                    if (g_udp_manual_close) {
                        continue;
                    }
                    nwy_test_cli_echo("\r\nudp connection error\r\n");
                    nwy_udpcli_socket_destory(&s_nwy_cli_udp_fd);
                    s_nwy_udp_connect_flag = 0;
                    nwy_sleep(1000);
                    continue;
                }
            }
            if (FD_ISSET(s_nwy_cli_udp_fd, &ex_fd))
            {
                nwy_test_cli_echo("\r\nudp select ex_fd:\r\n");
                nwy_udpcli_socket_destory(&s_nwy_cli_udp_fd);
                s_nwy_udp_connect_flag = 0;
                nwy_sleep(1000);
                continue;
            }
        }
        else
        {
        } //nwy_test_cli_echo("\r\nudp select timeout:\r\n");
    }
}
static int nwy_cli_tcp_connect(struct sockaddr_in *sa_v4, struct sockaddr_in6 *sa_v6, int af_inet_flag)
{

    int ret = NWY_GEN_E_INVALID_PARA;
    int get_opt_len = 0;
    fd_set write_fds;
    fd_set ex_fds;
    struct timeval timeout;
    NWY_CLI_LOG("nwy_cli_tcp_connect enter");

    if (af_inet_flag == AF_INET6)
    {
        ret = nwy_sdk_socket_connect(s_nwy_cli_tcp_fd, (struct sockaddr *)sa_v6, sizeof(struct sockaddr));
    }
    else
    {
        ret = nwy_sdk_socket_connect(s_nwy_cli_tcp_fd, (struct sockaddr *)sa_v4, sizeof(struct sockaddr));
    }

    NWY_CLI_LOG("connecting ret:%d,errno = %d", ret,nwy_sdk_socket_errno(),0);
    if(ret != NWY_SUCCESS)
    {
       if (nwy_sdk_socket_errno() != EINPROGRESS)
        {
            nwy_sdk_socket_close(s_nwy_cli_tcp_fd);
            s_nwy_cli_tcp_fd = 0;
            return -1;

        }
        else
        {
            timeout.tv_sec = 10;
            timeout.tv_usec = 0;
 
            FD_ZERO(&write_fds);
            FD_ZERO(&ex_fds);
            FD_SET(s_nwy_cli_tcp_fd, &write_fds);
            FD_SET(s_nwy_cli_tcp_fd, &ex_fds);
            ret = nwy_sdk_socket_select(s_nwy_cli_tcp_fd + 1, NULL, &write_fds, &ex_fds, &timeout);
            switch (ret)
            {
                case -1:       
                {
                    NWY_CLI_LOG("connect error ret:%d,errno = %d", ret,nwy_sdk_socket_errno(),0);
                    nwy_sdk_socket_close(s_nwy_cli_tcp_fd);
                    s_nwy_cli_tcp_fd = 0;
                    return -1;
                }
                case 0:        
                {
                    NWY_CLI_LOG("connect timeout ret:%d,errno = %d", ret,nwy_sdk_socket_errno(),0);
                    nwy_sdk_socket_close(s_nwy_cli_tcp_fd);
                    s_nwy_cli_tcp_fd = 0;
                    return -1;
                }
                default:
                {
                    NWY_CLI_LOG("connect :%d,errno = %d", ret,nwy_sdk_socket_errno(),0);
                    if(FD_ISSET(s_nwy_cli_tcp_fd,&ex_fds))
                    {
                        NWY_CLI_LOG("ex connect error: :%d,errno = %d", ret,0,0);
                        nwy_sdk_socket_close(s_nwy_cli_tcp_fd);
                        s_nwy_cli_tcp_fd = 0;
                        return -1;
                    }

                    if(FD_ISSET(s_nwy_cli_tcp_fd,&write_fds))
                    {
                         int error = 0;
                         socklen_t len = sizeof(error);
                         if(nwy_sdk_socket_getsockopt(s_nwy_cli_tcp_fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
                         {
                             NWY_CLI_LOG("connect error: :%d,errno = %d", ret,0,0);
                             nwy_sdk_socket_close(s_nwy_cli_tcp_fd);
                             s_nwy_cli_tcp_fd = 0;
                             return -1;
                         }
                         if(error != 0)
                          {
                               NWY_CLI_LOG("connect error: :%d,errno = %d", ret,0,0);
                               nwy_sdk_socket_close(s_nwy_cli_tcp_fd);
                               s_nwy_cli_tcp_fd = 0;
                               return -1;
                          }
                          else
                          {
                               NWY_CLI_LOG("connect ok", 0,0,0);
                               s_nwy_tcp_connect_flag = 1;
                          }
                    }
                    else
                    {
                        NWY_CLI_LOG("Invalid socket", 0,0,0);
                    }
                }
            }
        }
    }
    else
    {
        s_nwy_tcp_connect_flag = 1;
    }

    NWY_CLI_LOG("nwy_cli_tcp_connect s_nwy_tcp_connect_flag = %d", s_nwy_tcp_connect_flag);
    if (s_nwy_tcp_connect_flag)
    {
        if (s_nwy_tcp_recv_thread == NULL) {
            nwy_create_thread(&s_nwy_tcp_recv_thread, NWY_THREAD_STACK_SIZE, NWY_OSI_PRIORITY_NORMAL, "s_nwy_cli_recv_thread", (nwy_task_cb_func_t)nwy_cli_tcp_recv_func, (void *)&s_nwy_cli_tcp_fd, 8);
            if (s_nwy_tcp_recv_thread == NULL)
            {
                nwy_test_cli_echo("\r\ncreate tcp recv thread failed, close connect");
                nwy_tcpcli_socket_destory(&s_nwy_cli_tcp_fd);
                s_nwy_tcp_connect_flag = 0;
            }
        }

        return NWY_SUCESS;
    }
    return NWY_GEN_E_UNKNOWN;
}
static int nwy_cli_socket_setup_pro(char *url_or_ip, int port, int socekt_type)
{
    int isipv6 = 0;
    int fd = 0;
    int on = 1;
    int opt = 1;
    int ret = NWY_GEN_E_UNKNOWN;
    char ip_buf[NWY_BUFF_MAX + 1] = {0};
    ip_addr_t addr = {0};

    if (url_or_ip == NULL)
    {
        return NWY_GEN_E_INVALID_PARA;
    }
    nwy_test_cli_echo("\r\nurl test start:%s:%d\r\n", url_or_ip, port);
    ret = nwy_get_ip_str(url_or_ip, ip_buf, &isipv6);
    if (ret != NWY_SUCESS)
    {
        return NWY_GEN_E_INVALID_PARA;
    }
    if (isipv6)
    {
        memset(&s_nwy_server_v6, 0, sizeof(s_nwy_server_v6));
        if (nwy_ipv6_addr_aton(ip_buf, &addr.u_addr.ip6) == 0)
        {
            nwy_test_cli_echo("\r\ninput ip or url is invalid");
            return NWY_GEN_E_INVALID_PARA;
        }
        else
        {
            inet6_addr_from_ip6addr(&s_nwy_server_v6.sin6_addr, ip_2_ip6(&addr));
            s_nwy_server_v6.sin6_len = sizeof(struct sockaddr_in);
            s_nwy_server_v6.sin6_family = AF_INET6;
            s_nwy_server_v6.sin6_port = htons(port);
            s_nwy_inet_flag = AF_INET6;
        }
    }
    else
    {
        memset(&s_nwy_server_v4, 0, sizeof(s_nwy_server_v4));
        ret = nwy_hostname_check(ip_buf);
        if (ret != NWY_SUCESS)
        {
            nwy_test_cli_echo("\r\ninput ip or url is invalid");
            return NWY_GEN_E_INVALID_PARA;
        }

        if (nwy_ipv4_addr_aton(ip_buf, (ip_addr_t *)&addr.u_addr.ip4) == 0)
        {
            nwy_test_cli_echo("\r\ninput ip error:\r\n");
            return NWY_GEN_E_INVALID_PARA;
        }
        s_nwy_server_v4.sin_len = sizeof(struct sockaddr_in);
        s_nwy_server_v4.sin_family = AF_INET;
        s_nwy_server_v4.sin_port = htons(port);
        s_nwy_server_v4.sin_addr.s_addr = nwy_ipv4_addr_addr(ip_buf);
        s_nwy_inet_flag = AF_INET;
    }

    if (socekt_type == IPPROTO_TCP)
    {
        if (s_nwy_cli_tcp_fd <= 0)
        {
            s_nwy_cli_tcp_fd = nwy_sdk_socket_open(s_nwy_inet_flag, SOCK_STREAM, socekt_type);
            if (fd < 0)
            {
                nwy_test_cli_echo("\r\n socket open fail:\r\n");
                return NWY_GEN_E_INVALID_PARA;
            }
        }

        NWY_CLI_LOG("nwy s_nwy_cli_tcp_fd = %d", s_nwy_cli_tcp_fd);
        nwy_sdk_socket_setsockopt(s_nwy_cli_tcp_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(on));
        nwy_sdk_socket_setsockopt(s_nwy_cli_tcp_fd, IPPROTO_TCP, TCP_NODELAY, (void *)&opt, sizeof(opt));
        if (0 != nwy_sdk_socket_set_nonblock(s_nwy_cli_tcp_fd))
        {
            nwy_test_cli_echo("\r\nsocket set err\r\n");
            return NWY_GEN_E_INVALID_PARA;
        }

        ret = nwy_cli_tcp_connect(&s_nwy_server_v4, &s_nwy_server_v6, s_nwy_inet_flag);
        if (ret == NWY_SUCESS)
        {
            nwy_test_cli_echo("\r\nTCP set up sucess\r\n");
        }
    }
    else if (socekt_type == IPPROTO_UDP)
    {
        if (s_nwy_cli_udp_fd <= 0)
        {
            s_nwy_cli_udp_fd = nwy_sdk_socket_open(s_nwy_inet_flag, SOCK_DGRAM, socekt_type);
            if (s_nwy_cli_udp_fd <= 0)
            {
                nwy_test_cli_echo("\r\nsocket open fail:\r\n");
                return NWY_GEN_E_INVALID_PARA;
            }
        }

        if (s_nwy_udp_recv_thread == NULL) {
            nwy_create_thread(&s_nwy_udp_recv_thread, NWY_THREAD_STACK_SIZE, NWY_OSI_PRIORITY_NORMAL, "nwy_udp_recv_thread", (nwy_task_cb_func_t)nwy_cli_udp_recv_func, (void *)&s_nwy_cli_udp_fd, 8);
            if (s_nwy_udp_recv_thread == NULL)
            {
                nwy_test_cli_echo("\r\ncreate udp recv thread failed");
                return NWY_GEN_E_INVALID_PARA;
            }
        }
        s_nwy_udp_connect_flag = 1;
        nwy_test_cli_echo("\r\nUDP setup successful");

    }
    return ret;
}
/**************************TCP*********************************/
void nwy_test_cli_tcp_setup()
{
    char *tmp = NULL;
    char url_or_ip[128 + 1] = {0};
    int port = 0;
    int ret = 0;
    if (s_nwy_cli_tcp_fd > 0)
    {
        nwy_test_cli_echo("\r\nSocket has been setup");
        return;
    }

    if (nwy_test_cli_check_data_connect() == 0) {
        nwy_test_cli_echo("\r\nData call not connect");
        return;
    }
    tmp = nwy_test_cli_input_gets("\r\nPlease input url(<=128): ");
    if(strlen(tmp)>128)
    {
        nwy_test_cli_echo("\r\nurl parameter error");
        return;
    }
    memcpy(url_or_ip, tmp, strlen(tmp));
    tmp = nwy_test_cli_input_gets("\r\nPlease input port: ");
    if (nwy_port_get(tmp, &port) != NWY_SUCESS)
    {
        nwy_test_cli_echo("\r\nInput port error");
        return;
    }

    ret = nwy_cli_socket_setup_pro(url_or_ip, port, IPPROTO_TCP);
    if (ret != NWY_SUCESS)
    {
        nwy_test_cli_echo("\r\nTCP setup error");
    }
}
void nwy_test_cli_tcp_send()
{
    int send_len = 0;
    int len = 0;
    char *buff = NULL;
    if (!s_nwy_tcp_connect_flag)
    {
        nwy_test_cli_echo("\r\ntcp not setup");
        return;
    }
    buff = nwy_test_cli_input_gets("\r\nPlease input send data(<=512): ");
    len = nwy_test_cli_input_len_gets();
    if (len > NWY_UART_RECV_SINGLE_MAX)
    {
        nwy_test_cli_echo("\r\nNo more than 512 bytes at a time to send ");
        return;
    }
    send_len = nwy_sdk_socket_send(s_nwy_cli_tcp_fd, buff, len, 0);
    if (send_len != len)
    {
        nwy_test_cli_echo("\r\nsend len=%d, return len=%d,errno:%d", len, send_len,nwy_sdk_socket_errno());
    }
    else
        nwy_test_cli_echo("\r\nsend ok");
}
void nwy_test_cli_tcp_close()
{
    nwy_tcpcli_socket_destory(&s_nwy_cli_tcp_fd);
}
/**************************UDP*********************************/
void nwy_test_cli_udp_setup()
{
    char *tmp = NULL;
    char url_or_ip[128 + 1] = {0};
    int port = 0;
    if (s_nwy_cli_udp_fd > 0)
    {
        nwy_test_cli_echo("\r\nSocket has been setup");
        return;
    }
    if (nwy_test_cli_check_data_connect() == 0) {
        nwy_test_cli_echo("\r\nData call disconnect");
        return;
    }
    tmp = nwy_test_cli_input_gets("\r\nPlease input url: ");
    memcpy(url_or_ip, tmp, strlen(tmp));
    tmp = nwy_test_cli_input_gets("\r\nPlease input port: ");
    if (nwy_port_get(tmp, &port) != NWY_SUCESS)
    {
        nwy_test_cli_echo("\r\nInput port error");
        return;
    }
    g_udp_manual_close = 0;
    nwy_cli_socket_setup_pro(url_or_ip, port, IPPROTO_UDP);
}
void nwy_test_cli_udp_send()
{
    int send_len = 0;
    int len = 0;
    char *buff = NULL;
    buff = nwy_test_cli_input_gets("\r\nPlease input send data(<=512): ");
    len = nwy_test_cli_input_len_gets();
    if (len > NWY_UART_RECV_SINGLE_MAX)
    {
        nwy_test_cli_echo("\r\nNo more than 512 bytes at a time to send ");
        return;
    }

    if(s_nwy_cli_udp_fd == 0)
    {
        nwy_test_cli_echo("\r\nNo udp socket created");
        return;
    }

    if (s_nwy_inet_flag == AF_INET) {
        send_len= nwy_sdk_socket_sendto(s_nwy_cli_udp_fd, buff, len, 0, (struct sockaddr *)&s_nwy_server_v4, sizeof(struct sockaddr));
    } else if (s_nwy_inet_flag == AF_INET6) {
        send_len= nwy_sdk_socket_sendto(s_nwy_cli_udp_fd, buff, len, 0, (struct sockaddr *)&s_nwy_server_v6, sizeof(struct sockaddr));
    }
    if (send_len != strlen(buff))
    {
        nwy_test_cli_echo("\r\nsend len=%d, return len=%d,errno:%d", len, send_len,nwy_sdk_socket_errno());
    }
    else
        nwy_test_cli_echo("\r\nsend ok");
}
void nwy_test_cli_udp_close()
{
    g_udp_manual_close = 1;
    nwy_udpcli_socket_destory(&s_nwy_cli_udp_fd);
}
/**************************FTP*********************************/
static void nwy_cli_data_trans_proc(const char *data, int length)
{
    if (length >= s_nwy_trans_recv->size - s_nwy_trans_recv->pos)
    {
        length = s_nwy_trans_recv->size - s_nwy_trans_recv->pos;
        NWY_CLI_LOG("\r\ninput data len > mem len so cut off %d", length);
    }

    memcpy((s_nwy_trans_recv->data + s_nwy_trans_recv->pos), data, length);
    s_nwy_trans_recv->pos = s_nwy_trans_recv->pos + length;
    nwy_test_cli_echo("\r\n nwy_app_info.rev_len = %d length = %d\r\n", s_nwy_trans_recv->pos, length);
    if (s_nwy_trans_recv->pos >= s_nwy_trans_recv->size)
    {
        nwy_test_cli_send_trans_end();
        nwy_test_cli_sio_quit_trans_mode();
    }
}


int nwy_cli_ftp_get_local_filename(char * pathname,int len)
{

    char file_name[128 + 1] = {0};
    char file_name_com[128 + 1] = {0};
    char * start = NULL;
    char *p = NULL;
    if (NULL == pathname) {
    return -1;
    }
    if(strlen(pathname)>128 ||pathname[len-1] == '/')
    {
        return -1;
    }
    strcpy(file_name,pathname);
    p = strstr(file_name, "/");

    if(p== NULL)
    {
        return 0;
    }

    while(p!=NULL)
    {
        memset(pathname,0,len);
        strcpy(pathname,p+1);
        memset(file_name,0,sizeof(file_name));
        strcpy(file_name,pathname);
        p = strstr(file_name, "/");
    }

    return 0;
}



static int nwy_cli_ftp_write_file(nwy_file_ftp_info_s *pFileFtp, unsigned char *data, unsigned int len)
{
#ifdef NWY_OPEN_CLI_UIS8850
    int fs = nwy_sdk_fopen(pFileFtp->filename, NWY_AB_MODE);
#else
    int fs = nwy_sdk_fopen(pFileFtp->filename, NWY_WB_MODE);
#endif
    if (fs < 0)
    {
        NWY_CLI_LOG("nwy file open fail");
        return -1;
    }
    nwy_sdk_fseek(fs, pFileFtp->pos, NWY_SEEK_SET);
    nwy_sdk_fwrite(fs, data, len);
    nwy_sdk_fclose(fs);
    pFileFtp->pos += len;
    NWY_CLI_LOG("nwy file write %d size success.", len);
    return 0;
}
static void nwy_cli_ftp_result_cb(nwy_ftp_result_t *param)
{
    int *size;
    int ret;
    static int get_file_len = 0;
    if (NULL == param)
    {
        nwy_test_cli_echo("event is NULL\r\n");
    }
    NWY_CLI_LOG("event is %d", param->event);
    if (NWY_FTP_EVENT_LOGIN == param->event)
    {
        nwy_test_cli_echo("\r\nFtp login success");
    }
    else if (NWY_FTP_EVENT_PASS_ERROR == param->event)
    {
        nwy_test_cli_echo("\r\nFtp passwd error");
    }
    else if (NWY_FTP_EVENT_FILE_NOT_FOUND == param->event)
    {
        nwy_test_cli_echo("\r\nFtp file not found");
    }
    else if (NWY_FTP_EVENT_FILE_SIZE_ERROR == param->event)
    {
        nwy_test_cli_echo("\r\nFtp file size error");
    }
    else if (NWY_FTP_EVENT_DATA_SETUP_ERROR == param->event)
    {
        nwy_test_cli_echo("\r\nFtp data setup error");
    }
    else if (NWY_FTP_EVENT_SIZE == param->event)
    {
        size = (int *)param->data;
        nwy_test_cli_echo("\r\nFtp size is %d", *size);
    }
    else if (NWY_FTP_EVENT_LOGOUT == param->event)
    {
        nwy_test_cli_echo("\r\nFtp logout");
    }
    else if (NWY_FTP_EVENT_CLOSED == param->event)
    {
        nwy_test_cli_echo("\r\nFtp connection closed");
    }
    else if (NWY_FTP_EVENT_SIZE_ZERO == param->event)
    {
        nwy_test_cli_echo("\r\nFtp size is zero");
    }
    else if (NWY_FTP_EVENT_FILE_DELE_SUCCESS == param->event)
    {
        nwy_test_cli_echo("\r\nFtp file del success");
    }
    else if (NWY_FTP_EVENT_DATA_PUT_FINISHED == param->event)
    {
        nwy_test_cli_echo("\r\nFtp put file success");
    }
    else if (NWY_FTP_EVENT_DNS_ERR == param->event || NWY_FTP_EVENT_OPEN_FAIL == param->event)
    {
        nwy_test_cli_echo("\r\nFtp login fail");
    }
    else if (NWY_FTP_EVENT_DATA_GET == param->event)
    {
        if (NULL == param->data)
        {
            nwy_test_cli_echo("\r\nrecved data is NULL");
            return;
        }
        if (1 == s_nwy_cli_fileftp.is_vaild)
        {
            if (param->data_len != 0)
            {
                
                ret = nwy_cli_ftp_write_file(&s_nwy_cli_fileftp, (unsigned char *)param->data, param->data_len);
                if (0 != ret)
                {
                    nwy_test_cli_echo("\r\nwrite failed");
                    
                }
                else
                {
                    get_file_len=get_file_len+param->data_len;
                    nwy_test_cli_echo("\r\nlocal file write len :%d,current file len:%d\r\n", param->data_len,get_file_len);
                }
            }
        }
        else
        {
            //nwy_test_cli_echo("\r\ndata_size is %d", param->data_len);
            //nwy_test_cli_echo("\r\nparam->data is %s", param->data);
        }
    }
    else if (NWY_FTP_EVENT_DATA_CLOSED == param->event)
    {
        if(s_nwy_cli_fileftp.is_vaild == 1)
        {
            nwy_test_cli_echo("\r\nfile %s download success.file len:%d", s_nwy_cli_fileftp.filename,get_file_len);
            get_file_len = 0;
            memset(&s_nwy_cli_fileftp,0,sizeof(s_nwy_cli_fileftp));
        }
    }
    else
    {
      //  nwy_test_cli_echo("\r\nFtp unknown error");
    }
    return;
}
#ifdef NWY_OPEN_TEST_FTPS
static void nwy_cli_ftps_result_cb(nwy_ftps_result_t *param)
{
    int *size;
    int ret;
    static int get_file_len = 0;
    if (NULL == param)
    {
        nwy_test_cli_echo("event is NULL\r\n");
    }
    NWY_CLI_LOG("event is %d", param->event);
    if (NWY_FTPS_EVENT_LOGIN == param->event)
    {
        nwy_test_cli_echo("\r\nFtp login success");
    }
    else if (NWY_FTPS_EVENT_PASS_ERR == param->event)
    {
        nwy_test_cli_echo("\r\nFtp passwd error");
    }
    else if (NWY_FTPS_EVENT_FILE_NOT_FOUND == param->event)
    {
        nwy_test_cli_echo("\r\nFtp file not found");
    }
    else if (NWY_FTPS_EVENT_FILE_SIZE_ERR == param->event)
    {
        nwy_test_cli_echo("\r\nFtp file size error");
    }
    else if (NWY_FTPS_EVENT_DATA_SETUP_ERR == param->event)
    {
        nwy_test_cli_echo("\r\nFtp data setup error");
    }
    else if (NWY_FTPS_EVENT_SIZE == param->event)
    {
        size = (int *)param->data;
        nwy_test_cli_echo("\r\nFtp size is %d", *size);
    }
    else if (NWY_FTPS_EVENT_LOGOUT == param->event)
    {
        nwy_test_cli_echo("\r\nFtp logout");
    }
    else if (NWY_FTPS_EVENT_CLOSED == param->event)
    {
        nwy_test_cli_echo("\r\nFtp connection closed");
    }
    else if (NWY_FTPS_EVENT_SIZE_ZERO == param->event)
    {
        nwy_test_cli_echo("\r\nFtp size is zero");
    }
    else if (NWY_FTPS_EVENT_FILE_DELE_SUCCESS == param->event)
    {
        nwy_test_cli_echo("\r\nFtp file del success");
    }
    else if (NWY_FTPS_EVENT_DATA_PUT_FINISHED == param->event)
    {
        nwy_test_cli_echo("\r\nFtp put file success");
    }
    else if (NWY_FTPS_EVENT_DNS_ERR == param->event || NWY_FTPS_EVENT_OPEN_FAIL == param->event)
    {
        nwy_test_cli_echo("\r\nFtp login fail");
    }
    else if (NWY_FTPS_EVENT_DATA_GET == param->event)
    {
        if (NULL == param->data)
        {
            nwy_test_cli_echo("\r\nrecved data is NULL");
            return;
        }
        if (1 == s_nwy_cli_fileftp.is_vaild)
        {
            if (param->data_len != 0)
            {
                ret = nwy_cli_ftp_write_file(&s_nwy_cli_fileftp, (unsigned char *)param->data, param->data_len);
                if (0 != ret)
                {
                    nwy_test_cli_echo("\r\nwrite failed");
                }
                else
                {
                    get_file_len=get_file_len+param->data_len;
                    nwy_test_cli_echo("\r\nlocal file write len :%d,current file len:%d\r\n", param->data_len,get_file_len);
                }
            }
        }
        else
        {
            //nwy_test_cli_echo("\r\ndata_size is %d", param->data_len);
            //nwy_test_cli_echo("\r\nparam->data is %s", param->data);
        }
    }
    else if (NWY_FTPS_EVENT_DATA_CLOSED == param->event)
    {
        if(s_nwy_cli_fileftp.is_vaild == 1)
        {
            nwy_test_cli_echo("\r\nfile %s download success.file len:%d", s_nwy_cli_fileftp.filename,get_file_len);
            get_file_len = 0;
            memset(&s_nwy_cli_fileftp,0,sizeof(s_nwy_cli_fileftp));
        }
    }
    else
    {
      //  nwy_test_cli_echo("\r\nFtp unknown error");
    }
    return;
}
#endif
void nwy_test_cli_ftp_login()
{
    int result = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    int s_nwy_ftp_choice = 0;
    int s_nwy_ftps_type;
    nwy_ftp_login_para_t ftp_param;
    nwy_app_ssl_conf_t ssl_cfg;
    memset(&ftp_param, 0x00, sizeof(ftp_param));
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    ftp_param.channel = nwy_cli_str_to_int(sptr);
    if (ftp_param.channel < 1 || ftp_param.channel > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input ip(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\nip parameter error");
        return;
    }

    strcpy((char *)ftp_param.host, sptr);
    sptr = nwy_test_cli_input_gets("\r\n input port:");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    ftp_param.port = nwy_cli_str_to_int(sptr);
    sptr = nwy_test_cli_input_gets("\r\n input FTPtype (active mode = 1 passive mode = 0):");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    ftp_param.mode = nwy_cli_str_to_int(sptr);
    if (ftp_param.mode != 0 && ftp_param.mode != 1)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input username(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\nusername parameter error");
        return;
    }
    strcpy((char *)ftp_param.username, sptr);
    sptr = nwy_test_cli_input_gets("\r\n input passwd(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\npasswd parameter error");
        return;
    }
    strcpy((char *)ftp_param.passwd, sptr);
    ftp_param.timeout = 180;
#ifdef NWY_OPEN_TEST_FTPS
    sptr = nwy_test_cli_input_gets("\r\n input 1-FTPS, 0-FTP:");
    s_nwy_ftp_choice = nwy_cli_str_to_int(sptr);
    if (s_nwy_ftp_choice == 1)
    {
        sptr = nwy_test_cli_input_gets("\r\n input ftps type: 1-explicit,2-implicit");
        s_nwy_ftps_type = nwy_cli_str_to_int(sptr);
        memset(&ssl_cfg, 0x00, sizeof(ssl_cfg));
        sptr = nwy_test_cli_input_gets("\r\n input ssl version: 0:SSL3.0 1:TLS1.0 2:TLS1.1 3:TLS1.2");
        ssl_cfg.ssl_version = (nwy_ssl_version_type_e)nwy_cli_str_to_int(sptr);
        sptr = nwy_test_cli_input_gets("\r\n input authmode: 0:No authentication 1:Manage server authentication 2:Manage server and client authentication");
        ssl_cfg.authmode = (nwy_ssl_auth_mode_e)atoi(sptr);
        if (0 != ssl_cfg.authmode)
        {
            sptr = nwy_test_cli_input_gets("\r\n input cacert name:");
            strcpy(ssl_cfg.cacert.cert_name, sptr);
            if(ssl_cfg.authmode == 2)
            {
            sptr = nwy_test_cli_input_gets("\r\n input clientcert name:");
            strcpy(ssl_cfg.clientcert.cert_name, sptr);
            sptr = nwy_test_cli_input_gets("\r\n input clientkey name:");
            strcpy(ssl_cfg.clientkey.cert_name, sptr);
            }
        }
        s_ssl_para.ftps_type = s_nwy_ftps_type;
        s_ssl_para.ssl_conf.ssl_version = ssl_cfg.ssl_version;
        s_ssl_para.ssl_conf.authmode = ssl_cfg.authmode;
        strcpy(s_ssl_para.ssl_conf.cacert.cert_name, ssl_cfg.cacert.cert_name);
        strcpy(s_ssl_para.ssl_conf.clientcert.cert_name, ssl_cfg.clientcert.cert_name);
        strcpy(s_ssl_para.ssl_conf.clientkey.cert_name, ssl_cfg.clientkey.cert_name);

        result = nwy_ftps_login(&ftp_param, &s_ssl_para, nwy_cli_ftps_result_cb);
    }
    else
#endif
        result = nwy_ftp_login(&ftp_param, nwy_cli_ftp_result_cb);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n %s success", s_nwy_ftp_choice ? "nwy_ftps_setup" : "nwy_ftp_setup");
    }
    else
    {
        nwy_test_cli_echo("\r\n %s fail", s_nwy_ftp_choice ? "nwy_ftps_setup" : "nwy_ftp_setup");
    }
}
void nwy_test_cli_ftp_get()
{
    int result = NWY_GEN_E_UNKNOWN;
    int type = 0;
    char buff[128+1] = {0};
    char *sptr = NULL;
    uint16_t channel = 0;
    int nwy_ftp_choice = 0;
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    channel = nwy_cli_str_to_int(sptr);
    if (channel < 1 || channel > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input type:(1 ASCII 2 Binary)");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    type = nwy_cli_str_to_int(sptr);
    if (type < 1 || type > 2)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input filename(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ninput error");
        return;
    }
    strcpy(buff, sptr);

    s_nwy_cli_fileftp.is_vaild = 1;
    memset(s_nwy_cli_fileftp.filename,0x00,sizeof(s_nwy_cli_fileftp.filename));
        
    strcpy(s_nwy_cli_fileftp.filename, sptr);

    if(0!= nwy_cli_ftp_get_local_filename(s_nwy_cli_fileftp.filename,strlen(s_nwy_cli_fileftp.filename)))
    {
        nwy_test_cli_echo("\r\n file name is invalid\r\n");
        return;
    }
    #ifdef NWY_OPEN_TEST_FTPS
    sptr = nwy_test_cli_input_gets("\r\n input 1-FTPS, 0-FTP:");
    nwy_ftp_choice = nwy_cli_str_to_int(sptr);
    if (nwy_ftp_choice == 1)
    {
        result = nwy_ftps_get(channel, buff, type, 0, 0, 180, &s_ssl_para);
    }
    else
    #endif
    result = nwy_ftp_get(channel,buff, type, 0, 0);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_ftp_get success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_ftp_get fail");
    }
}
void nwy_test_cli_ftp_put()
{
    int type = 0;
    int mode = 0;
    int len = 0;
    int result = NWY_GEN_E_UNKNOWN;
    char filename[128+1] = {0};
    char *sptr = NULL;
    uint16_t channel = 0;
    int nwy_ftp_choice = 0;
#ifdef NWY_OPEN_TEST_FTPS
    nwy_ftps_put_param_t ftps_put_para;
#endif
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    channel = nwy_cli_str_to_int(sptr);
    if (channel < 1 || channel > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input filename(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ninput error");
        return;
    }

    strcpy(filename, sptr);

    sptr = nwy_test_cli_input_gets("\r\n input type:(1 ASCII 2 Binary)");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    type = nwy_cli_str_to_int(sptr);
    if (type < 1 || type > 2)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input mode:(1 STOR 2 APPE 3 DELE)");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    mode = nwy_cli_str_to_int(sptr);
    if (mode < 1 || mode > 3)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    if(mode != 3)
    {

        sptr = nwy_test_cli_input_gets("\r\n input put data len[1-8096]:");
        if (!nwy_cli_check_str_isdigit(sptr))
        {
            nwy_test_cli_echo("\r\n input error");
            return;
        }
        len = nwy_cli_str_to_int(sptr);

        if(len == 0 || len > 8096)
        {
            nwy_test_cli_echo("\r\n input data len error!\r\n");
            return;
        }

        s_nwy_trans_recv = malloc(sizeof(dataRecvContext_t) + len);
        if (s_nwy_trans_recv == NULL)
        {
            nwy_test_cli_echo("\r\n malloc buffer fail");
            return;
        }
        memset(s_nwy_trans_recv, 0, sizeof(dataRecvContext_t) + len);
        s_nwy_trans_recv->size = len;
        s_nwy_trans_recv->pos = 0;
        nwy_test_cli_sio_enter_trans_mode(nwy_cli_data_trans_proc);
        nwy_test_cli_echo("\r\nplease input data");
        if (nwy_test_cli_wait_trans_end())
        {
            #ifdef NWY_OPEN_TEST_FTPS
            sptr = nwy_test_cli_input_gets("\r\n input 1-FTPS, 0-FTP:");
            nwy_ftp_choice = atoi(sptr);
            if(nwy_ftp_choice == 1)
            {
                ftps_put_para.channel = channel;
                ftps_put_para.filename = filename;
                ftps_put_para.data_ptr = s_nwy_trans_recv->data;
                ftps_put_para.len = s_nwy_trans_recv->size;
                ftps_put_para.mode = mode;
                ftps_put_para.tout = 180;
                ftps_put_para.type = type;
                sptr = nwy_test_cli_input_gets("\r\n don't keep data? (0: yes  1: no):");
                int choice = nwy_cli_str_to_int(sptr);
                if (choice == 0)
                {
                    ftps_put_para.eof = true;
                }
                else
                {
                    ftps_put_para.eof = false;
                }
                result = nwy_ftps_put(&ftps_put_para, &s_ssl_para);
            }
            else
            #endif
            result = nwy_ftp_put(channel, filename, type, mode, s_nwy_trans_recv->data, s_nwy_trans_recv->size);
            if (0 == result)
            {
                nwy_test_cli_echo("\r\n nwy_ftp_put success");
            }
            else
            {
                nwy_test_cli_echo("\r\n nwy_ftp_put fail");
            }
        }
        free(s_nwy_trans_recv);
        s_nwy_trans_recv = NULL;
    }
    else
    {
        #ifdef NWY_OPEN_TEST_FTPS
        sptr = nwy_test_cli_input_gets("\r\n input 1-FTPS, 0-FTP:");
        nwy_ftp_choice = atoi(sptr);
        if (nwy_ftp_choice == 1)
        {
            ftps_put_para.channel = channel;
            ftps_put_para.filename = filename;
            ftps_put_para.data_ptr = NULL;
            ftps_put_para.len = 0;
            ftps_put_para.mode = mode;
            ftps_put_para.tout = 180;
            ftps_put_para.type = type;
            sptr = nwy_test_cli_input_gets("\r\n don't keep data? (0: yes  1: no):");
            int choice = nwy_cli_str_to_int(sptr);
            if (choice == 0)
            {
                ftps_put_para.eof = true;
            }
            else
            {
                ftps_put_para.eof = false;
            }
            result = nwy_ftps_put(&ftps_put_para, &s_ssl_para);
        }
        else
        #endif
        result = nwy_ftp_put(channel, filename, type, mode, NULL, 0);
        if (0 == result)
        {
            nwy_test_cli_echo("\r\n nwy_ftp_put success");
        }
        else
        {
            nwy_test_cli_echo("\r\n nwy_ftp_put fail");
        }
    }
}
void nwy_test_cli_ftp_fsize()
{
    char *sptr = NULL;
    char buff[128+1] = {0};
    int result = NWY_GEN_E_UNKNOWN;
    uint16_t channel = 0;
    int nwy_ftp_choice = 0;
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    channel = nwy_cli_str_to_int(sptr);
    if (channel < 1 || channel > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input filename(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ninput error");
        return;
    }
    strcpy(buff, sptr);
    #ifdef NWY_OPEN_TEST_FTPS
    sptr = nwy_test_cli_input_gets("\r\n input 1-FTPS, 0-FTP:");
    nwy_ftp_choice = atoi(sptr);
    if (nwy_ftp_choice == 1)
        result = nwy_ftps_filesize(channel, buff, 30, &s_ssl_para);
    else
    #endif
    result = nwy_ftp_filesize(channel,buff, 30);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_ftp_filesize success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_ftp_filesize fail");
    }
}
void nwy_test_cli_ftp_list()
{
    nwy_test_cli_echo("\r\nOption not Supported!\r\n");
}
void nwy_test_cli_ftp_delet()
{
    int ret = 0;
    char *sptr = NULL;
    char file_name[128 + 1] = "";

    nwy_test_cli_echo("\r\n delete get file");
    sptr = nwy_test_cli_input_gets("\r\n input filename(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ninput error");
        return;
    }
    strcat(file_name, sptr);

    if(0!= nwy_cli_ftp_get_local_filename(file_name,strlen(file_name)))
    {
        nwy_test_cli_echo("\r\n file name is invalid\r\n");
        return;
    }

    int testfd = nwy_sdk_fopen(file_name, NWY_RB_MODE);
    if (testfd < 0)
    {
        nwy_test_cli_echo("\r\nfile %s don't exist\r\n", file_name);
        return;
    }
    nwy_sdk_fclose(testfd);
    ret = nwy_sdk_file_unlink(file_name);
    if (ret == 0)
    {
        nwy_test_cli_echo("\r\n delete %s file success", file_name);
    }
    else
    {
        nwy_test_cli_echo("\r\n delete %s file fail", file_name);
    }
}
void nwy_test_cli_ftp_logout()
{
    int result = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    uint16_t channel = 0;
    int nwy_ftp_choice = 0;
    sptr = nwy_test_cli_input_gets("\r\n input channel(1-7):");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    channel = nwy_cli_str_to_int(sptr);
    if (channel < 1 || channel > 7)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }

    nwy_test_cli_echo("\r\n ftp will logout");
    #ifdef NWY_OPEN_TEST_FTPS
    sptr = nwy_test_cli_input_gets("\r\n input 1-FTPS, 0-FTP:");
    nwy_ftp_choice = atoi(sptr);
    if (nwy_ftp_choice == 1)
        result = nwy_ftps_logout(channel, 30, &s_ssl_para);
    else
    #endif
    result = nwy_ftp_logout(channel,30);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_ftp_logout success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_ftp_logout fail");
    }
}
/**************************HTTP*********************************/

static void nwy_cli_http_result_cb(nwy_http_result_t *param)
{
    if (NULL == param)
    {
        nwy_test_cli_echo("event is NULL\r\n");
    }
    NWY_CLI_LOG("event is %d", param->event);
    if (NWY_HTTP_DNS_ERR == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP dns err");
    }
    else if (NWY_HTTP_OPEN_FAIL == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP open fail");
    }
    else if (NWY_HTTP_OPENED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP setup success");
    }
    else if (NWY_HTTP_CLOSED_PASV == param->event || NWY_HTTP_CLOSED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP closed");
    }
    else if (NWY_HTTP_DATA_RECVED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP recv data len %d.\r\n", param->data_len);
        char *buff = (char *)malloc(NWY_EXT_SIO_RX_MAX + 1);
        if (NULL == buff)
        {
            nwy_test_cli_echo("%s\r\n", param->data);
            return;
        }
        int offset = 0;
        int tmeplen = 0;
        while (param->data_len)
        {
            memset(buff, 0x00, NWY_EXT_SIO_RX_MAX + 1);
            tmeplen = param->data_len > NWY_EXT_SIO_RX_MAX ? NWY_EXT_SIO_RX_MAX : param->data_len;
            memcpy(buff, param->data + offset, tmeplen);
            nwy_test_cli_echo("%s", buff);
            offset += tmeplen;
            param->data_len -= tmeplen;
            NWY_CLI_LOG("param->data_len %d", param->data_len);
            nwy_sleep(100);
        }
        free(buff);
        nwy_test_cli_echo("\r\n");
    }
    else if (NWY_HTTP_DATA_SEND_ERR == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP request send error");
    }
    else if (NWY_HTTP_DATA_SEND_FINISHED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP request send OK");
    }
    else
    {
        nwy_test_cli_echo("\r\nHTTP unkown error");
    }
    return;
}

static void nwy_cli_https_result_cb(nwy_http_result_t *param)
{
    if (NULL == param)
    {
        nwy_test_cli_echo("event is NULL\r\n");
    }
    NWY_CLI_LOG("event is %d", param->event);
    if (NWY_HTTP_DNS_ERR == param->event)
    {
        nwy_test_cli_echo("\r\nHTTPS dns err");
    }
    else if (NWY_HTTP_OPEN_FAIL == param->event)
    {
        nwy_test_cli_echo("\r\nHTTPS open fail");
    }
    else if (NWY_HTTPS_SSL_CONNECTED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTPS setup success");
    }
    else if (NWY_HTTP_CLOSED_PASV == param->event || NWY_HTTP_CLOSED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTPS closed");
    }
    else if (NWY_HTTP_DATA_RECVED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTPS recv data len %d.\r\n", param->data_len);
        char *buff = (char *)malloc(NWY_EXT_SIO_RX_MAX + 1);
        if (NULL == buff)
        {
            nwy_test_cli_echo("%s\r\n", param->data);
            return;
        }
        int offset = 0;
        int tmeplen = 0;
        while (param->data_len)
        {
            memset(buff, 0x00, NWY_EXT_SIO_RX_MAX + 1);
            tmeplen = param->data_len > NWY_EXT_SIO_RX_MAX ? NWY_EXT_SIO_RX_MAX : param->data_len;
            memcpy(buff, param->data + offset, tmeplen);
            nwy_test_cli_echo("%s", buff);
            offset += tmeplen;
            param->data_len -= tmeplen;
            NWY_CLI_LOG("param->data_len %d", param->data_len);
            nwy_sleep(100);
        }
        free(buff);
        nwy_test_cli_echo("\r\n");
    }
    else if (NWY_HTTP_DATA_SEND_ERR == param->event)
    {
        nwy_test_cli_echo("\r\nHTTPS request send error");
    }
    else if (NWY_HTTP_DATA_SEND_FINISHED == param->event)
    {
        nwy_test_cli_echo("\r\nHTTP request send ok");
    }
    else if (NWY_HTTPS_SSL_INIT_ERROR == param->event)
    {
        nwy_test_cli_echo("\r\nHTTPS SSL init fail");
    }
    else if (NWY_HTTPS_SSL_HANDSHAKE_ERROR == param->event)
    {
        nwy_test_cli_echo("\r\nHTTPS SSL handshare fail");
    }
    else if (NWY_HTTPS_SSL_AUTH_FAIL == param->event)
    {
        nwy_test_cli_echo("\r\nHTTPS SSL Authentication fail");
    }
    else
    {
        nwy_test_cli_echo("\r\nHTTPS unkown error");
    }
    return;
}

void nwy_test_cli_http_setup()
{
    int result = NWY_GEN_E_UNKNOWN;
    int port = 0;
    uint16_t channel = 0;
    char url[NWY_HTTP_URL_MAX_LEN] = {0};
    char *sptr = NULL;
    int ishttps = 0;
    boolean s_nwy_http_choice = true;
    nwy_app_ssl_conf_t ssl_cfg;
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input url:");
    strcpy(url, sptr);
    sptr = nwy_test_cli_input_gets("\r\n input port:");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("input error");
        return;
    }
    port = nwy_cli_str_to_int(sptr);

    sptr = nwy_test_cli_input_gets("\r\n input 0 for http, 1 for https:");
    ishttps = nwy_cli_str_to_int(sptr);
    if(ishttps != 0 && ishttps !=1)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    s_nwy_http_choice = ishttps ? true : false;
    
    if (s_nwy_http_choice)
    {
        memset(&ssl_cfg, 0x00, sizeof(ssl_cfg));
        sptr = nwy_test_cli_input_gets("\r\n input ssl version: 0:SSL3.0 1:TLS1.0 2:TLS1.1 3:TLS1.2");
        if ((nwy_cli_str_to_int(sptr) < 0) || (nwy_cli_str_to_int(sptr) > 3))
        {
            nwy_test_cli_echo("\r\nInvaild ssl version: %d ", nwy_cli_str_to_int(sptr));
            return;
        }
        ssl_cfg.ssl_version = (nwy_ssl_version_type_e)nwy_cli_str_to_int(sptr);
        sptr = nwy_test_cli_input_gets("\r\n input authmode: 0:No authentication 1:Manage server authentication 2:Manage server and client authentication");
        ssl_cfg.authmode = (nwy_ssl_auth_mode_e)nwy_cli_str_to_int(sptr);
        if (0 != ssl_cfg.authmode)
        {
            sptr = nwy_test_cli_input_gets("\r\n input cacert name:");
            strcpy(ssl_cfg.cacert.cert_name, sptr);
            if(ssl_cfg.authmode == 2)
            {
                sptr = nwy_test_cli_input_gets("\r\n input clientcert name:");
                strcpy(ssl_cfg.clientcert.cert_name, sptr);
                sptr = nwy_test_cli_input_gets("\r\n input clientkey name:");
                strcpy(ssl_cfg.clientkey.cert_name, sptr);
            }
        }
        result = nwy_https_setup(channel, url, port, nwy_cli_https_result_cb, &ssl_cfg);
    }
    else
    {
        result = nwy_http_setup(channel, url, port, nwy_cli_http_result_cb);
    }
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n %s success", s_nwy_http_choice ? "nwy_https_setup" : "nwy_http_setup");
    }
    else
    {
        nwy_test_cli_echo("\r\n %s fail", s_nwy_http_choice ? "nwy_https_setup" : "nwy_http_setup");
    }
}
void nwy_test_cli_http_get()
{
    char *sptr = NULL;
    int keepalive = 0;
    int offset = 0;
    int size = 0;
    int ishttps = 0;
    int result = NWY_GEN_E_UNKNOWN;
    uint16_t channel = 0;
    boolean s_nwy_http_choice = true;
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input whether keepalive:(0 1)");
    keepalive = nwy_cli_str_to_int(sptr);
    if ((keepalive < 0) || (keepalive > 1))
    {
        nwy_test_cli_echo("\r\nInvaild keepalive: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input offset:");
    offset = nwy_cli_str_to_int(sptr);
    sptr = nwy_test_cli_input_gets("\r\n input size:");
    size = nwy_cli_str_to_int(sptr);
    
    sptr = nwy_test_cli_input_gets("\r\n input 0 for http, 1 for https:");
    ishttps = nwy_cli_str_to_int(sptr);
    if(ishttps != 0 && ishttps !=1)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    s_nwy_http_choice = ishttps ? true : false;
    
    result = nwy_http_get(channel,keepalive, offset, size, s_nwy_http_choice);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_http_get success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_http_get fail");
    }
}
void nwy_test_cli_http_head()
{
    char *sptr = NULL;
    int keepalive = 0;
    int ishttps = 0;
    int result = NWY_GEN_E_UNKNOWN;
    uint16_t channel = 0;
    boolean s_nwy_http_choice = true;
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input whether keepalive:(0 1)");
    keepalive = nwy_cli_str_to_int(sptr);
    if ((keepalive < 0) || (keepalive > 1))
    {
        nwy_test_cli_echo("\r\nInvaild keepalive: %d ", channel);
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input 0 for http, 1 for https:");
    ishttps = nwy_cli_str_to_int(sptr);
    if(ishttps != 0 && ishttps !=1)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    s_nwy_http_choice = ishttps ? true : false;

    result = nwy_http_head(channel,keepalive, s_nwy_http_choice);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_http_head success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_http_head fail");
    }
}
void nwy_test_cli_http_post()
{
    char *sptr = NULL;
    int keepalive = 0;
    int type = 0;
    int result = NWY_GEN_E_UNKNOWN;
    uint8_t channel = 0;
    int ishttps = 0;
    boolean s_nwy_http_choice = true;
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input whether keepalive:(0 1)");
    keepalive = nwy_cli_str_to_int(sptr);
    if ((keepalive < 0) || (keepalive > 1))
    {
        nwy_test_cli_echo("\r\nInvaild keepalive: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input type: 0:x-www-form-urlencoded 1:text 2:json 3:xml 4:html 99:User-defined mode");
    type = nwy_cli_str_to_int(sptr);
    if(type != 0 && type !=1 && type !=2 && type !=3 && type !=4  && type !=99)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input 0 for http, 1 for https:");
    ishttps = nwy_cli_str_to_int(sptr);
    if(ishttps != 0 && ishttps !=1)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    s_nwy_http_choice = ishttps ? true : false;

    sptr = nwy_test_cli_input_gets("\r\n input data:");
    result = nwy_http_post(channel,keepalive, type, sptr, strlen(sptr), s_nwy_http_choice);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_http_post success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_http_post fail");
    }
}
#ifdef NWY_OPEN_TEST_HTTPPUT
void nwy_test_cli_http_put()
{
    char *sptr = NULL;
    int keepalive = 0;
    int type = 0;
    int result = NWY_GEN_E_UNKNOWN;
    uint8_t channel = 0;
    int ishttps = 0;
    boolean s_nwy_http_choice = true;
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input whether keepalive:(0 1)");
    keepalive = nwy_cli_str_to_int(sptr);
    if ((keepalive < 0) || (keepalive > 1))
    {
        nwy_test_cli_echo("\r\nInvaild keepalive: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input type: 0:x-www-form-urlencoded 1:text 2:json 3:xml 4:html 99:User-defined mode");
    type = nwy_cli_str_to_int(sptr);
    if(type != 0 && type !=1 && type !=2 && type !=3 && type !=4  && type !=99)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    sptr = nwy_test_cli_input_gets("\r\n input 0 for http, 1 for https:");
    ishttps = nwy_cli_str_to_int(sptr);
    if(ishttps != 0 && ishttps !=1)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    s_nwy_http_choice = ishttps ? true : false;
    sptr = nwy_test_cli_input_gets("\r\n input data:");
    result = nwy_http_put(channel,keepalive, type, sptr, strlen(sptr), s_nwy_http_choice);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_http_post success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_http_post fail");
    }
}
#endif

void nwy_test_cli_http_close()
{
    int result = NWY_GEN_E_UNKNOWN;
    char *sptr = NULL;
    uint8_t channel = 0;
    int ishttps = 0;
    boolean s_nwy_http_choice = true;
    sptr = nwy_test_cli_input_gets("\r\n input channel:(1-7)");
    channel = nwy_cli_str_to_int(sptr);
    if ((channel <= 0) || (channel > 7))
    {
        nwy_test_cli_echo("\r\nInvaild channel: %d ", channel);
        return;
    }

    sptr = nwy_test_cli_input_gets("\r\n input 0 for http, 1 for https:");
    ishttps = nwy_cli_str_to_int(sptr);
    if(ishttps != 0 && ishttps !=1)
    {
        nwy_test_cli_echo("\r\n input error");
        return;
    }
    s_nwy_http_choice = ishttps ? true : false;
    result = nwy_http_close(channel,s_nwy_http_choice);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n %s success", s_nwy_http_choice ? "nwy_https_close" : "nwy_http_close");
    }
    else
    {
        nwy_test_cli_echo("\r\n %s fail", s_nwy_http_choice ? "nwy_https_close" : "nwy_http_close");
    }
}
void nwy_test_cli_https_add_cert()
{
    char *sptr = NULL;
    char filename[128+1] = {0};

    sptr = nwy_test_cli_input_gets("\r\n begin input cert name(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ncert name error");
        return;
    }
    strcpy(filename, sptr);
    sptr = nwy_test_cli_input_gets("\r\n input cert length(1-4096):");
    if (!nwy_cli_check_str_isdigit(sptr))
    {
        nwy_test_cli_echo("input error");
        return;
    }
    uint32_t length =  nwy_cli_str_to_int(sptr);
    if (length < 1 || length>4096)
    {
        nwy_test_cli_echo("\r\n invalid cert size:%d", length);
        return;
    }

    s_nwy_trans_recv = malloc(sizeof(dataRecvContext_t) + length + CERT_EFS_CHECKSUM_STR_SIZE +1);
    if (s_nwy_trans_recv == NULL)
    {
        nwy_test_cli_echo("\r\n malloc buffer fail");
        return;
    }
    memset(s_nwy_trans_recv, 0, sizeof(dataRecvContext_t) + length+ CERT_EFS_CHECKSUM_STR_SIZE +1);
    s_nwy_trans_recv->size = length;
    s_nwy_trans_recv->pos = 0;
    nwy_test_cli_sio_enter_trans_mode(nwy_cli_data_trans_proc);
    nwy_test_cli_echo("\r\nplease input data");
    if(nwy_test_cli_wait_trans_end())
    {
        unsigned wsize = nwy_cert_add(filename, s_nwy_trans_recv->data, s_nwy_trans_recv->size);
        if (wsize != s_nwy_trans_recv->size)
        {
            nwy_test_cli_echo("\r\nnwy_cert_add fail");
        }
        else
        {
            nwy_test_cli_echo("\r\nnwy_cert_add success");
        }
    }
    free(s_nwy_trans_recv);
    s_nwy_trans_recv = NULL;

    nwy_test_cli_echo("\r\nadd end");
}
void nwy_test_cli_https_check_cert()
{
    char filename[128+1] = {0};
    char *sptr = NULL;
    int result = NWY_GEN_E_UNKNOWN;
    sptr = nwy_test_cli_input_gets("\r\n input cert name(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ncert name error");
        return;
    }
    memset(filename, 0x00, sizeof(filename));
    strcpy(filename, sptr);
    result = nwy_cert_check(filename);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_cert_check success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_cert_check fail");
    }
}
void nwy_test_cli_https_delet_cert()
{
    char filename[128+1] = {0};
    char *sptr = NULL;
    int result = NWY_GEN_E_UNKNOWN;
    sptr = nwy_test_cli_input_gets("\r\n input cert name(<=128):");
    if(strlen(sptr)>128)
    {
        nwy_test_cli_echo("\r\ncert name error");
        return;
    }
    memset(filename, 0x00, sizeof(filename));
    strcpy(filename, sptr);
    result = nwy_cert_del(filename);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_cert_del success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_cert_del fail");
    }
}
#ifdef NWY_OPEN_TEST_LITE_RWS

/*--------------------------------------------websocket-----------------------------------------------------------------------*/
static void nwy_websocket_test_received_cb(void * socket, const void *data, const unsigned int length,void *arg) 
{

    char *ocpp_resp  = NULL;
    if(data != NULL && length > 0)
    {
        ocpp_resp = malloc(length+1);
        memset(ocpp_resp,0,length+1);
        memcpy(ocpp_resp,data,length);
        NWY_CLI_LOG("nwy_onocpp_socket_received_cb: data =%d %s",length, ocpp_resp);
        nwy_test_cli_echo("\r\n websocket recv:%s", ocpp_resp);
        free(ocpp_resp);
    }
    else
    {
         NWY_CLI_LOG("nwy_onocpp_socket_received_cb error");
         nwy_test_cli_echo("\r\n websocket recv :%s", "error");
    }
}
static void nwy_websocket_state_cb(void *socket,int reason, void *arg) 
{
    NWY_CLI_LOG("nwy_ocpp:nwy_websocket_state_cb reason =%d\n",reason);

    if(reason == NWY_WS_CONNECTED)
    {
        nwy_test_cli_echo("\r\n websocket state :%s", "NWY_WS_CONNECTED");
    }
    else
    {
         nwy_test_cli_echo("\r\n websocket state :%s", "NWY_WS_DISCONNECTED");
    }

}

static void *nwy_websocket_test_hdl = NULL;

void nwy_test_cli_websocket_setup()
{
     int result = NWY_GEN_E_UNKNOWN;
     nwy_test_cli_echo("websocket_setup entry");
     nwy_rws_opt_t opt;
     nwy_rws_ssl_conf_type ssl_conf;
     char *url="ws://123.139.59.166:10122/v1";//Websocket test server 
     char *websocket_protocol="ocpp1.6";//Websocket sub protocol, using ocpp1.6
     char *custer_header="Authorization: Basic YWt4eTAxNmZhN2VaVTZTdTp2emtwY2dVREJTU21rYUxn";//Websocket sub protocol, using ocpp1.6
     memset(&opt,0,sizeof(nwy_rws_opt_t));
     memset(&ssl_conf, 0, sizeof(nwy_rws_ssl_conf_type));
    
     NWY_CLI_LOG("nwy_ocpp_start start\n");
     opt.ping_interval = 60;
     opt.ping_timout = 10;
     opt.should_handshake = 1;
     opt.proct = websocket_protocol;
     opt.ssl_conf = NULL;
     opt.expand_header = custer_header;
     nwy_websocket_test_hdl = nwy_ws_open(url,&opt,nwy_websocket_test_received_cb,nwy_websocket_state_cb,NULL);
     if(nwy_websocket_test_hdl == NULL)
     {
         nwy_test_cli_echo("\r\n %s fail", "nwy_ws_open");
         return -1;
     }
     NWY_CLI_LOG("nwy_ocpp_start OK\n");

     nwy_test_cli_echo("\r\n %s success", "nwy_ws_open");

}
void nwy_test_cli_websocket_send()
{
    int result = 0;
    char ws_data[128] = {0};
    static int websocket_index = 0;
    nwy_test_cli_echo("\r\nwebsocket_send entry");
    snprintf(ws_data,sizeof(ws_data),"ni hao websocket:%d",websocket_index);
    result =  nwy_rws_send_text(nwy_websocket_test_hdl,ws_data);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_rws_send_text success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_rws_send_text fail");
    }
    websocket_index++;
}
void nwy_test_cli_websocket_close()
{
    int result = NWY_GEN_E_UNKNOWN;

    result = nwy_rws_close(nwy_websocket_test_hdl);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n %s success", "nwy_rws_close");
    }
    else
    {
        nwy_test_cli_echo("\r\n %s fail", "nwy_rws_close");
    }
}
void nwy_test_cli_websocket_memckeck()
{
#ifdef NWY_OPEN_TEST_HEAP_INFO
        char heapinfo[100] = {0};
    
        nwy_dm_get_device_heapinfo(heapinfo);
        nwy_test_cli_echo("\r\n dev heapinfo:%s\r\n", heapinfo);
#else
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#endif

}
#endif

#ifdef NWY_OPEN_TEST_NTP_TIME
void nwy_ntp_time_cb(void* handle, nwy_updatetime_errid err_code, nwy_time_t *time)
{
    switch(err_code)
    {
        case NWY_UPDATETIME_SDK_SUCCESS:
            nwy_test_cli_echo("\r\nupdatetime:%02d/%02d/%02d,%02d:%02d:%02d", time->year, time->mon, time->day,
                                time->hour, time->min, time->sec);
            break;
        case NWY_UPDATETIME_IP_SDK_ERR:
            nwy_test_cli_echo("\r\nsocket error");
            break;
        case NWY_UPDATETIME_SEND_SDK_ERR:
            nwy_test_cli_echo("\r\nsend error");
            break;
        case NWY_UPDATETIME_RECV_SDK_TIMEOUT:
            nwy_test_cli_echo("\r\nrequest timeout");
            break;
        case NWY_UPDATETIME_RECV_SDK_ERR:
            nwy_test_cli_echo("\r\nrecv error");
            break;
        default:
            nwy_test_cli_echo("\r\nunkown error");
            break;
    }
    return ;
}
#endif
void nwy_test_cli_ntp_time()
{
#ifndef NWY_OPEN_TEST_NTP_TIME
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char ntp_server[128];
    int port;
    char *sptr = NULL;
    int result = NWY_GEN_E_UNKNOWN;
    sptr = nwy_test_cli_input_gets("\r\n input ntp server:");
    memset(ntp_server, 0x00, sizeof(ntp_server));
    strcpy(ntp_server, sptr);
    sptr = nwy_test_cli_input_gets("\r\n input ntp port:");
    port = nwy_cli_str_to_int(sptr);
    result = nwy_updatetime_ntp(ntp_server, 10*1000, NULL, NULL, nwy_ntp_time_cb);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_updatetime_ntp success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_updatetime_ntp fail");
    }
#endif
}


#ifdef NWY_OPEN_TEST_PING
void nwy_ping_cb(void* param)
{
    nwy_ping_cb_msg_t *cb_msg = (nwy_ping_cb_msg_t *)param;
    float percentage = 0;
    char buff[256] = {0};
    switch (cb_msg->status)
    {
        case NWY_PING_ST_SOCKET_ERR:
            nwy_test_cli_echo("\r\n PING SOCKET ERROR");
            break;
        case NWY_PING_ST_DNS_ERR:
            nwy_test_cli_echo("\r\n PING DNS ERROR");
            break;
        case NWY_PING_ST_REPLY_TIMEOUT:
            nwy_test_cli_echo("\r\n PING TIMEOUT");
            break;
        case NWY_PING_ST_ALL_FINISH:
            if(cb_msg->fin_stats_info.num_pkts_sent > 0)
                percentage = (float)(cb_msg->fin_stats_info.num_pkts_lost)/(float)cb_msg->fin_stats_info.num_pkts_sent;
            memset(buff, 0, 256);
            nwy_test_cli_echo("\r\nPING: finish,Packets:Sent = %lu, Received = %lu, Lost = %lu (%d%%loss)\r\n",
                    cb_msg->fin_stats_info.num_pkts_sent, cb_msg->fin_stats_info.num_pkts_recvd, 
                    cb_msg->fin_stats_info.num_pkts_lost, (int)(percentage*100));
            nwy_test_cli_echo("\r\nPING: RTT statistics: Minimum = %lu, Maximum = %lu, Avergae = %lu\r\n",
                           cb_msg->fin_stats_info.min_rtt,
                           cb_msg->fin_stats_info.max_rtt,
                           cb_msg->fin_stats_info.avg_rtt);
            break;
        default:
            break;
    }
    return ;
}
#endif
void nwy_test_cli_ping()
{
#ifndef NWY_OPEN_TEST_PING
        nwy_test_cli_echo("\r\nOption not Supported!\r\n");
#else
    char *sptr = NULL;
    int result = NWY_GEN_E_UNKNOWN;
    nwy_ping_param_type ping_param;
    memset(&ping_param, 0, sizeof(nwy_ping_param_type));
    sptr = nwy_test_cli_input_gets("\r\n input ping url:");
    strcpy(ping_param.remote_addr, sptr);
    sptr = nwy_test_cli_input_gets("\r\n input ping payload(8-1460):");
    ping_param.payload = nwy_cli_str_to_int(sptr);
    if(ping_param.payload < 8 || ping_param.payload > 1460)
    {
        nwy_test_cli_echo("\r\n ping_param payload error");
        return ;
    }
    sptr = nwy_test_cli_input_gets("\r\n input ping count:");
    ping_param.count = nwy_cli_str_to_int(sptr);
    sptr = nwy_test_cli_input_gets("\r\n input ping timout(s):");
    ping_param.timeout = nwy_cli_str_to_int(sptr) * 1000;
    sptr = nwy_test_cli_input_gets("\r\n input ping 4-ipv4, 6-ipv6");
    ping_param.ip_type = nwy_cli_str_to_int(sptr);
    if((ping_param.ip_type !=4) && (ping_param.ip_type !=6) )
    {
        nwy_test_cli_echo("\r\n ping_param ip_type error");
        return ;
    }
    sptr = nwy_test_cli_input_gets("\r\n input ping interval(s):");
    ping_param.interval = nwy_cli_str_to_int(sptr) * 1000;
    ping_param.debug_switch = 0;
    result = nwy_ping_start(&ping_param, nwy_ping_cb);
    if (0 == result)
    {
        nwy_test_cli_echo("\r\n nwy_ping_start success");
    }
    else
    {
        nwy_test_cli_echo("\r\n nwy_ping_start fail");
    }
#endif
}
