#include "../include/ConfigManager.h"
#include <stdio.h>

int main()
{
	ConfigManager conf_w, conf_r;
	char szConf[] = "test.conf";
	
	printf("==========================\n");
	printf("ConfigManager Module Check\n");
	printf("==========================\n");
	printf("Function : conf_w.ChkConf\n");
	printf("Config File : %s\n",szConf);
	printf("Result : %d\n",conf_w.ChkConf(szConf));
	printf("Press Enter Key.\n");
	getchar();
	printf("=====================\n");
	printf("Function : conf_w.MakeConf\n");
	printf("Config File : %s\n",szConf);
	printf("Result : %d\n",conf_w.MakeConf(szConf));
	printf("conf.ChkConf Result : %d\n",conf_w.ChkConf(szConf));
	printf("Press Enter Key.\n");
	getchar();
	printf("=====================\n");
	printf("Function : conf_w.InsertTitle\n");
	printf("InsertTitle(test) Result : %d\n",conf_w.InsertTitle("test"));
	printf("Press Enter Key.\n");
	getchar();
	printf("=====================\n");
	printf("Function : conf_w.InsertKey\n");
	printf("InsertKey(a,aa) Result : %d\n",conf_w.InsertKey("a","aa"));
	printf("InsertLine Result : %d\n",conf_w.InsertLine());
	printf("InsertKey(b,11) Result : %d\n",conf_w.InsertKey("b",11));
	printf("InsertKey(d,bb) Result : %d\n",conf_w.InsertKey("d","bb"));
	printf("Press Enter Key.\n");
	getchar();
	printf("=====================\n");
	printf("Function : conf_w.SearchKey\n");
	printf("SearchKey(a) Result : %s\n",conf_w.SearchKey("a"));
	printf("SearchNumKey(b) Result : %d\n",conf_w.SearchNumKey("b"));
	printf("SearchLongKey(b) Result : %ld\n",conf_w.SearchLongKey("b"));
	printf("SearchKey(c) Result : %s\n",conf_w.SearchKey("c"));
	printf("Press Enter Key.\n");
	getchar();
	printf("=====================\n");
	printf("Function : conf.WriteConf\n");
	printf("Result : %d\n",conf_w.WriteConf(szConf));
	printf("Press Enter Key.\n");
	getchar();
	printf("=====================\n");
	printf("Function : conf_r.ReadConf\n");
	printf("Config File : %s\n",szConf);
	printf("Result : %d\n",conf_r.ReadConf(szConf));
	printf("Press Enter Key.\n");
	getchar();
	printf("=====================\n");
	printf("Function : conf_r.SearchKey\n");
	printf("SearchKey(a) Result : %s\n",conf_r.SearchKey("a"));
	printf("SearchNumKey(b) Result : %d\n",conf_r.SearchNumKey("b"));
	printf("SearchLongKey(b) Result : %ld\n",conf_r.SearchLongKey("b"));
	printf("SearchKey(c) Result : %s\n",conf_r.SearchKey("c"));
	printf("Press Enter Key.\n");
	getchar();
	printf("=====================\n");
	printf("Close Config");
	conf_r.CloseConf();
	conf_w.CloseConf();
	getchar();
	printf("=====================\n");
	printf("Erase Config File : %s\n",szConf);
	printf("Result : %d\n",remove(szConf));
	printf("=====================\n");
	printf("End.\n\n");
	return 0;
}