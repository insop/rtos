#ifndef		_ERROR_CODE_H_
#define		_ERROR_CODE_H_

const	char	OK			= 1;
const	char	Error			= -1;
const	char	ErrorPriority		= -13;
const 	char	ErrorGetHighPriority	= -15;
const 	char	ErrorInvalidBindFileNum	= -17;
const 	char	ErrorNullBindEntry	= -19;
const 	char	ErrorModuleRead		= -21;
const 	char	ErrorFindModule		= -23;
const	char	ErrorNullAoutHeader	= -25;
const	char	ErrorAoutHeaderMagic	= -27;
const	char	ErrorSetPriorityRange	= -29;
const	char	ErrorCreateWithInvalidPriority		= -31;
const	char	ErrorCreateWithInvalidModuleName	= -33;
const	char	ErrorNullTaskPtRemove	= -35;


#endif//	_ERROR_CODE_H_
