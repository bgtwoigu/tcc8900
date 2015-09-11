/*!
 ***********************************************************************
 \par Copyright
 \verbatim
  ________  _____           _____   _____           ____  ____   ____		
     /     /       /       /       /       /     /   /    /   \ /			
    /     /___    /       /___    /       /____ /   /    /____/ \___			
   /     /       /       /       /       /     /   /    /           \		
  /     /_____  /_____  /_____  /_____  /     / _ /_  _/_      _____/ 		
   																				
  Copyright (c) 2009 Telechips Inc.
  Korad Bldg, 1000-12 Daechi-dong, Kangnam-Ku, Seoul, Korea					
 \endverbatim
 ***********************************************************************
 */
/*!
 ***********************************************************************
 *
 * \file
 *		TCC_VPU_CODEC.h
 * \date
 *		2009/10/30
 * \author
 *		AValgorithm(AValgorithm@telechips.com) 
 * \brief
 *		main api
 * \version
 *      0.9.6 (2009/12/21)
 *      0.9.5 (2009/12/21)
 *      0.9.4 (2009/12/10)
 *      0.9.3 (2009/12/10)
 *      0.8.7 (2009/11/10)
 *      0.8.6 (2009/10/30)
 *      0.8.5 (2009/08/26)
 *		0.8.4 (2009/08/14)
 *		0.8.3 (2009/08/04)
 *		0.8.2 (2009/07/13)
 *		0.5.0 : first beta release (2009/03/23)
 *
 ***********************************************************************
 */
#ifndef _TCC_VPU_CODEC_H_
#define _TCC_VPU_CODEC_H_

//#define USE_VPU_DISPLAY_MODE	//! use ring buffer

#define MAX_NUM_INSTANCE		4

#define PA 0 // physical address
#define VA 1 // virtual  address

#define PIC_TYPE_I		0x000
#define PIC_TYPE_P		0x001
#define PIC_TYPE_B		0x002
#define PIC_TYPE_B_PB	0x102 //! only for MPEG-4 Packed PB-frame

#define STD_AVC		0 //!< DEC / ENC
#define STD_VC1		1 //!< DEC
#define STD_MPEG2	2 //!< DEC
#define STD_MPEG4	3 //!< DEC / ENC
#define STD_H263	4 //!< DEC / ENC
#define STD_DIV3	5 //!< DEC
#define STD_RV		6 //!< DEC
#define STD_WMV78	8
#define	STD_SORENSON263	9


#define RETCODE_SUCCESS						0
#define RETCODE_FAILURE						1
#define RETCODE_INVALID_HANDLE				2
#define RETCODE_INVALID_PARAM				3
#define RETCODE_INVALID_COMMAND				4
#define RETCODE_ROTATOR_OUTPUT_NOT_SET		5
#define RETCODE_ROTATOR_STRIDE_NOT_SET		6
#define RETCODE_FRAME_NOT_COMPLETE			7
#define RETCODE_INVALID_FRAME_BUFFER		8
#define RETCODE_INSUFFICIENT_FRAME_BUFFERS	9
#define RETCODE_INVALID_STRIDE				10
#define RETCODE_WRONG_CALL_SEQUENCE			11
#define RETCODE_CALLED_BEFORE				12
#define RETCODE_NOT_INITIALIZED				13
#define RETCODE_USERDATA_BUF_NOT_SET		14
#define RETCODE_CODEC_FINISH				15		//the end of decoding
#define RETCODE_CODEC_EXIT					16
#define RETCODE_CODEC_SPECOUT				17
#define RETCODE_CODEC_MEM_ERROR			18

