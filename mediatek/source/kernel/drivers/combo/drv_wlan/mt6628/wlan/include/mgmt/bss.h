




#ifndef _BSS_H
#define _BSS_H



//NOTE(Kevin): change define for george
//#define MAX_LEN_TIM_PARTIAL_BMP     (((MAX_ASSOC_ID + 1) + 7) / 8)   /* Required bits = (MAX_ASSOC_ID + 1) */
#define MAX_LEN_TIM_PARTIAL_BMP                     ((CFG_STA_REC_NUM + 7) / 8)  /* reserve length greater than maximum size of STA_REC */ //obsoleted: Assume we only use AID:1~15

/* CTRL FLAGS for Probe Response */
#define BSS_PROBE_RESP_USE_P2P_DEV_ADDR             BIT(0)
#define BSS_PROBE_RESP_INCLUDE_P2P_IE               BIT(1)




#define bssAssignAssocID(_prStaRec)      ((_prStaRec)->ucIndex + 1)

/*----------------------------------------------------------------------------*/
/* Routines for all Operation Modes                                           */
/*----------------------------------------------------------------------------*/
P_STA_RECORD_T
bssCreateStaRecFromBssDesc (
    IN P_ADAPTER_T                  prAdapter,
    IN ENUM_STA_TYPE_T              eStaType,
    IN ENUM_NETWORK_TYPE_INDEX_T    eNetTypeIndex,
    IN P_BSS_DESC_T                 prBssDesc
    );

VOID
bssComposeNullFrame (
    IN P_ADAPTER_T      prAdapter,
    IN PUINT_8          pucBuffer,
    IN P_STA_RECORD_T   prStaRec
    );

VOID
bssComposeQoSNullFrame (
    IN P_ADAPTER_T      prAdapter,
    IN PUINT_8          pucBuffer,
    IN P_STA_RECORD_T   prStaRec,
    IN UINT_8           ucUP,
    IN BOOLEAN          fgSetEOSP
    );

WLAN_STATUS
bssSendNullFrame (
    IN P_ADAPTER_T          prAdapter,
    IN P_STA_RECORD_T       prStaRec,
    IN PFN_TX_DONE_HANDLER  pfTxDoneHandler
    );

WLAN_STATUS
bssSendQoSNullFrame (
    IN P_ADAPTER_T          prAdapter,
    IN P_STA_RECORD_T       prStaRec,
    IN UINT_8               ucUP,
    IN PFN_TX_DONE_HANDLER  pfTxDoneHandler
    );


/*----------------------------------------------------------------------------*/
/* Routines for both IBSS(AdHoc) and BSS(AP)                                  */
/*----------------------------------------------------------------------------*/
VOID
bssGenerateExtSuppRate_IE (
    IN P_ADAPTER_T      prAdapter,
    IN P_MSDU_INFO_T    prMsduInfo
    );

VOID
bssBuildBeaconProbeRespFrameCommonIEs (
    IN P_MSDU_INFO_T    prMsduInfo,
    IN P_BSS_INFO_T     prBssInfo,
    IN PUINT_8          pucDestAddr
    );

VOID
bssComposeBeaconProbeRespFrameHeaderAndFF (
    IN PUINT_8      pucBuffer,
    IN PUINT_8      pucDestAddr,
    IN PUINT_8      pucOwnMACAddress,
    IN PUINT_8      pucBSSID,
    IN UINT_16      u2BeaconInterval,
    IN UINT_16      u2CapInfo
    );

WLAN_STATUS
bssSendBeaconProbeResponse (
    IN P_ADAPTER_T prAdapter,
    IN ENUM_NETWORK_TYPE_INDEX_T eNetTypeIndex,
    IN PUINT_8 pucDestAddr,
    IN UINT_32 u4ControlFlags
    );

WLAN_STATUS
bssProcessProbeRequest (
    IN P_ADAPTER_T  prAdapter,
    IN P_SW_RFB_T   prSwRfb
    );

VOID
bssClearClientList (
    IN P_ADAPTER_T  prAdapter,
    IN P_BSS_INFO_T prBssInfo
    );

VOID
bssAddStaRecToClientList (
    IN P_ADAPTER_T  prAdapter,
    IN P_BSS_INFO_T prBssInfo,
    IN P_STA_RECORD_T prStaRec
    );

VOID
bssRemoveStaRecFromClientList (
    IN P_ADAPTER_T  prAdapter,
    IN P_BSS_INFO_T prBssInfo,
    IN P_STA_RECORD_T prStaRec
    );


