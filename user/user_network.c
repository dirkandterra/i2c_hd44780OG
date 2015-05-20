/*
Tnx to Sprite_TM (source came from his esp8266ircbot)
*/

#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"
#include "user_network.h"
#include "user_config.h"



static char lineBuf[1024];
static int lineBufPos;
err_t err;
sint8 prob;
sint8 gotDNS=0;
uint8 toggle=0;
LOCAL os_timer_t network_timer;

//WiFi access point data
typedef struct {
	char ssid[32];
	char rssi;
	char enc;
} ApData;

//Scan result
typedef struct {
	char scanInProgress; //if 1, don't access the underlying stuff from the webpage.
	ApData **apData;
	int noAps;
} ScanResultData;

//Static scan status storage.
ScanResultData cgiWifiAps;

static void ICACHE_FLASH_ATTR networkParseLine(struct espconn *conn, char *line) {
	char buff[1024];
    uint8 page, y;
    page = line[0];
    y = line[1];
    char* data = line + 2;    
    os_printf("P-L: %x-%x: %s\n\r",page,y,data);
    display_data(page, y, data);
}

static void ICACHE_FLASH_ATTR networkParseChar(struct espconn *conn, char c) {
	lineBuf[lineBufPos++]=c;
	if (lineBufPos>=sizeof(lineBuf)) lineBufPos--;

	if (lineBufPos>2 && lineBuf[lineBufPos-1]=='\n') {
		lineBuf[lineBufPos-1]=0;
		networkParseLine(conn, lineBuf);
		lineBufPos=0;
	}
}

static void ICACHE_FLASH_ATTR networkRecvCb(void *arg, char *data, unsigned short len) {
	struct espconn *conn=(struct espconn *)arg;
	/*int x;
	for (x=0; x<len; x++) networkParseChar(conn, data[x]);*/
	int x;
	int ii;
	//!!LCD_clear();
    char page_buffer[20]="Data: ";

    char *token, *it;

    token= strtok(data,"~");
    while(token!=NULL){
    	it=token;
    	token=strtok(NULL,"~");
    }


    for(ii=0;ii<sizeof(it)&&ii<14;ii++){
    	page_buffer[ii+6]=it[ii];
    }
    //!! LCD_setCursor(0,0);
    //!! LCD_print(page_buffer);
    //!!LCD_setCursor(11,3);

	os_printf("Data: %s\r\n",it);
	if(it[0]=='4'){
		ExtIO_high();
	}
	else{
		ExtIO_low();

	}
	os_printf("Received from Server\n\r");
}

static void ICACHE_FLASH_ATTR networkConnectedCb(void *arg) {
	struct espconn *conn=(struct espconn *)arg;
	char *data= (char *)os_malloc(180);
	os_sprintf(data,"GET /dricker.onlinewebshop.net/files/tri.php?num=%c HTTP/1.1\r\nHost: f10-preview.awardspace.net\r\nUser-Agent: Arduino\r\nAccept: application/json\r\nConnection: close\r\n\r\n",toggle+0x30);

	if (toggle){
		toggle=0;
		//data = "GET /dricker.onlinewebshop.net/files/tri.php?num=1 HTTP/1.1\r\nHost: f10-preview.awardspace.net\r\nUser-Agent: Arduino\r\nAccept: application/json\r\nConnection: close\r\n\r\n";
	}
	else{
		toggle=1;
		//data = "GET /dricker.onlinewebshop.net/files/tri.php?num=0 HTTP/1.1\r\nHost: f10-preview.awardspace.net\r\nUser-Agent: Arduino\r\nAccept: application/json\r\nConnection: close\r\n\r\n";
	}

	sint8 d = espconn_sent(conn,data,strlen(data));

	 os_free(data);

	espconn_regist_recvcb(conn, networkRecvCb);
	/*lineBufPos=0;*/
    os_printf("connected to Server\n\r");
}

static void ICACHE_FLASH_ATTR networkReconCb(void *arg, sint8 err) {
    os_printf("Reconnect to Server: %d\n\r",err);
	network_init();
}

static void ICACHE_FLASH_ATTR networkDisconCb(void *arg) {
    os_printf("Disconnected from Server\n\r");
	network_init();
}