#define RETCODE_ERR_STRIDE_ZERO_OR_ALIGN8	100
#define RETCODE_ERR_MIN_RESOLUTION			101
#define RETCODE_ERR_MAX_RESOLUTION			102
#define RETCODE_H264ERR_PROFILE				110
#define RETCODE_VC1ERR_COMPLEX_PROFILE		120
#define RETCODE_H263ERR_ANNEX_D				130
#define RETCODE_H263ERR_ANNEX_EFG			131
#define RETCODE_H263ERR_UFEP				132
#define RETCODE_H263ERR_ANNEX_D_PLUSPTYPE	133
#define RETCODE_H263ERR_ANNEX_EF_PLUSPTYPE	134
#define RETCODE_H263ERR_ANNEX_NRS_PLUSPTYPE	135
#define RETCODE_H263ERR_ANNEX_PQ_MPPTYPE	136
#define RETCODE_H263ERR_UUI					137	//Unlimited Unrestricted Motion Vectors Indicator(UUI)
#define RETCODE_H263ERR_SSS					138	//Slice Structure Submode(Rectangular Slices or Arbitaray Slice Ordering)
#define RETCODE_MPEG4ERR_OBMC_DISABLE		140
#define RETCODE_MPEG4ERR_SPRITE_ENABLE		141	//Sprite Coding Mode(include GMC)
#define RETCODE_MPEG4ERR_MP4ERR_SCALABILITY	142
#define RETCODE_MPEG2ERR_CHROMA_FORMAT		150
#define RETCODE_RVERR_SUB_MOF_FLAG31TO16	160 //RV
#define RETCODE_RVERR_SUB_MOF_FLAG16TO0		161 //RV8, RV9, RV89combo

#define RETCODE_WRAP_AROUND					-10

#define CODE_BUF_SIZE			( 136 * 1024 ) // 136K
#define FMO_SLICE_SAVE_BUF_SIZE	( 32 )
#define WORK_BUF_SIZE			( 512 * 1024 ) + ( FMO_SLICE_SAVE_BUF_SIZE * 1024 * 8 ) // 768K
#define PARA_BUF2_SIZE			( 2 * 1024 ) // 2K
#define PARA_BUF_SIZE			( 10 * 1024 ) // 10K
#define WORK_CODE_PARA_BUF_SIZE ( CODE_BUF_SIZE + WORK_BUF_SIZE + PARA_BUF2_SIZE + PARA_BUF_SIZE )


#ifdef RESOLUTION_LIMIT_D1_INCLUDE
#define	LARGE_STREAM_BUF_SIZE	(2*512*1024)
#else
#define	LARGE_STREAM_BUF_SIZE	(8*1024*1024)
#endif

#define	SLICE_SAVE_SIZE			0x040000
#define PS_SAVE_SIZE			0x080000

#define ALIGNED_BUFF(buf, mul) ( ( (unsigned int)buf + (mul-1) ) & ~(mul-1) ) // mul�� ���

typedef int codec_handle_t; 		//!< handle
typedef int codec_result_t; 		//!< return value
typedef unsigned int codec_addr_t;	//!< address

//------------------------------------------------------------------------------
// decode struct and definition
//------------------------------------------------------------------------------
//! represents rectangular window information in a frame 
typedef struct pic_crop_t
{
    unsigned int m_iCropLeft;
    unsigned int m_iCropTop;
    unsigned int m_iCropRight;
    unsigned int m_iCropBottom;
} pic_crop_t;

