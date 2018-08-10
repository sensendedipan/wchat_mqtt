/**
******************************************************************************
* @file    			httputil.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-02-14
* @brief   	  	    http����Ҫ��ʵ�ú���
******************************************************************************
*/
#include "stdio.h"
#include "string.h"
#include "eeprom.h"
#include "webutil.h"
#include "webpge.h"
#include "utility.h"
#include "at24cx_driver.h"
#include "debug.h"
#include "main.h"

#define DEBUG_WEB


extern char http_tx_buf[MAX_URI_SIZE];
extern char http_rx_buf[MAX_URI_SIZE];

static bool flag_can_login = false;

/**
*@brief		���http��Ӧ
*@param		��
*@return	��
*/
void runWebServer(void)
{
	uint16_t len;	
	st_http_request *http_request;									/*����һ���ṹָ��*/
	memset(http_rx_buf, 0x00, MAX_URI_SIZE);
	http_request = (st_http_request*)http_rx_buf;
	
	/* http service start */	
    switch(getSn_SR(SOCK_HTTPS))									/*��ȡsocket״̬*/
    {
    case SOCK_INIT:													/*socket���ڳ�ʼ��״̬*/
        listen(SOCK_HTTPS);
        break;
    
    case SOCK_LISTEN:												/*socket���ڼ���״̬*/
        break;
    
    case SOCK_ESTABLISHED:											/*socket��������״̬*/
        if (getSn_IR(SOCK_HTTPS) & Sn_IR_CON) {
            setSn_IR(SOCK_HTTPS, Sn_IR_CON);						/*����жϱ�־λ*/
        }
        
        if ((len = getSn_RX_RSR(SOCK_HTTPS)) > 0) {
            len = recv(SOCK_HTTPS, (uint8_t*)http_request, len); 	/*����http����*/
            *(((uint8_t*)http_request)+len) = 0;
            proc_http(SOCK_HTTPS, (uint8_t*)http_request);			/*����http���󲢷���http��Ӧ*/
            disconnect(SOCK_HTTPS);
        }
        break;
        
    case SOCK_CLOSE_WAIT:   										/*socket���ڵȴ��ر�״̬*/
        if ((len = getSn_RX_RSR(SOCK_HTTPS)) > 0) {
            len = recv(SOCK_HTTPS, (uint8_t*)http_request, len);	/*����http����*/      
            *(((uint8_t*)http_request)+len) = 0;
            proc_http(SOCK_HTTPS, (uint8_t*)http_request);			/*����http���󲢷���http��Ӧ*/
        }
        disconnect(SOCK_HTTPS);
        break;
        
    case SOCK_CLOSED:                   							/*socket���ڹر�״̬*/
        socket(SOCK_HTTPS, Sn_MR_TCP, 80, 0x00);   					/*��socket*/
        break;
    
    default:
        break;
    }
}




/**
*@brief		����http�����Ĳ�����http��Ӧ
*@param		s: http������socket
*@param		buf��������������
*@return	��
*/

