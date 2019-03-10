#pragma once

/**
* @Brief		错误类型定义.	
*
*/


///对比压缩错误码.
#define MINICC_OK                        ( 0)        ///< 正确返回.
#define MINICC_Error                     (-1)        ///< 未知错误.
#define MINICC_InvalidParams             (-2)        ///< 参数无效.
#define MINICC_InvalidData               (-3)        ///< 数据无效或已损坏.
#define MINICC_LackDataSize              (-4)        ///< 数据缓存开辟不足.
#define MINICC_IllegalDataType           (-5)        ///< 非法字段类型.
#define MINICC_OverLimitDataLength       (-6)        ///< 超出限制数据长度.