//-----------------------------------------------------
// data structure to get information necessary to 
// start decoding from the decoder (this is an output parameter)
//-----------------------------------------------------
typedef struct dec_initial_info_t     
{
	int m_iPicWidth;				//!< {(PicX+15)/16} * 16  (this width  will be used while allocating decoder frame buffers. picWidth  is a multiple of 16)
	int m_iPicHeight;				//!< {(PicY+15)/16} * 16  (this height will be used while allocating decoder frame buffers. picHeight is a multiple of 16)
	unsigned int m_uiFrameRateRes;	//!< decoded picture frame rate residual(number of time units of a clock operating at the frequency[m_iFrameRateDiv] Hz, frameRateInfo = m_uiFrameRateRes/m_uiFrameRateDiv
	unsigned int m_uiFrameRateDiv;	//!< decoded picture frame rate unit number in Hz				
	int m_iMinFrameBufferCount;		//!< the minimum number of frame buffers that are required for decoding. application must allocate at least this number of frame buffers.
	int m_iMinFrameBufferSize;		//!< minimum frame buffer size
	int	m_iNormalSliceSize;			//!< recommended size of to save slice. this value is determined as a quater of the memory size for one raw YUV image in KB unit.
	int	m_iWorstSliceSize;			//!< recommended size of to save slice in worst case. this value is determined as a half of the memory size for one raw YUV image in KB unit.

	// H264/AVC only param
	pic_crop_t m_iAvcPicCrop;		//!< represents rectangular window information in a frame 
	int m_iAvcConstraintSetFlag[4];	//!< syntax element which is used to make level in H.264. used only in H.264 case. 
	int m_iAvcParamerSetFlag;		//!< These are H.264 SPS or PPS syntax element
									//!< [bit 0  ] direct_8x8_inference_flag in H.264 SPS
	int m_iFrameBufDelay;			//!< maximum display frame buffer delay to process reordering in case of H.264

	// MPEG-4 only param
	int m_iM4vDataPartitionEnable;	//!< ( 0: disable   1: enable )
	int m_iM4vReversibleVlcEnable;	//!< ( 0: disable   1: enable )
	int m_iM4vShortVideoHeader;		//!< ( 0: disable   1: enable )
	int m_iM4vH263AnnexJEnable;		//!< ( 0: disable   1: enable )

	//! VC-1 only param
	int m_iVc1Psf;					//!< this is only available in VC1 and indicates the value of "Progressive Segmented Frame" 

	//! Additional Info
	int m_iProfile;					//!< profile of the decoded stream
	int m_iLevel;					//!< level of the decoded stream
	int m_iInterlace;				//!< when this value is 1, decoded stream will be decoded into both progressive or interlace frame. otherwise, all the frames will be progressive. In H.264, this is frame_mbs_only_flag.
	int m_iAspectRateInfo;			//!< aspect rate information. if this value is 0, then aspect ratio information is not present 
	int m_iReportErrorReason;		//!< reports the reason of 'RETCODE_CODEC_SPECOUT' or 'RETCODE_INVALID_STRIDE' error

} dec_initial_info_t;

//! data structure for initializing Video unit
typedef struct dec_init_t 
{
	codec_addr_t m_BitWorkAddr[2];		//!< physical[0] and virtual[1] address of a working space of the decoder. This working buffer space consists of work buffer, code buffer, and parameter buffer.
	codec_addr_t m_RegBaseVirtualAddr;	//!< virtual address BIT_BASE

	// Bitstream Info
	int m_iBitstreamFormat;				//!< bitstream format
	codec_addr_t m_BitstreamBufAddr[2];	//!< bitstream buf address : multiple of 4
	int m_iBitstreamBufSize;			//!< bitstream buf size	   : multiple of 1024
	int m_iPicWidth;					//!< frame width from demuxer or etc
	int m_iPicHeight;					//!< frame height from demuxer or etc

	// Decoding Options
#define M4V_DEBLK_DISABLE	0		// (default)
#define M4V_DEBLK_ENABLE	1		// mpeg-4 deblocking
#define M4V_GMC_FILE_SKIP	(0<<1)	// (default) seq.init failure
#define M4V_GMC_FRAME_SKIP	(1<<1)	// frame skip without decoding
	unsigned int m_uiDecOptFlags;

	// H264 only param
	unsigned char* m_pSpsPpsSaveBuffer;	//!< multiple of 4
	int m_iSpsPpsSaveBufferSize;		//!< multiple of 1024

	// VPU Control 
	unsigned int m_bEnableUserData;		//!< If this is set, VPU returns userdata.
	unsigned int m_bEnableVideoCache;	//!< video cache 
	unsigned int m_bCbCrInterleaveMode;	//!< 0 (chroma separate mode   : CbCr data is written in separate frame memories)
										//!< 1 (chroma interleave mode : CbCr data is interleaved in chroma memory)
	int m_iFilePlayEnable;				//!< enable file play mode. If this value is set to 0, streaming mode with ring buffer will be used

#define RESOLUTION_1080_HD	0			//1920x1088
#define RESOLUTION_720P_HD	1			//1280x720
#define RESOLUTION_625_SD	2			//720x576
	int m_iMaxResolution;				//!< maximum resolution limitation option

	//! Callback Func
	void* (*m_Memcpy ) ( void*, const void*, unsigned int );	//!< memcpy
	void  (*m_Memset ) ( void*, int, unsigned int );			//!< memset
	int   (*m_Interrupt ) ( void );								//!< hw interrupt (return value is always 0)
} dec_init_t;

