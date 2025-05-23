#ifndef _UTOPIA_DAPI_H_
#define _UTOPIA_DAPI_H_

#include "MsTypes.h"
/* driver owners need not include this explicitly */
#include "utopia_driver_id.h" 
#include "MsCommon.h"

/*
 * for develop: block on error
 * for ut: don't block on error
 */
#define BLOCK_ON_ERROR 1
#if BLOCK_ON_ERROR
#define RET_OR_BLOCK(ret) MS_PANIC(0); return ret; /* fix compiling warning */
#else
#define RET_OR_BLOCK(ret) return ret
#endif

/* print method */
#if defined(ANDROID)
#include <cutils/log.h>
#define printu ALOGI
#elif defined(MSOS_TYPE_LINUX_KERNEL)
#define printu printk
#else
#define printu printf
#endif

/*
 * type converter
 */
#define TO_MODULE_PTR(pTmp) ((UTOPIA_MODULE*)pTmp)
#define TO_MODULE_SHM_PTR(pTmp) ((UTOPIA_MODULE_SHM*)pTmp)
// FIXME: this macro doesn't consider synchronization problem, just for tmp use
#define MODULE_PTR_TO_RPOOL_PTR(pModule) \
    ((UTOPIA_RESOURCE_POOL*)shmid2va(pModule->psModuleShm->shmid_rpool_head.ID))

#define TO_RESOURCE_PTR(pTmp) ((UTOPIA_RESOURCE*)pTmp)
// FIXME: this macro doesn't consider synchronization problem, just for tmp use
#define RESOURCE_PTR_TO_RPOOL_PTR(pResource) \
    ((UTOPIA_RESOURCE_POOL*)shmid2va(pResource->shmid_rpool.ID))

#define TO_INSTANCE_PTR(pTmp) ((UTOPIA_INSTANCE*)pTmp)
#define TO_RPOOL_PTR(pTmp) ((UTOPIA_RESOURCE_POOL*)pTmp)


typedef enum{
    CMD_USIGNAL_PROCESS_KILL = 0xFFFF0001,
}UTOPIA_SYSYEM_IOCTL;


/*
 * instance functions
 */
// MS_U32 UtopiaInstanceCreate(MS_U32 u32PrivateSize, void** ppInstance);
// MS_U32 UtopiaInstanceDelete(void* pInstant);
// MS_U32 UtopiaInstanceGetPrivate(void* pInstance, void** ppPrivate);
// MS_U32 UtopiaInstanceGetModule(void* pInstance, void** ppModule);
/* We hope, we can support poly, ex: JPD and JPD3D as different Module */
// MS_U32 UtopiaInstanceGetModuleID(void* pInstance, MS_U32* pu32ModuleID); 
/* We hope, we can support poly, ex: JPD and JPD3D as different Module */
// MS_U32 UtopiaInstanceGetModuleVersion(void* pInstant, MS_U32* pu32Version); 
/* We hope we can support interface version mantain */
// MS_U32 UtopiaInstanceGetAppRequiredModuleVersion(void* pInstance, 
//         MS_U32* pu32ModuleVersion);
MS_U32 UtopiaInstanceGetPid(void* pInstance);

/*
 * module functions
 */
MS_U32 UtopiaModuleCreate(MS_U32 u32ModuleID, 
        MS_U32 u32PrivateSize, void** ppModule);
// MS_U32 UtopiaModuleGetPrivate(void* pModule, void** ppPrivate);
MS_U32 UtopiaModuleSetSTRPrivate(void* pModule, MS_U32 u32STRPrivateSize);
MS_U32 UtopiaModuleGetSTRPrivate(void* pModule, void** ppPrivate);
MS_U32 UtopiaModuleRegister(void* pModule);
MS_U32 UtopiaModuleSetupFunctionPtr(void* pModule, FUtopiaOpen fpOpen, 
        FUtopiaClose fpClose, FUtopiaIOctl fpIoctl);
MS_U32 UtopiaModuleSetupSTRFunctionPtr(void* pModuleTmp, FUtopiaSTR fpSTR);
MS_U32 UtopiaModuleSuspend(void);
MS_U32 UtopiaModuleResume(void);
// MS_U32 UtopiaModuleSetVersion(void* pModule, MS_U32 u32Version);
// MS_U32 UtopiaModuleGetDebugLevel(void* pInstance, MS_U32* pu32DebugLevel);
// MS_U32 UtopiaModuleGetPtr(MS_U32 u32ModuleID, void** ppModule);

MS_U32 UtopiaModuleQueryMode(MS_U32 u32ModuleID);


/*
 * resource functions
 */
MS_U32 UtopiaResourceCreate(char* u8ResourceName, 
        MS_U32 u32PrivateSize, void** ppResource);
// MS_U32 UtopiaResourceGetPrivate(void* pResource, void** ppPrivate);
MS_U32 UtopiaResourceRegister(void* pModule, void* pResouce, MS_U32 u32PoolID);
// MS_U32 UtopiaResourceObtain(void* pInstant, 
//         MS_U32 u32PoolID, void** ppResource);
// MS_U32 UtopiaResourceTryObtain(void* pInstant, 
//         MS_U32 u32PoolID, void** ppResource);
// MS_U32 UtopiaResourceRelease(void* pResource);
MS_U32 UtopiaResourceGetPid(void* pResource);
MS_U32 UtopiaResourceGetNext(void* pModTmp, void** ppResource);

// MS_U32 UtopiaModuleAddResourceStart(void* psModule, MS_U32 u32PoolID);
// MS_U32 UtopiaModuleAddResourceEnd(void* psModule, MS_U32 u32PoolID);
// MS_U32 UtopiaModuleResetPool(void* pModuleTmp, MS_U32 u32RPoolID);

MS_U32 UtopiaStrWaitCondition(const char* u8ModuleName, MS_U32 u32Mode, MS_U32 u32Stage);
MS_U32 UtopiaStrSendCondition(const char* u8ModuleName, MS_U32 u32Mode, MS_U32 u32Stage);

#endif /* _UTOPIA_DAPI_H_ */