static void ICACHE_FLASH_ATTR networkServerFoundCb(const char *name, ip_addr_t *ip, void *arg) {
	static esp_tcp tcp;
	struct espconn *conn=(struct espconn *)arg;
	if (err==-5){
		os_printf("Busy with DNS request... failed\n");
		network_init();
		return;
	}
	if (ip==NULL) {
        os_printf("Nslookup failed :%d / Trying again...\n",err);
		network_init();
		return;
	}
	gotDNS=1;
    os_printf("Server at %d.%d.%d.%d\n",
        *((uint8 *)&ip->addr), *((uint8 *)&ip->addr + 1),
        *((uint8 *)&ip->addr + 2), *((uint8 *)&ip->addr + 3));

    char page_buffer[20];
    os_sprintf(page_buffer,"DST: %d.%d.%d.%d",
        *((uint8 *)&ip->addr), *((uint8 *)&ip->addr + 1),
        *((uint8 *)&ip->addr + 2), *((uint8 *)&ip->addr + 3));
    //!!LCD_setCursor(0,1);
    //!!LCD_print(page_buffer);
    //!! LCD_setCursor(11,1);
	conn->type=ESPCONN_TCP;
	conn->state=ESPCONN_NONE;
	conn->proto.tcp=&tcp;
	conn->proto.tcp->local_port=espconn_port();
	conn->proto.tcp->remote_port=80;
	os_memcpy(conn->proto.tcp->remote_ip, &ip->addr, 4);

	espconn_regist_connectcb(conn, networkConnectedCb);
	espconn_regist_disconcb(conn, networkDisconCb);
	espconn_regist_reconcb(conn, networkReconCb);
	espconn_connect(conn);
}

void ICACHE_FLASH_ATTR
network_start() {
	static struct espconn conn;
	static ip_addr_t ip;

	if (!gotDNS){
		os_printf("Looking up DNS...\n");
		//espconn_gethostbyname(&conn, "www.google.com", &ip, networkServerFoundCb);
		espconn_gethostbyname(&conn, "f10-preview.awardspace.net", &ip, networkServerFoundCb);
		//espconn_gethostbyname(&conn, "dricker.onlinewebshop.net", &ip, networkServerFoundCb);
	}
	else{
		os_printf("Connecting to server...\n");
		espconn_connect(&conn);
	}
}

void ICACHE_FLASH_ATTR ssidSearch(){
	int ii=0;
	struct station_config stationConf;
	char *ssidName;

	for (ii=0; ii<cgiWifiAps.noAps; ii++){
		ssidName=cgiWifiAps.apData[ii]->ssid;
		if(strcmp(ssidName,SSID)==0){
			os_memcpy(&stationConf.ssid, SSID, 32);
			os_memcpy(&stationConf.password, SSID_PWD, 32);
		    wifi_station_set_config(&stationConf);
		    os_printf("WIFI Config: %s\r\n" ,ssidName);

		}
		else if(strcmp(ssidName,SSID2)==0){
			os_printf("Found SSID: SSID2\r\n");
			os_memcpy(&stationConf.ssid, SSID2, 32);
			os_memcpy(&stationConf.password, SSID_PWD2, 32);
		    wifi_station_set_config(&stationConf);
		    os_printf("WIFI Config: %s\r\n" ,ssidName);
		}
		else{
			os_printf("SSID: %s\r\n",ssidName);
		}
	}
}