typedef struct dec_input_t 
{
	codec_addr_t m_BitstreamDataAddr[2];//!< bitstream data address
	int m_iBitstreamDataSize;			//!< bitstream data size
	codec_addr_t m_UserDataAddr[2];//!< Picture Layer User-data address
	int m_iUserDataBufferSize;	//!< Picture Layer User-data Size

	int m_iFrameSearchEnable;			//!< I-frame Search Mode
										//!< If this option is enabled, then decoder performs skipping frame decoding until decoder meet IDR(and/or I)-picture for H.264 or I-frame.
										//!< If this option is enabled, m_iSkipFrameMode option is ignored.
										//!< 0x000 ( Disable )
										//!< 0x001 ( Enable : search IDR-picture for H.264 or I-frame ) 
										//!< 0x201 ( Enable : search I(or IDR)-picture for H.264 or I-frame ) 

	int m_iSkipFrameMode;				//!< Skip Frame Mode
										//!< 0 ( Skip disable )
										//!< 1 ( Skip except I(IDR) picture )
										//!< 2 ( Skip B picture : skip if nal_ref_idc==0 in H.264 )
										//!< 3 ( Unconditionally Skip one picture )

	int m_iSkipFrameNum;				//!< Number of skip frames (for I-frame Search Mode or Skip Frame Mode)
										//!< When this number is 0, m_iSkipFrameMode option is disabled.

	int	m_iErrorMBsLimitaton;			//!< Threshold of the number of error MBs (only for H.264)
										//!< If the number of error MBs is greater than the threshold, current decoded frame is discarded.
} dec_input_t;

typedef struct dec_buffer_t
{
	codec_addr_t m_FrameBufferStartAddr[2];	//!< physical[0] and virtual[1] address of a frame buffer of the decoder.
	int m_iFrameBufferCount;				//!< allocated frame buffer count
	codec_addr_t m_AvcSliceSaveBufferAddr;  //!< start address and size of slice save buffer which the decoder can save slice RBSP : multiple of 4
	int m_iAvcSliceSaveBufferSize;			//!< multiple of 1024
} dec_buffer_t;


typedef struct dec_buffer2_t
{
	codec_addr_t	m_addrFrameBuffer[2][32];		//!< physical[0] and virtual[1] address of a frame buffer of the decoder.
	unsigned long	m_ulFrameBufferCount;			//!< allocated frame buffer count
	codec_addr_t	m_addrAvcSliceSaveBuffer;		//!< start address and size of slice save buffer which the decoder can save slice RBSP : multiple of 4
	unsigned long	m_ulAvcSliceSaveBufferSize;		//!< multiple of 1024
} dec_buffer2_t;


typedef struct dec_ring_buffer_setting_in_t
{
	codec_addr_t m_OnePacketBufferAddr;
	unsigned int m_iOnePacketBufferSize;
} dec_ring_buffer_setting_in_t;

typedef struct dec_ring_buffer_status_out_t
{
	unsigned int m_iAvailableSpaceInRingBuffer;
} dec_ring_buffer_status_out_t;



//-----------------------------------------------------
// data structure to get resulting information from 
// VPU after decoding a frame
//-----------------------------------------------------
// for m_iOutputStatus
#define VPU_DEC_OUTPUT_FAIL			0
#define VPU_DEC_OUTPUT_SUCCESS		1

// for m_iDecodingStatus
#define VPU_DEC_SUCCESS								1
#define VPU_DEC_INFO_NOT_SUFFICIENT_SPS_PPS_BUFF	2
#define VPU_DEC_INFO_NOT_SUFFICIENT_SLICE_BUFF		3
#define VPU_DEC_BUF_FULL							4
#define VPU_DEC_SUCCESS_FIELD_PICTURE				5	//added 20091110

