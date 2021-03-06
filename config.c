#include <os.h>
#include "graph.h"
#include "Chinese.h"
#include "menu.h"

#define CONFIG_PATH "/documents/ndless/nNovel.cfg.tns"

typedef struct
{
	int show_time;
	int show_second;
	int show_page_slide;
	int auto_save;
	int auto_save_time;
	int auto_save_exit;
	char Font_path[50];	
} CONFIG;
CONFIG config;

void load_config()
{
	FILE * cfgdata = fopen( CONFIG_PATH , "rb+" );
	
	if( cfgdata == NULL )
	{
		cfgdata = fopen( CONFIG_PATH , "wb+" );
		fseek( cfgdata , 0 , SEEK_SET );
		config.show_time		= 1 ;
		config.show_second		= 1 ; 
		config.show_page_slide	= 1 ;
		config.auto_save		= 1 ;
		config.auto_save_time	= 20 ;
		config.auto_save_exit	= 1 ;
		strcpy( config.Font_path , "/documents/Fonts/HZK16.nft.tns" );
		fwrite( &config , sizeof(int)*6 + sizeof(char)*50 , 1 , cfgdata );
		fclose( cfgdata );
	}
	else
	{
		fseek( cfgdata , 0 , SEEK_SET );
		fread( &config , sizeof(int)*6 + sizeof(char)*50 , 1 , cfgdata );
		fclose( cfgdata );
	}
}

void setup_config()
{
	FILE * cfgdata = fopen( CONFIG_PATH , "rb+" );
	char menu_title[200],temp[50];
	menu_title[0] = 0 ;
	
	strcat( menu_title , "    设置\n更换字体          ");

	strcat( menu_title , "\n时间显示          ");
	if(config.show_time == 1)
		strcat( menu_title , "开" );
	else 
		strcat( menu_title , "关" );
	
	
	strcat( menu_title , "\n秒数显示          ");
	if(config.show_second == 1)
		strcat( menu_title , "开" );
	else 
		strcat( menu_title , "关" );
	
	
	strcat( menu_title , "\n像素翻页效果      ");
	if(config.show_page_slide == 1)
		strcat( menu_title , "开" );
	else 
		strcat( menu_title , "关" );
	
	
	strcat( menu_title , "\n自动保存          ");
	if(config.auto_save == 1)
		strcat( menu_title , "开" );
	else 
		strcat( menu_title , "关" );
	
	
	strcat( menu_title , "\n自动保存选项      ");	
	strcat( menu_title , "\n建立文件关联      ");	
	
	strcat( menu_title , "\n退出时自动保存    ");
	if(config.auto_save_exit == 1)
		strcat( menu_title , "开\n" );
	else 
		strcat( menu_title , "关\n" );	
	
	int i ;
	i = menu( SCREEN_BASE_ADDRESS , 30 , 30 , menu_title , "\n选择一个字体文件\n是否在屏幕左下方显示时间\n显示的时间是否包括秒数\n当效果不佳时可以关闭\n是否开启自动保存\n自动保存的频率\n关联后可直接打开txt文件阅读\n程序退出时是否保存进度\n" );
	switch(i)
	{
		case 1 :
		{
			if(file_browse("/documents/Fonts",".nft.tns",temp))
			{	
				CloseHZK();
				sprintf(config.Font_path,"/documents/Fonts/%s",temp);
				OpenHZK();
			}
			
			break ;
		}
		case 2 : if( config.show_time == 1 )  				config.show_time = 0 ;			else config.show_time = 1 ;			break;
		case 3 : if( config.show_second == 1 )  			config.show_second = 0 ;		else config.show_second = 1 ;		break;
		case 4 : if( config.show_page_slide == 1 )  		config.show_page_slide = 0 ;	else config.show_page_slide = 1 ;	break;
		case 5 : if( config.auto_save == 1 )  				config.auto_save = 0 ;			else config.auto_save = 1 ;			break;
		case 6 :     config.auto_save_time = num_input( SCREEN_BASE_ADDRESS , 50 , 50 , "当页码被整除时" );			break;
		case 7 : 
		{
			FILE *fp = NULL;
			fp = fopen( "/documents/ndless/ndless.cfg.tns" , "rb+" );
			if( fp == NULL )
			{
				wait_no_key_pressed();
				if(show_msgbox_2b("鎻愮ず","鏈壘鍒皀dless.cfg.tns\n鏄惁瑕佸缓绔嬭鏂囦欢锛�","鏄�","涓嶏紝璋㈣阿")==1)
				{
					fp = fopen( "/documents/ndless/ndless.cfg.tns" , "wb+" );
				}
				else
					break;
			}
			int cfgsize = 0;
			fseek( fp , 0 , SEEK_END );
			cfgsize = ftell(fp);
			char *pncfg = NULL;
			pncfg = malloc( cfgsize + 50 );
			if(pncfg==NULL)
			{
				wait_no_key_pressed();
				show_msgbox("閿欒","鍐呭瓨涓ラ噸涓嶈冻锛屾棤娉曠户缁紒");
				fclose( fp );
				break;
			}
			fseek(fp,0,SEEK_SET);
			fread(pncfg,cfgsize,1,fp);
			if( strstr(pncfg,"ext.txt=nNovel") )
			{
				wait_no_key_pressed();
				show_msgbox("鎻愮ず","鏂囦欢鍏宠仈宸插缓绔嬶紝鏃犻渶閲嶅鎿嶄綔銆�");
				fclose( fp );
				free(pncfg);
				break;
			}
			else
			{
				strcat(pncfg,"\r\next.txt=nNovel");
				fseek(fp,0,SEEK_SET);
				fwrite(pncfg,strlen(pncfg),1,fp);
				wait_no_key_pressed();
				show_msgbox("鎻愮ず","鏂囦欢鍏宠仈宸插缓绔�!");
				fclose( fp );
				free(pncfg);
				break;
			}
		}
		case 8 : if( config.auto_save_exit == 1 )  			config.auto_save_exit = 0 ;		else config.auto_save_exit = 1 ;	break;
		default : break ;
	}
	fseek( cfgdata , 0 , SEEK_SET );
	fwrite( &config , sizeof(int)*6 + sizeof(char)*50 , 1 , cfgdata );
	fclose( cfgdata );
}


