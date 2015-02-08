




#ifndef _WLAN_TYPEDEF_H
#define _WLAN_TYPEDEF_H




typedef struct _BSS_INFO_T      BSS_INFO_T, *P_BSS_INFO_T;

typedef BSS_INFO_T              AIS_BSS_INFO_T, *P_AIS_BSS_INFO_T;
typedef BSS_INFO_T              P2P_BSS_INFO_T, *P_P2P_BSS_INFO_T;
typedef BSS_INFO_T              BOW_BSS_INFO_T, *P_BOW_BSS_INFO_T;

typedef struct _AIS_SPECIFIC_BSS_INFO_T AIS_SPECIFIC_BSS_INFO_T, *P_AIS_SPECIFIC_BSS_INFO_T;
typedef struct _P2P_SPECIFIC_BSS_INFO_T P2P_SPECIFIC_BSS_INFO_T, *P_P2P_SPECIFIC_BSS_INFO_T;
typedef struct _BOW_SPECIFIC_BSS_INFO_T BOW_SPECIFIC_BSS_INFO_T, *P_BOW_SPECIFIC_BSS_INFO_T;


/* BSS related structures */
/* Type definition for BSS_DESC_T structure, to describe parameter sets of a particular BSS */
typedef struct _BSS_DESC_T      BSS_DESC_T, *P_BSS_DESC_T, **PP_BSS_DESC_T;





#endif /* _WLAN_TYPEDEF_H */