typedef struct dec_output_info_t
{
	int m_iPicType;					//!< ( 0- I picture,  1- P picture,  2- B picture )

	int m_iDispOutIdx;				//!< index of output frame buffer
	int m_iDecodedIdx;				//!< index of decoded frame buffer

	int m_iOutputStatus;
	int m_iDecodingStatus;

	int m_iInterlacedFrame;			//!< Interlaced Frame 
	int m_iNumOfErrMBs;				//!< number of error macroblocks

	// VC-1 Only
	int m_iVc1HScaleFlag;			//!< Flag for reduced resolution output in horizontal direction (in case of VC1 )
	int m_iVc1VScaleFlag;		    //!< Flag for reduced resolution output in vertical direction (in case of VC1 )

	// MPEG-2 Only
	int m_iM2vProgressiveFrame;		//!< indicate progressive frame in picture coding extention in MPEG2
	int m_iM2vProgressiveSequence;
	int m_iM2vFieldSequence;		//!< indicate field sequence in picture coding extention in MPEG2  
	int m_iM2vAspectRatio;
	int m_iM2vFrameRate;

	// More Info
	int m_iPictureStructure;		//!< indicates that the decodec picture is progressive or interlaced picture 
	int m_iTopFieldFirst;			//!< if this value is 1, top field is first decoded and then bottom field is decoded.  
	int m_iRepeatFirstField;		//!< repeat first field. this value is used during display process. 

	// RV Only
	int m_iRvTimestamp;				//!< RV TR syntax

	int m_iHeight;					//!< Height of input bitstream. In some cases, this value can be different from the height of previous frames.
	int m_iWidth;					//!< Width of input bitstream. In some cases, this value can be different from the height of previous frames.
	pic_crop_t m_CropInfo;			//!< Cropping information.

	// User Data Buffer Address
	codec_addr_t m_UserDataAddress[2];	//!< If contents have picture-layer user-data, return it.

	int m_iConsumedBytes;			//!< consumed bytes (only for file play mode, m_iFilePlayEnable=1)

	int m_iInvalidDispCount;		//!< counter of consecutive display index error
	int m_iPicSideInfo;				//!< Picture side information (only for H.264)
									//!< bit[0]: DPB(Decode Picture Buffer) syntax error notification
									//		  1: Dpb syntax elements had error during current frame decoding process
									//!< bit[1]: weighted prediction notification
									//		  1: Implicit Weighted Prediction was used during current frame decoding process in H.264

	unsigned int m_Reserved[13];	//! Reserved. 

} dec_output_info_t;

#define COMP_Y 0
#define COMP_U 1
#define COMP_V 2

typedef struct dec_output_t 
{
	dec_output_info_t m_DecOutInfo;
	unsigned char* m_pDispOut[2][3]; //! physical[0] and virtual[1] display  address of Y, Cb, Cr component
	unsigned char* m_pCurrOut[2][3]; //! physical[0] and virtual[1] current  address of Y, Cb, Cr component
	unsigned char* m_pPrevOut[2][3]; //! physical[0] and virtual[1] previous address of Y, Cb, Cr component
} dec_output_t;

// Decoder Op Code 
#define VPU_DEC_INIT				0x00	//!< init
#define VPU_DEC_SEQ_HEADER			0x01	//!< decode sequence header 
#define VPU_DEC_GET_INFO			0x02
#define VPU_DEC_REG_FRAME_BUFFER	0x03	//!< register frame buffer
#define VPU_DEC_REG_FRAME_BUFFER2	0x04	//!< register frame buffer
#define VPU_DEC_DECODE				0x10	//!< decode
#define VPU_DEC_BUF_FLAG_CLEAR		0x11	//!< display buffer flag clear
#define VPU_DEC_FLUSH_OUTPUT		0x12	//!< flush delayed output frame
#define GET_RING_BUFFER_STATUS		0x13
#define FILL_RING_BUFFER_AUTO		0x14    //!< Fill the ring buffer
#define GET_INITIAL_INFO_FOR_STREAMING_MODE_ONLY			0x15    //!< Get initial Info for ring buffer use
#define VPU_DEC_CLOSE				0x20	//!< close

/*!
 ***********************************************************************
 * \brief
 *		TCC_VPU_DEC		: main api function of libvpudec
 * \param
 *		[in]Op			: decoder operation 
 * \param
 *		[in,out]pHandle	: libvpudec's handle
 * \param
 *		[in]pParam1		: init or input parameter
 * \param
 *		[in]pParam2		: output or info parameter
 * \return
 *		If successful, TCC_VPU_DEC returns 0 or plus. Otherwise, it returns a minus value.
 ***********************************************************************
 */
codec_result_t
TCC_VPU_DEC( int Op, codec_handle_t* pHandle, void* pParam1, void* pParam2 );