//Callback the code calls when a wlan ap scan is done. Basically stores the result in
//the cgiWifiAps struct.
void ICACHE_FLASH_ATTR wifiScanDoneCb(void *arg, STATUS status) {
	int n;
	struct bss_info *bss_link = (struct bss_info *)arg;
	os_printf("wifiScanDoneCb %d\n", status);
	if (status!=OK) {
		cgiWifiAps.scanInProgress=0;
		return;
	}

	//Clear prev ap data if needed.
	if (cgiWifiAps.apData!=NULL) {
		for (n=0; n<cgiWifiAps.noAps; n++) os_free(cgiWifiAps.apData[n]);
		os_free(cgiWifiAps.apData);
	}

	//Count amount of access points found.
	n=0;
	while (bss_link != NULL) {
		bss_link = bss_link->next.stqe_next;
		n++;
	}
	//Allocate memory for access point data
	cgiWifiAps.apData=(ApData **)os_malloc(sizeof(ApData *)*n);
	cgiWifiAps.noAps=n;
	os_printf("Scan done: found %d APs\n", n);

	//Copy access point data to the static struct
	n=0;
	bss_link = (struct bss_info *)arg;
	while (bss_link != NULL) {
		if (n>=cgiWifiAps.noAps) {
			//This means the bss_link changed under our nose. Shouldn't happen!
			//Break because otherwise we will write in unallocated memory.
			os_printf("Huh? I have more than the allocated %d aps!\n", cgiWifiAps.noAps);
			break;
		}
		//Save the ap data.
		cgiWifiAps.apData[n]=(ApData *)os_malloc(sizeof(ApData));
		cgiWifiAps.apData[n]->rssi=bss_link->rssi;
		cgiWifiAps.apData[n]->enc=bss_link->authmode;
		strncpy(cgiWifiAps.apData[n]->ssid, (char*)bss_link->ssid, 32);

		bss_link = bss_link->next.stqe_next;
		n++;
	}
	ssidSearch();
	//DR**
	if (cgiWifiAps.apData[1]!=NULL)
	{
		//!!LCD_clear();
		char page_buffer[20];
		os_sprintf(page_buffer,">%s",cgiWifiAps.apData[1]->ssid);
		//!!LCD_setCursor(0,0);
		//!!LCD_print(page_buffer);
	}
	//**
	//We're done.
	cgiWifiAps.scanInProgress=0;
}
//Routine to start a WiFi access point scan.
void ICACHE_FLASH_ATTR wifiStartScan() {
//	int x;
	os_printf("Start WIFI scan -> %d\r\n",cgiWifiAps.scanInProgress);
	if (cgiWifiAps.scanInProgress) return;
	cgiWifiAps.scanInProgress=1;
	wifi_station_scan(NULL, wifiScanDoneCb);
}

void ICACHE_FLASH_ATTR network_check_ip(void)
{
    struct ip_info ipconfig;

    os_timer_disarm(&network_timer);



    wifi_get_ip_info(STATION_IF, &ipconfig);


    if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0) {
    	os_printf("Getting IP...\r\n");
    	//!!LCD_clear();
        char page_buffer[20];
        os_sprintf(page_buffer,"IP: %d.%d.%d.%d",IP2STR(&ipconfig.ip));
        //!!LCD_setCursor(0,0);

        //!!LCD_print(page_buffer);
        //!!LCD_setCursor(11,3);
        network_start();
    }
    else
    {
    	wifiStartScan();
        os_printf("No ip found\n\r");
        os_timer_setfn(&network_timer, (os_timer_func_t *)network_check_ip, NULL);
        os_timer_arm(&network_timer, 15000, 0);
    }

}

void setup_wifi_ap_mode(void)
{
	wifi_set_opmode((wifi_get_opmode()|STATIONAP_MODE)&0x03);
	struct softap_config apconfig;
	if(wifi_softap_get_config(&apconfig))
	{
		wifi_softap_dhcps_stop();
		memset(apconfig.ssid, 0, sizeof(apconfig.ssid));
		memset(apconfig.password, 0, sizeof(apconfig.password));
		apconfig.ssid_len = os_sprintf(apconfig.ssid, "ESP");
		os_sprintf(apconfig.password, "%s", "00000000");
		apconfig.authmode = AUTH_WPA_WPA2_PSK;
		apconfig.ssid_hidden = 0;
		apconfig.channel = 7;
		apconfig.max_connection = 4;
		if(!wifi_softap_set_config(&apconfig))
		{
			#ifdef PLATFORM_DEBUG
			os_printf("ESP8266 not set AP config!\r\n");
			#endif
		};
		struct ip_info ipinfo;
		wifi_get_ip_info(SOFTAP_IF, &ipinfo);
		IP4_ADDR(&ipinfo.ip, 192, 168, 4, 1);
		IP4_ADDR(&ipinfo.gw, 192, 168, 4, 1);
		IP4_ADDR(&ipinfo.netmask, 255, 255, 255, 0);
		wifi_set_ip_info(SOFTAP_IF, &ipinfo);
		wifi_softap_dhcps_start();
	}
	#ifdef PLATFORM_DEBUG
	os_printf("ESP8266 in AP mode configured.\r\n");
	#endif
}

void ICACHE_FLASH_ATTR
network_init()
{
	setup_wifi_ap_mode();
    os_timer_disarm(&network_timer);
    os_timer_setfn(&network_timer, (os_timer_func_t *)network_check_ip, NULL);
    os_timer_arm(&network_timer, 10000, 0);
}




