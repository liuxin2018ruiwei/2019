#include <stdio.h>
#include<string.h>
#include<stdlib.h>
//#include "DetectFaceIF.h"
#include "CaseCommonInfo.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "Driver.h"
#include "TestDriverFactory.h"
  
  
  int main( int argc, char *argv[] )
  {
	int startIdx = 0;
	int endIdx = TestDriverFactory::CaseIDMax;
	
	if( 2 == argc )
	{
		startIdx = atoi(argv[1] );
		endIdx = startIdx + 1;
	}

	Driver *pDriver = nullptr;
	for( unsigned int nCaseId = startIdx; nCaseId < endIdx;  nCaseId++ )
	{
		
		pDriver = TestDriverFactory::createDriver( (TestDriverFactory::CaseID)nCaseId );
		if( nullptr == pDriver )
		{
			continue;
		}

		CaseCommonInfo::getInstance()->setCaseID( pDriver->getCaseID() );
		
		printf("--------------complete Creat, will Init-----------------\n");
		pDriver->init();

		printf("--------------complete Init, will Run-----------------\n");
		pDriver->run();

		printf("--------------complete Run, will Uninit-----------------\n");
		
		pDriver->uninit();
		
		pDriver = nullptr;
	}


	return 0;
	
  }


#ifdef __cplusplus
}
#endif