/*----------------------------------------------------------------------------*/
/* Routines for IBSS(AdHoc) only                                              */
/*----------------------------------------------------------------------------*/
VOID
ibssProcessMatchedBeacon (
    IN P_ADAPTER_T  prAdapter,
    IN P_BSS_INFO_T prBssInfo,
    IN P_BSS_DESC_T prBssDesc,
    IN UINT_8       ucRCPI
    );

WLAN_STATUS
ibssCheckCapabilityForAdHocMode (
    IN P_ADAPTER_T  prAdapter,
    IN P_BSS_DESC_T prBssDesc
    );

VOID
ibssInitForAdHoc (
    IN P_ADAPTER_T  prAdapter,
    IN P_BSS_INFO_T prBssInfo
    );

WLAN_STATUS
bssUpdateBeaconContent (
    IN P_ADAPTER_T prAdapter,
    IN ENUM_NETWORK_TYPE_INDEX_T eNetTypeIndex
    );


/*----------------------------------------------------------------------------*/
/* Routines for BSS(AP) only                                                  */
/*----------------------------------------------------------------------------*/
VOID
bssInitForAP (
    IN P_ADAPTER_T  prAdapter,
    IN P_BSS_INFO_T prBssInfo,
    IN BOOLEAN fgIsRateUpdate
    );

VOID
bssUpdateDTIMCount (
    IN P_ADAPTER_T  prAdapter,
    IN ENUM_NETWORK_TYPE_INDEX_T eNetTypeIndex
    );

VOID
bssSetTIMBitmap (
    IN P_ADAPTER_T  prAdapter,
    IN P_BSS_INFO_T prBssInfo,
    IN UINT_16 u2AssocId
    );


/*link function to p2p module for txBcnIETable*/
#if CFG_ENABLE_WIFI_DIRECT
UINT_32
linkToP2pCalculateP2P_IELenForBeacon (
    IN P_ADAPTER_T prAdapter,
    IN ENUM_NETWORK_TYPE_INDEX_T eNetTypeIndex,
    IN P_STA_RECORD_T prStaRec
    );

VOID
linkToP2pGenerateP2P_IEForBeacon (
    IN P_ADAPTER_T prAdapter,
    IN P_MSDU_INFO_T prMsduInfo
    );

UINT_32
linkToP2pCalculateWSC_IELenForBeacon (
    IN P_ADAPTER_T prAdapter,
    IN ENUM_NETWORK_TYPE_INDEX_T eNetTypeIndex,
    IN P_STA_RECORD_T prStaRec
    );

VOID
linkToP2pGenerateP2P_IEForAssocReq (
    IN P_ADAPTER_T prAdapter,
    IN P_MSDU_INFO_T prMsduInfo
    );

VOID
linkToP2pGenerateP2P_IEForAssocRsp (
    IN P_ADAPTER_T prAdapter,
    IN P_MSDU_INFO_T prMsduInfo
    );


UINT_32
linkToP2pCalculateP2P_IELenForAssocReq (
    IN P_ADAPTER_T prAdapter,
    IN ENUM_NETWORK_TYPE_INDEX_T eNetTypeIndex,
    IN P_STA_RECORD_T prStaRec
    );

UINT_32
linkToP2pCalculateP2P_IELenForAssocRsp (
    IN P_ADAPTER_T prAdapter,
    IN ENUM_NETWORK_TYPE_INDEX_T eNetTypeIndex,
    IN P_STA_RECORD_T prStaRec
    );

VOID
linkToP2pGenerateWSC_IEForBeacon(
    IN P_ADAPTER_T        prAdapter,
    IN P_MSDU_INFO_T      prMsduInfo
    );

UINT_32
linkToP2pCalculateWSC_IELenForProbeRsp (
    IN P_ADAPTER_T prAdapter,
    IN ENUM_NETWORK_TYPE_INDEX_T eNetTypeIndex,
    IN P_STA_RECORD_T prStaRec
    );

VOID
linkToP2pGenerateWSC_IEForProbeRsp(
    IN P_ADAPTER_T        prAdapter,
    IN P_MSDU_INFO_T      prMsduInfo
    );
#endif

/* WMM-2.2.2 WMM ACI to AC coding */
typedef enum _ENUM_ACI_T {
    ACI_BE = 0,
    ACI_BK = 1,
    ACI_VI = 2,
    ACI_VO = 3,
    ACI_NUM
} ENUM_ACI_T, *P_ENUM_ACI_T;

typedef enum _ENUM_AC_PRIORITY_T {
    AC_BK_PRIORITY = 0,
    AC_BE_PRIORITY,
    AC_VI_PRIORITY,
    AC_VO_PRIORITY
} ENUM_AC_PRIORITY_T, *P_ENUM_AC_PRIORITY_T;


#endif /* _BSS_H */