//------------------------------------------------------------------------------
// encode struct and definition : from "VpuApi.h"
//------------------------------------------------------------------------------
typedef struct enc_init_t 
{
	codec_addr_t m_BitWorkAddr[2];		//!< physical[0] and virtual[1] address of a working space of the decoder. This working buffer space consists of work buffer, code buffer, and parameter buffer.
	codec_addr_t m_RegBaseVirtualAddr;	//!< virtual address BIT_BASE

	// Encoding Info
	int m_iBitstreamFormat;
	int m_iPicWidth;					//!< multiple of 16
	int m_iPicHeight;					//!< multiple of 16
	int m_iFrameRate;
	int m_iTargetKbps;					//!< Target bit rate in Kbps. if 0, there will be no rate control, 
										//!< and pictures will be encoded with a quantization parameter equal to quantParam
	int m_iKeyInterval;					//!< max 32767
	int m_iIFrameQp;	
	int m_iAvcFastEncoding;				//!< fast encoding for AVC( 0: default, 1: encode intra 16x16 only )

	// Options
	int m_iSliceMode;
	int m_iSliceSizeMode;
	int m_iSliceSize;
	int m_iIntraMBNumInPFrame;

	// VPU Control 
	unsigned int m_bEnableVideoCache;
	unsigned int m_bCbCrInterleaveMode;	//!< 0 (chroma separate mode   : CbCr data is written in separate frame memories)
										//!< 1 (chroma interleave mode : CbCr data is interleaved in chroma memory)

	codec_addr_t m_BitstreamBufferAddr; //!< physical address : multiple of 4
	codec_addr_t m_BitstreamBufferAddr_VA; //!< virtual address : multiple of 4
	int m_iBitstreamBufferSize;			//!< multiple of 1024
	
	codec_addr_t m_MeSearchRamAddr;		//!< physical address : multiple of 4
	int m_iMeSearchRamSize;				//!< ( ( m_iPicWidth + 15 ) & ~15 ) * 36 + 2048; multiple of 16

	// Callback Func
	void* (*m_Memcpy ) ( void*, const void*, unsigned int );	//!< memcpy
	void  (*m_Memset ) ( void*, int, unsigned int );			//!< memset
	int   (*m_Interrupt ) ( void );								//!< hw interrupt (return value is always 0)
} enc_init_t;

typedef struct enc_initial_info_t 
{
	int m_iMinFrameBufferCount;
	int m_iMinFrameBufferSize;		//!< minimum frame buffer size
} enc_initial_info_t;

typedef struct enc_input_t 
{
	codec_addr_t m_PicYAddr;
	codec_addr_t m_PicCbAddr;
	codec_addr_t m_PicCrAddr;
	
	int m_iForceIPicture;
	int m_iSkipPicture;
	int m_iQuantParam;

	codec_addr_t m_BitstreamBufferAddr; //!< physical address
	int m_iBitstreamBufferSize;
} enc_input_t;

typedef struct enc_buffer_t
{
	codec_addr_t m_FrameBufferStartAddr[2];
} enc_buffer_t;

typedef struct enc_output_t 
{
	codec_addr_t m_BitstreamOut[2];
	int m_iBitstreamOutSize;
	int m_iPicType;
	int m_Reserved;
} enc_output_t;

#define MPEG4_VOL_HEADER 0x00
#define MPEG4_VOS_HEADER 0x01
#define MPEG4_VIS_HEADER 0x02
#define AVC_SPS_RBSP 0x10
#define AVC_PPS_RBSP 0x11

typedef struct enc_header_t
{
	int m_iHeaderType;			//!< [in]
	codec_addr_t m_HeaderAddr;	//!< [out]
	int m_iHeaderSize;			//!< [out]
} enc_header_t;


// Encoder Op Code
#define VPU_ENC_INIT				0x00	//!< init
#define VPU_ENC_REG_FRAME_BUFFER	0x01	//!< register frame buffer
#define VPU_ENC_PUT_HEADER			0x10
#define VPU_ENC_ENCODE				0x12	//!< encode
#define VPU_ENC_CLOSE				0x20	//!< close

codec_result_t
TCC_VPU_ENC( int Op, codec_handle_t* pHandle, void* pParam1, void* pParam2 );


#endif//_TCC_VPU_CODEC_H_

