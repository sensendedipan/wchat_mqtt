
#ifndef _DEBUG_H
#define _DEBUG_H


#define DEBUG

#ifdef  DEBUG   
	#define debug(fmt, args...)		        printf("\napple: "fmt, ##args)
    
	#define debugPrintf(fmt,args...)   	    printf(fmt"----[file name = %s  line num = %d]",__FILE__,__LINE__,##args)		
	#define debugLogInfo(fmt, args...)		printf("INFO: "fmt, ##args)
	#define debugLogWarn(fmt, args...)		printf("WARN: "fmt, ##args)
	#define debugLogError(fmt, args...)		printf("EROR: "fmt, ##args)	
	
#else
	#define debug(fmt, args...)		 
    
    #define debugPrintf(fmt,args...)
	#define debugLogInfo(fmt, args...)		
	#define debugLogWarn(fmt, args...)		
	#define debugLogError(fmt, args...)		
#endif



#endif