static uint32_t FLASH_BASE_ADDR = ADDR_FLASH_TEMP;
void proc_http(SOCKET s, uint8_t* buf)
{
	char name[20] = {0}; 											
	char req_name[32] = {0x00,};													/*����һ��http��Ӧ���ĵ�ָ��*/
	unsigned long file_len = 0;														/*����http������ͷ�Ľṹ��ָ��*/
	uint16_t send_len = 0;
	char* http_response;

	st_http_request *http_request;
	memset(http_tx_buf, 0x00, MAX_URI_SIZE);
	http_response = http_rx_buf;
	http_request = (st_http_request*)http_tx_buf;
	parse_http_request(http_request, buf);    										/*����http������ͷ*/

    switch (http_request->METHOD)		
    {
    case METHOD_ERR:															    /*������ͷ����*/
        memcpy(http_response, ERROR_REQUEST_PAGE, sizeof(ERROR_REQUEST_PAGE));
        send(s, (uint8_t*)http_response, strlen((const char*)http_response));
        break;
    
    case METHOD_HEAD:															    /*HEAD����ʽ*/
        break;
    
    case METHOD_GET:															    /*GET����ʽ*/
        //name = http_request->URI; 
        memcpy(name, http_request->URI, strlen(http_request->URI)); 
        if (strcmp((char*)name, "/login.htm") == 0 || strcmp((char*)name, "/") == 0 || (strcmp((char*)name, "/login.html") == 0)) {
            file_len = strlen(LOGIN_HTML);    
            make_http_response_head((uint8_t*)http_response, PTYPE_HTML, file_len);
            send(s, (const uint8_t*)http_response, strlen((char const*)http_response));
            send_len = 0;
            while (file_len)
            {
                if (file_len > 1024) {
                    if (getSn_SR(s) != SOCK_ESTABLISHED) {
                        return;
                    }
                    send(s, (uint8_t*)LOGIN_HTML+send_len, 1024);
                    send_len += 1024;
                    file_len -= 1024;
                } else {
                    send(s, (uint8_t*)LOGIN_HTML+send_len, file_len);
                    send_len += file_len;
                    file_len -= file_len;
                } 
            }
        
        } else if (strcmp((char*)name, "/index.htm") == 0 || (strcmp((char*)name, "/index.html") == 0)) {
            if (flag_can_login == false) {
                sprintf((char*)http_response, "<html><script language=\"javascript\" type=\"text/javascript\">window.location.href='login.html';</script></html>");                                                       
                send(s, (unsigned char *)http_response, strlen((char *)http_response));	/*����http��Ӧ*/  
            
            } else {
                file_len = strlen(INDEX_HTML);   
                make_http_response_head((uint8_t*)http_response, PTYPE_HTML, file_len);
                send(s, (const uint8_t*)http_response, strlen((char const*)http_response));
                send_len = 0;
                while (file_len)
                {
                    if (file_len > 1024) {
                        if (getSn_SR(s) != SOCK_ESTABLISHED) {
                            return;
                        }
                        send(s, (uint8_t*)INDEX_HTML+send_len, 1024);
                        send_len += 1024;
                        file_len -= 1024;
                    } else {
                        send(s, (uint8_t*)INDEX_HTML+send_len, file_len);
                        send_len += file_len;
                        file_len -= file_len;
                    } 
                }
            }
            
        } else {
            memset(http_tx_buf, 0, MAX_URI_SIZE);
			http_get_cgi_handler(name, (uint8_t*)http_tx_buf);
            sprintf((char*)http_response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s", strlen(http_tx_buf), http_tx_buf);
            send(s, (unsigned char*)http_response, strlen((char const*)http_response));
        }
        break;
        
    case METHOD_POST:												/*POST����*/
        mid(http_request->URI, "/", " ", req_name);					/*��ȡ��������ļ���*/
        if (strcmp(req_name, "login.cgi") == 0) {
            if (loginCheck(http_request) == true) {                                     /*������֤ͨ��*/    
                flag_can_login = true;               
                sprintf((char*)http_response, "<html><script language=\"javascript\" type=\"text/javascript\">window.location.href='index.html';</script></html>");                                                       
                send(s, (unsigned char *)http_response, strlen((char *)http_response));	/*����http��Ӧ*/                 
            
            } else {                                                                    /*������֤δͨ��*/
                flag_can_login = false;
                sprintf((char*)http_response, "<html><script language=\"javascript\" type=\"text/javascript\">window.location.href='login.html';</script></html>");                                                       
                send(s, (unsigned char *)http_response, strlen((char *)http_response));	/*����http��Ӧ*/                
            }
            
        } else if (strcmp(req_name,"config.cgi") == 0) {					
            if (strstr((http_request->URI), "&NetSave")) {
			    save_net_config(http_request);               
				http_cgi_response(10, (char*)http_response, s);
               // w5500Init();
                systemReboot();
                
			} else if (strstr((http_request->URI), "&NetRst")) {
				http_cgi_response(10, (char*)http_response, s);
                systemReboot();
                
			} else {
                http_cgi_response(5, (char*)http_response, s);
            }	
            
		} else if (strcmp(req_name, "admin.cgi") == 0) {
			if (strstr((http_request->URI), "&adminSave")) {
				save_admin_config(http_request);	
				http_cgi_response_to_index(3, (char*)http_response, s);
                
			} else if (strstr((http_request->URI), "&adminRst")) {
				http_cgi_response(10, (char*)http_response, s);
                systemReboot();
                
			} else {
                http_cgi_response(5, (char*)http_response, s);
            }
            
		}        
        break;
        
    default:
        break;
    }
}




/**
*@brief		ִ��http��Ӧ
*@param		��  
*@return	��
*/
void make_cgi_response(uint16_t delay, char *url, char *cgi_response_buf)
{
	sprintf(cgi_response_buf, "<html><head><title>iWeb - Configuration</title><script language=javascript>j=%d;function func(){document.getElementById('delay').innerText=' '+j + ' ';j--;setTimeout('func()',1000);if(j==0)location.href='http://%d.%d.%d.%d/';}</script></head><body onload='func()'>���Եȣ��豸��������������<span style='color:red;' id='delay'></span> ���������Ĳ���������������</body></html>",delay,url[0],url[1],url[2],url[3]);
	return;
}



/**
*@brief		�ж��û����������Ƿ���ȷ
*@param		��  
*@return	�û������������ȷ����true ���򷵻�false
*/
bool loginCheck(st_http_request *http_request)
{ 
	static unsigned char *paramUser = NULL;
    static unsigned char *paramPassword = NULL;
    bool flagUser = false;
    bool flagPassword = false;
    uint8_t len_usr;
    uint8_t len_psd;
    uint8_t len_usr_ee;
    uint8_t len_psd_ee;
    
    len_usr_ee = (strlen((const char*)gEepromLgi.newusr) > 6) ? 6:strlen((const char*)gEepromLgi.newusr);
    len_psd_ee = (strlen((const char*)gEepromLgi.newpsd) > 6) ? 6:strlen((const char*)gEepromLgi.newpsd);
	
	paramUser = get_http_param_value(http_request->URI, "user");
	if (paramUser) {        
        len_usr = (strlen((const char*)paramUser) > 6) ? 6:strlen((const char*)paramUser);        
        if (len_usr != len_usr_ee) return false;
                
        for (uint8_t i = 0; i < len_usr; i++) {
            if (gEepromLgi.newusr[i] != *(paramUser+i)) {
                flagUser = false;
                break;
                
            } else if (i == len_usr-1) {
                flagUser = true;
            }
        }                
	}
	
	paramPassword = get_http_param_value(http_request->URI, "password");
	if (paramPassword) {
        len_psd = (strlen((const char*)paramPassword) > 6) ? 6:strlen((const char*)paramPassword);
        if (len_psd != len_psd_ee) return false;
        
        for (uint8_t i = 0; i < len_psd; i++) {
            if (gEepromLgi.newpsd[i] != *(paramPassword+i)) {
                flagPassword = false;
                break;
                
            } else if (i == len_psd-1) {
                flagPassword = true;
            }
        }        
	}
#ifdef DEBUG_WEB   
    if (flagUser && flagPassword)printf("login ok \n");
#endif
    return flagUser && flagPassword;
}






//////////////////////////////////////////////////////////////////////////////////////////////////

/**
*@brief		Http web��ȡcgi����
*@param		
*@return	��
*/
static void http_get_cgi_handler(char* uri_name, uint8_t* buf)
{ 
    uint8_t ret = HTTP_OK;
    uint16_t len = 0;
    
    if (strcmp(uri_name, "/w5500.js") == 0) {
        make_basic_config_setting_json_callback((char*)buf);
        
    } else if (strcmp(uri_name, "/config.cgi") == 0) {
        make_basic_config_setting_json_callback((char*)buf);
          
    } else if (strcmp(uri_name, "/admin.cgi") == 0) {
        
    } else if (strcmp(uri_name, "/img/favicon.ico") == 0) {
        
    } else {
        
    }
}



/**
*@brief		��������������Ϣ���õ�json_callback
*@param		��
*@return	��
*/
static void make_basic_config_setting_json_callback(char* buf)
{	
    bool dev_enable_state = false;
	sprintf(buf, "Callback({\"dhcp\":\"%d\",\"mac\":\"%02X:%02X:%02X:%02X:%02X:%02X\",\"ip\":\"%d.%d.%d.%d\",\"gw\":\"%d.%d.%d.%d\",\
							\"sub\":\"%d.%d.%d.%d\",\"dns\":\"%d.%d.%d.%d\",\"port\":\"%d\",\"baud\":\"%d\",\
	                        \"Rdns\":\"%d\",\"Rurl\":\"%s\",\"Rip\":\"%d.%d.%d.%d\",\"Rport\":\"%d\"});",

/*dhcp*/		gEepromLnet.dhcp,
/*mac*/	        myMac[0], myMac[1], myMac[2], myMac[3], myMac[4], myMac[5],
/*l-ip*/   	    gEepromLnet.ip[0], gEepromLnet.ip[1], gEepromLnet.ip[2], gEepromLnet.ip[3],
/*gw*/ 		    gEepromLnet.gay[0],gEepromLnet.gay[1],gEepromLnet.gay[2],gEepromLnet.gay[3],
/*sub*/ 		gEepromLnet.sub[0],gEepromLnet.sub[1],gEepromLnet.sub[2],gEepromLnet.sub[3],
/*dns*/ 		gEepromLnet.dns[0],gEepromLnet.dns[1],gEepromLnet.dns[2],gEepromLnet.dns[3],
/*port*/ 		gEepromLnet.port, gEepromLnet.baud,
    
                gEepromMqtt.dns, gEepromMqtt.url, //Զ�̷�����������
				gEepromMqtt.ip[0],gEepromMqtt.ip[1],gEepromMqtt.ip[2],gEepromMqtt.ip[3], gEepromMqtt.port
               
			   );
}




/**
*@brief		��������������Ϣ���õ�json_callback
*@param		��
*@return	��
*/
static void make_status_json_callback(char* buf)
{ 

}





/**
*@brief		
*@param		
*@return	
*/
void http_cgi_response(uint16_t delay, char* http_response, SOCKET s)
{
	make_cgi_response(delay, (char*)(gEepromLnet.ip), http_tx_buf);	/*������Ӧ���ı�����*/
	sprintf((char*)http_response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s", strlen(http_tx_buf), http_tx_buf);
	send(s, (unsigned char*)http_response, strlen((char*)http_response));
	disconnect(s);	/*�Ͽ�socket����*/
}



void http_cgi_response_to_index(uint16_t delay, char* http_response, SOCKET s)
{
	make_cgi_response(delay, (char*)(gEepromLnet.ip), http_tx_buf);	/*������Ӧ���ı�����*/
    sprintf((char *)http_response, "<html><script language=\"javascript\" type=\"text/javascript\">window.location.href='index.html';</script></html>"); 
	send(s, (unsigned char*)http_response, strlen((char*)http_response));
	disconnect(s);	/*�Ͽ�socket����*/
}



/**
*@brief		
*@param		
*@return	
*/
static void save_admin_config(st_http_request *http_request)
{ 
	static uint8_t *param = NULL;
    uint8_t usr[6] = {0};
    uint8_t psd[6] = {0};
    uint8_t len_usr;
    uint8_t len_psd;
    uint8_t len_vsd;
    uint8_t len_usr_ee;
    uint8_t len_psd_ee;
    
    
	if (strstr(http_request->URI, "select=0")) {//! ��֤��ҳ����Ĺ���Աԭʼ�����Ƿ���豸��ǰ��������һ��
#ifdef DEBUG_WEB
        debug("config login usr and psd \n");
#endif         
        len_usr_ee = (strlen((const char*)gEepromLgi.newusr) > sizeof(gEepromLgi.newusr)) ? sizeof(gEepromLgi.newusr):strlen((const char*)gEepromLgi.newusr);
        len_psd_ee = (strlen((const char*)gEepromLgi.newpsd) > sizeof(gEepromLgi.newpsd)) ? sizeof(gEepromLgi.newpsd):strlen((const char*)gEepromLgi.newpsd);
   
        //! ��֤��ҳ�����ԭʼ�����Ƿ���豸��ǰ��������һ��
        param = get_http_param_value(http_request->URI, "admin");	
        if (param) {
            len_usr = (strlen((const char*)param) > sizeof(gEepromLgi.newusr)) ? sizeof(gEepromLgi.newusr):strlen((const char*)param);
            if (len_usr != len_usr_ee) {
                return;
            } //! ���Ȳ�ͬ �˳�              
            for (uint8_t i = 0; i < sizeof(gEepromLgi.newusr); i++) usr[i] = *(param+i); 
            
        } else {
            return;
        }        
        
        param = get_http_param_value(http_request->URI, "password");	
        if (param) {
            len_psd = (strlen((const char*)param) > sizeof(gEepromLgi.newpsd)) ? sizeof(gEepromLgi.newpsd):strlen((const char*)param);
            if (len_psd != len_psd_ee) {
                return;
            } //! ���Ȳ�ͬ �˳�         
            for (uint8_t i = 0; i < sizeof(gEepromLgi.newpsd); i++) psd[i] = *(param+i);

        } else {
            return;
        }        
        
        bool flag_usr = false;
        bool flag_psd = false;
        for (uint8_t i = 0; i < len_usr_ee; i++) {
            if (gEepromLgi.newusr[i] != usr[i]) {
                flag_usr = false;
                debug("login input old usr name unvalid \n");
                break;
                
            } else if (i == len_usr_ee-1) {
                flag_usr = true;
            }
        }     
        
        for (uint8_t i = 0; i < len_psd_ee; i++) {
            if (gEepromLgi.newpsd[i] != psd[i]) {
                flag_psd = false;
                //debug("login input old usr psd unvalid \n");
                break;
                
            } else if (i == len_psd_ee-1) {
                flag_psd = true;
            }
        }    

        if (flag_usr && flag_psd) {//! �����ԭʼ������ȷ        
            uint8_t newpsd[6] = {0}; //! �˴������þ�̬������Ҫ��֤ÿ�εĳ�ʼֵ����ȫ0������
            uint8_t valpsd[6] = {0};
            uint8_t newusr[6] = {0};
            uint8_t len_npassword;
            uint8_t len_nnpassword;
            
            param = get_http_param_value(http_request->URI, "npassword");
            if (param) {
                len_npassword = (strlen((const char*)param) > sizeof(gEepromLgi.newpsd)) ? sizeof(gEepromLgi.newpsd):strlen((const char*)param);
                for (uint8_t i = 0; i < len_npassword; i++) {
                    newpsd[i] = *(param+i);
                }
                //debug("login npassword: %s %d \n", param, len_npassword); 
                
            } else {
                return;
            }

            
            param = get_http_param_value(http_request->URI, "nnpassword");
            if (param) {
                len_nnpassword = (strlen((const char*)param) > sizeof(gEepromLgi.newpsd)) ? sizeof(gEepromLgi.newpsd):strlen((const char*)param);        
                for (uint8_t i = 0; i < len_nnpassword; i++) {
                    valpsd[i] = *(param+i);
                } 
                //debug("login nnpassword: %s %d \n", param, len_nnpassword);  
                
            } else {
                return;
            }

            bool flag_psd_valid = false;
            if (len_npassword == len_nnpassword) {  //! ��������������볤��һ��
                for (uint8_t i = 0; i < len_npassword; i++) {
                    if (newpsd[i] != valpsd[i]) {
                        flag_psd_valid = false;
                        break;
                    
                    } else if (i == len_npassword-1) {
                        flag_psd_valid = true;
                    }
                } 
                
            } else {
                return; //! ��������������볤�Ȳ�һ�£� ֱ���˳�
            }
            
            if (flag_psd_valid) {  //! �����������������ȷ
                param = get_http_param_value(http_request->URI, "nadmin");
                if (param) {
                    uint8_t len = (strlen((const char*)param) > sizeof(gEepromLgi.newusr)) ? sizeof(gEepromLgi.newusr):strlen((const char*)param);
                    memcpy(newusr, param, len);
                    memcpy(gEepromLgi.newusr, newusr, sizeof(gEepromLgi.newusr));
                    memcpy(gEepromLgi.newpsd, newpsd, sizeof(gEepromLgi.newpsd));               
                    write_lgi_info_to_eeprom(gEepromLgi);
                    delay_ms(10);
                    read_lgi_info_from_eeprom(&gEepromLgi);
                }
                
            } else {
                return;
            }
            
        } else {
            return;
        }	
	
	} else if (strstr(http_request->URI, "select=1")) {//! ��֤��ҳ�����MQTTԭʼ�����Ƿ���豸��ǰ��������һ��
#ifdef DEBUG_WEB
        debug("config mqtt usr and psd \n");
#endif         
        len_usr_ee = (strlen((const char*)gEepromMlgi.usr) > sizeof(gEepromMlgi.usr)) ? sizeof(gEepromMlgi.usr):strlen((const char*)gEepromMlgi.usr);
        len_psd_ee = (strlen((const char*)gEepromMlgi.psd) > sizeof(gEepromMlgi.psd)) ? sizeof(gEepromMlgi.psd):strlen((const char*)gEepromMlgi.psd);
            
        //! ��֤��ҳ�����ԭʼ�����Ƿ���豸��ǰ��������һ��
        param = get_http_param_value(http_request->URI, "admin");	
        if (param) {
            len_usr = (strlen((const char*)param) > sizeof(gEepromMlgi.usr)) ? sizeof(gEepromMlgi.usr):strlen((const char*)param);
            if (len_usr != len_usr_ee) return; //! ���Ȳ�ͬ �˳�              
            for (uint8_t i = 0; i < sizeof(gEepromMlgi.usr); i++) usr[i] = *(param+i); 
            //debug("mqtt: old usr: %s \n", param);  
        } else {
            return;
        }        
        
        param = get_http_param_value(http_request->URI, "password");	
        if (param) {
            len_psd = (strlen((const char*)param) > sizeof(gEepromMlgi.psd)) ? sizeof(gEepromMlgi.psd):strlen((const char*)param);
            if (len_psd != len_psd_ee) return; //! ���Ȳ�ͬ �˳�         
            for (uint8_t i = 0; i < sizeof(gEepromMlgi.psd); i++) psd[i] = *(param+i);
           // debug("mqtt: old psd: %s \n", param);
        } else {
            return;
        }        
        
        bool flag_usr = false;
        bool flag_psd = false;
        for (uint8_t i = 0; i < len_usr_ee; i++) {
            if (gEepromMlgi.usr[i] != usr[i]) {
                flag_usr = false;
                //debug("mqtt: input old usr name unvalid \n");
                break;
                
            } else if (i == len_usr_ee-1) {
                flag_usr = true;
            }
        }     
        
        for (uint8_t i = 0; i < len_psd_ee; i++) {
            if (gEepromMlgi.psd[i] != psd[i]) {
                flag_psd = false;
                //debug("mqtt: input old usr psd unvalid \n");
                break;
                
            } else if (i == len_psd_ee-1) {
                flag_psd = true;
            }
        }    

        if (flag_usr && flag_psd) {//! �����ԭʼ������ȷ        
            uint8_t newpsd[6] = {0}; //! �˴������þ�̬������Ҫ��֤ÿ�εĳ�ʼֵ����ȫ0������
            uint8_t valpsd[6] = {0};
            uint8_t newusr[6] = {0};
            uint8_t len_npassword;
            uint8_t len_nnpassword;
            
            param = get_http_param_value(http_request->URI, "npassword");
            if (param) {
                len_npassword = (strlen((const char*)param) > sizeof(gEepromMlgi.psd)) ? sizeof(gEepromMlgi.psd):strlen((const char*)param);
                for (uint8_t i = 0; i < len_npassword; i++) {
                    newpsd[i] = *(param+i);
                }
                //debug("mqtt: npassword: %s %d \n", param, len_npassword); 
                
            } else {
                return;
            }

            
            param = get_http_param_value(http_request->URI, "nnpassword");
            if (param) {
                len_nnpassword = (strlen((const char*)param) > sizeof(gEepromMlgi.psd)) ? sizeof(gEepromMlgi.psd):strlen((const char*)param);        
                for (uint8_t i = 0; i < len_nnpassword; i++) {
                    valpsd[i] = *(param+i);
                } 
                //debug("mqtt: nnpassword: %s %d \n", param, len_nnpassword);  
                
            } else {
                return;
            }

            bool flag_psd_valid = false;
            if (len_npassword == len_nnpassword) {  //! ��������������볤��һ��
                for (uint8_t i = 0; i < len_npassword; i++) {
                    if (newpsd[i] != valpsd[i]) {
                        flag_psd_valid = false;
                        break;
                    
                    } else if (i == len_npassword-1) {
                        flag_psd_valid = true;
                    }
                } 
                
            } else {
                return; //! ��������������볤�Ȳ�һ�£� ֱ���˳�
            }
            
            if (flag_psd_valid) {  //! �����������������ȷ
                param = get_http_param_value(http_request->URI, "nadmin");
                if (param) {
                    uint8_t len = (strlen((const char*)param) > sizeof(gEepromMlgi.usr)) ? sizeof(gEepromMlgi.usr):strlen((const char*)param);
                    memcpy(newusr, param, len);
                    memcpy(gEepromMlgi.usr, newusr, sizeof(gEepromMlgi.usr));
                    memcpy(gEepromMlgi.psd, newpsd, sizeof(gEepromMlgi.psd));               
                    write_mlgi_info_to_eeprom(gEepromMlgi);
                    delay_ms(10);
                    read_mlgi_info_from_eeprom(&gEepromMlgi);
                }
                
            } else {
                return;
            }
            
        } else {
            return;
        }		
	}
}




/**
*@brief		
*@param		
*@return	
*/
static void save_net_config(st_http_request *http_request)
{    
	static uint8_t *param = NULL;
	uint16_t port;

    uint8_t flag_mqtt_dns = DNS_DISABLE;
    
    //! lnet
    eeprom_lnet_t lnet;
    memcpy(&lnet, &gEepromLnet, sizeof(gEepromLnet));
    bool flag_lnet_update = false;
	param = get_http_param_value(http_request->URI, "dhcp");		
	if (((*param) == 0X30) || ((*param) == 0X31)) {
        lnet.dhcp = atoi((const char*)param);
        if (lnet.dhcp != gEepromLnet.dhcp) flag_lnet_update = true;
	}
    
	param = get_http_param_value(http_request->URI, "dns");	
	if (param) {
		inet_addr_((uint8_t*)param, lnet.dns);
        if (lnet.dns != gEepromLnet.dns) flag_lnet_update = true;
	}
    
	param = get_http_param_value(http_request->URI, "port");	
	if (param) {
        lnet.port = atoi((const char*)param);
        if (lnet.port > 65535) lnet.port = gEepromLnet.port;
        if (lnet.port != gEepromLnet.port) flag_lnet_update = true;
        debug("port: %d \n", lnet.port);
	}

	param = get_http_param_value(http_request->URI, "baud");	
	if (param) {
        lnet.baud = atoi((const char*)param);
        if (lnet.baud > 12000000) lnet.baud = gEepromLnet.baud;
        if (lnet.baud != gEepromLnet.baud) flag_lnet_update = true;
        debug("baud: %d \n", lnet.baud);
	}
    
	if (lnet.dhcp == IP_STATIC) {
		param = get_http_param_value(http_request->URI, "ip");		
		if (param) {
			inet_addr_((uint8_t*)param, lnet.ip);

            for (uint8_t i = 0; i < sizeof(lnet.ip); i++) {
                if (lnet.ip[i] != gEepromLnet.ip[i]) {
                    flag_lnet_update = true; 
                    break;                   
                }
            }            
		}
        
		param = get_http_param_value(http_request->URI, "gw");	
		if (param) {
			inet_addr_((uint8_t*)param, lnet.gay);

            for (uint8_t i = 0; i < sizeof(lnet.gay); i++) {
                if (lnet.gay[i] != gEepromLnet.gay[i]) {
                    flag_lnet_update = true;
                    break;                   
                }
            }            
		}
        
		param = get_http_param_value(http_request->URI, "sub");
		if (param) {
			inet_addr_((uint8_t*)param, lnet.sub);

            for (uint8_t i = 0; i < sizeof(lnet.sub); i++) {
                if (lnet.sub[i] != gEepromLnet.sub[i]) {
                    flag_lnet_update = true;
                    break;                   
                }
            }            
		}           
	}

    if (flag_lnet_update == true) {
        write_lnet_info_to_eeprom(lnet);//!
        delay_ms(10);
        read_lnet_info_from_eeprom(&gEepromLnet);       
    }
    
	//! mqtt
    eeprom_mqtt_t mqtt;
    memcpy(&mqtt, &gEepromMqtt, sizeof(gEepromMqtt));
    bool flag_mqtt_update = false;
	param = get_http_param_value(http_request->URI, "DomainName");	
	if (((*param) == 0X30) || ((*param) == 0X31)) {
        mqtt.dns = atoi((const char*)param);
        if (mqtt.dns != gEepromMqtt.dns) flag_mqtt_update = true;
	}
    
	if (mqtt.dns == DNS_ENABLE) {
	    param = get_http_param_value(http_request->URI, "Rurl");	
		if (param) { 
            uint8_t mqtt_url_len = (strlen((const char*)param) > sizeof(mqtt.url)) ? sizeof(mqtt.url):strlen((const char*)param);
            if (mqtt_url_len != 0) {
                memset(mqtt.url, 0, sizeof(mqtt.url));
                memcpy(mqtt.url, param, mqtt_url_len); 
                for (uint8_t i = 0; i < sizeof(mqtt.url); i++) {
                    if (mqtt.url[i] != gEepromMqtt.url[i]) { //!�ж��������Ƿ������������
                        flag_mqtt_update = true;  
                        break;                    
                    } 
                } 
            }            
		}
      
	} else {
		param = get_http_param_value(http_request->URI, "Rip");	
		if (param) {
            if (strlen((const char*)param) != 0) {
                inet_addr_((uint8_t*)param, mqtt.ip);

                for (uint8_t i = 0; i < sizeof(mqtt.ip); i++) {
                    if (mqtt.ip[i] != gEepromMqtt.ip[i]) {
                        flag_mqtt_update = true;
                        break;                   
                    }
                }
            }            
		}
	}    
           
    param = get_http_param_value(http_request->URI, "Rport");	
    if (param) {
        uint16_t port = atoi((const char*)param);
        if (port != 0) {
            mqtt.port = port;
            if (mqtt.port != gEepromMqtt.port) flag_mqtt_update = true;
        }
    }
        
    if (flag_mqtt_update == true) {
        write_mqtt_info_to_eeprom(mqtt);
        delay_ms(10);
        read_mqtt_info_from_eeprom(&gEepromMqtt);
    }
   
}






